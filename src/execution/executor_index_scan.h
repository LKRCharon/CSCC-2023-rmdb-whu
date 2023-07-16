/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include <set>
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"

class IndexScanExecutor : public AbstractExecutor {
   private:
    std::string tab_name_;          // 表名称
    TabMeta tab_;                   // 表的元数据
    std::vector<Condition> conds_;  // 扫描条件  charon:这里是已经处理好的，等号在最前 大于小于在后面
    RmFileHandle *fh_;              // 表的数据文件句柄
    std::vector<ColMeta> cols_;         // 需要读取的字段
    size_t len_;                        // 选取出来的一条记录的长度
    std::vector<Condition> fed_conds_;  // 扫描条件，和conds_字段相同

    int key_len_ = 0;                           // 索引key的总长度
    std::vector<std::string> index_col_names_;  // index scan涉及到的索引包含的字段
    IndexMeta index_meta_;                      // index scan涉及到的索引元数据
    std::string index_name_;                    //索引对应的文件名
    // int used_index_num = 0;  //使用的索引列数，考虑多列索引但是where是单列的情况

    Rid rid_;
    std::unique_ptr<IxScan> scan_;

    SmManager *sm_manager_;

   public:
    IndexScanExecutor(SmManager *sm_manager, std::string tab_name, std::vector<Condition> conds, IndexMeta index_meta,
                      Context *context) {
        sm_manager_ = sm_manager;
        context_ = context;
        context_->lock_mgr_->lock_shared_on_table(context_->txn_,fh_->GetFd());

        tab_name_ = std::move(tab_name);
        tab_ = sm_manager_->db_.get_table(tab_name_);
        conds_ = std::move(conds);
        // 改了这里，获得indexname和indexmeta的方法
        index_meta_ = index_meta;
        index_name_ = tab_name_;
        for (auto &col : index_meta_.cols) {
            index_name_ += "_" + col.name;
        }
        index_name_ += ".idx";

        fh_ = sm_manager_->fhs_.at(tab_name_).get();
        cols_ = tab_.cols;
        len_ = cols_.back().offset + cols_.back().len;

        std::map<CompOp, CompOp> swap_op = {
            {OP_EQ, OP_EQ}, {OP_NE, OP_NE}, {OP_LT, OP_GT}, {OP_GT, OP_LT}, {OP_LE, OP_GE}, {OP_GE, OP_LE},
        };

        for (auto &cond : conds_) {
            if (cond.lhs_col.tab_name != tab_name_) {
                // lhs is on other table, now rhs must be on this table
                assert(!cond.is_rhs_val && cond.rhs_col.tab_name == tab_name_);
                // swap lhs and rhs
                std::swap(cond.lhs_col, cond.rhs_col);
                cond.op = swap_op.at(cond.op);
            }
        }
        fed_conds_ = conds_;
    }
    IndexScanExecutor(SmManager *sm_manager, std::string tab_name, std::vector<Condition> conds,
                      std::vector<std::string> index_col_names, Context *context) {
        sm_manager_ = sm_manager;
        context_ = context;
        tab_name_ = std::move(tab_name);
        tab_ = sm_manager_->db_.get_table(tab_name_);
        conds_ = std::move(conds);
        // index_no_ = index_no;
        index_col_names_ = index_col_names;
        index_meta_ = *(tab_.get_index_meta(index_col_names_));
        fh_ = sm_manager_->fhs_.at(tab_name_).get();
        cols_ = tab_.cols;
        len_ = cols_.back().offset + cols_.back().len;
        std::map<CompOp, CompOp> swap_op = {
            {OP_EQ, OP_EQ}, {OP_NE, OP_NE}, {OP_LT, OP_GT}, {OP_GT, OP_LT}, {OP_LE, OP_GE}, {OP_GE, OP_LE},
        };

        for (auto &cond : conds_) {
            if (cond.lhs_col.tab_name != tab_name_) {
                // lhs is on other table, now rhs must be on this table
                assert(!cond.is_rhs_val && cond.rhs_col.tab_name == tab_name_);
                // swap lhs and rhs
                std::swap(cond.lhs_col, cond.rhs_col);
                cond.op = swap_op.at(cond.op);
            }
        }
        fed_conds_ = conds_;
    }

    std::string getType() override { return "indexScan"; }

    void beginTuple() override {
        check_runtime_conds();

        // index is available, scan index
        auto ih = sm_manager_->ihs_.at(index_name_).get();
        Iid lower = ih->leaf_begin();
        Iid upper = ih->leaf_end();

        char *key = new char[index_meta_.col_total_len];
        memset(key, 0, index_meta_.col_total_len);

        // 根据索引的信息，调整lowerbound和upperbound的位置
        // 遍历cond 准备好key 改好upper和lower
        std::set<std::string> used_col_names_set;
        int offset = 0;
        for (int i = 0; i < fed_conds_.size(); i++) {
            const auto cond = fed_conds_.at(i);
            auto index_col = index_meta_.get_col(cond.lhs_col.col_name);
            if (used_col_names_set.count(cond.lhs_col.col_name) == 0) {
                used_col_names_set.emplace(cond.lhs_col.col_name);
                memcpy(key + offset, cond.rhs_val.raw->data, index_col.len);
            } else {
                memcpy(key + offset - index_col.len, cond.rhs_val.raw->data, index_col.len);
            }
            int used_index_num = (int)used_col_names_set.size();
            if (cond.op == OP_EQ) {
                lower = ih->lower_bound(key, used_index_num);
                upper = ih->upper_bound(key, used_index_num,true);
            } else if (cond.op == OP_GE) {
                lower = ih->lower_bound(key, used_index_num);
            } else if (cond.op == OP_LE) {
                // find leaf 不使用num，在叶子节点二分才用num
                upper = ih->upper_bound(key, used_index_num,true);
            } else if (cond.op == OP_GT) {
                lower = ih->upper_bound(key, used_index_num,false);
            } else if (cond.op == OP_LT) {
                upper = ih->lower_bound(key, used_index_num);
            }
            offset += index_col.len;
        }
        scan_ = std::make_unique<IxScan>(ih, lower, upper, sm_manager_->get_bpm());
        if (!is_end()) {
            rid_ = scan_->rid();
            try {
                auto rec = fh_->get_record(rid_, context_);
                if (!eval_conds(cols_, fed_conds_, rec.get())) {
                    scan_->set_end();
                }
            } catch (RecordNotFoundError &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    void nextTuple() override {
        check_runtime_conds();
        assert(!is_end());

        scan_->next();
        if (is_end()) {
            return;
        }
        rid_ = scan_->rid();

        try {
            auto rec = fh_->get_record(rid_, context_);
            if (!eval_conds(cols_, fed_conds_, rec.get())) {
                scan_->set_end();
            }
        } catch (RecordNotFoundError &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    bool is_end() const override { return scan_->is_end(); }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta> &cols() const override { return cols_; }

    std::unique_ptr<RmRecord> Next() override {
        assert(!is_end());
        return fh_->get_record(rid_, context_);
    }

    void feed(const std::map<TabCol, Value> &feed_dict) override {
        fed_conds_ = conds_;
        for (auto &cond : fed_conds_) {
            if (!cond.is_rhs_val && cond.rhs_col.tab_name != tab_name_) {
                cond.is_rhs_val = true;
                cond.rhs_val = feed_dict.at(cond.rhs_col);
            }
        }
        check_runtime_conds();
    }

    Rid &rid() override { return rid_; }

    void check_runtime_conds() {
        for (auto &cond : fed_conds_) {
            assert(cond.lhs_col.tab_name == tab_name_);
            if (!cond.is_rhs_val) {
                assert(cond.rhs_col.tab_name == tab_name_);
            }
        }
    }

    bool eval_cond(const std::vector<ColMeta> &rec_cols, const Condition &cond, const RmRecord *rec) {
        auto lhs_col = get_col(rec_cols, cond.lhs_col);
        char *lhs = rec->data + lhs_col->offset;
        char *rhs;
        ColType rhs_type;
        if (cond.is_rhs_val) {
            rhs_type = cond.rhs_val.type;
            rhs = cond.rhs_val.raw->data;
        } else {
            // rhs is a column
            auto rhs_col = get_col(rec_cols, cond.rhs_col);
            rhs_type = rhs_col->type;
            rhs = rec->data + rhs_col->offset;
        }
        assert(rhs_type == lhs_col->type);  // TODO convert to common type
        int cmp = ix_compare(lhs, rhs, rhs_type, lhs_col->len);
        if (cond.op == OP_EQ) {
            return cmp == 0;
        } else if (cond.op == OP_NE) {
            return cmp != 0;
        } else if (cond.op == OP_LT) {
            return cmp < 0;
        } else if (cond.op == OP_GT) {
            return cmp > 0;
        } else if (cond.op == OP_LE) {
            return cmp <= 0;
        } else if (cond.op == OP_GE) {
            return cmp >= 0;
        } else {
            throw InternalError("Unexpected op type");
        }
    }

    bool eval_conds(const std::vector<ColMeta> &rec_cols, const std::vector<Condition> &conds, const RmRecord *rec) {
        return std::all_of(conds.begin(), conds.end(),
                           [&](const Condition &cond) { return eval_cond(rec_cols, cond, rec); });
    }
};