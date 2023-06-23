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

#include "common/logger.h"
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"
// 从rucbase里参考的框架

class SeqScanExecutor : public AbstractExecutor {
   private:
    std::string tab_name_;              // 表的名称
    std::vector<Condition> conds_;      // scan的条件
    RmFileHandle *fh_;                  // 表的数据文件句柄
    std::vector<ColMeta> cols_;         // scan后生成的记录的字段
    size_t len_;                        // scan后生成的每条记录的长度
    std::vector<Condition> fed_conds_;  // 同conds_，两个字段相同

    Rid rid_;
    std::unique_ptr<RecScan> scan_;  // table_iterator

    SmManager *sm_manager_;

   public:
    SeqScanExecutor(SmManager *sm_manager, std::string tab_name, std::vector<Condition> conds, Context *context) {
        sm_manager_ = sm_manager;
        tab_name_ = std::move(tab_name);
        conds_ = std::move(conds);
        TabMeta &tab = sm_manager_->db_.get_table(tab_name_);
        fh_ = sm_manager_->fhs_.at(tab_name_).get();
        cols_ = tab.cols;
        len_ = cols_.back().offset + cols_.back().len;

        context_ = context;

        fed_conds_ = conds_;
        LOG_DEBUG("seqscan cons");
    }

    std::string getType() override { return "SeqScan"; }

    /**
     * @brief 构建表迭代器scan_,并开始迭代扫描,直到扫描到第一个满足谓词条件的元组停止,并赋值给rid_
     *
     */
    void beginTuple() override {
        LOG_DEBUG("seqscan begin tuple");

        check_runtime_conds();

        scan_ = std::make_unique<RmScan>(fh_);

        // 得到第一个满足fed_conds_条件的record,并把其rid赋给算子成员rid_
        while (!scan_->is_end()) {
            rid_ = scan_->rid();
            try {
                auto rec = fh_->get_record(rid_, context_);  // TableHeap->GetTuple() 当前扫描到的记录
                // lab3 task2 todo
                // 利用eval_conds判断是否当前记录(rec.get())满足谓词条件
                if (eval_conds(cols_, fed_conds_, rec.get())) {
                    // 满足则中止循环
                    break;
                }
                // lab3 task2 todo end
            } catch (RecordNotFoundError &e) {
                std::cerr << e.what() << std::endl;
            }

            scan_->next();  // 找下一个有record的位置
        }
    }

    void nextTuple() override {
        check_runtime_conds();
        assert(!is_end());
        for (scan_->next(); !scan_->is_end(); scan_->next()) {  // 用TableIterator遍历TableHeap中的所有Tuple
            // lab3 task2 todo
            // 获取当前记录(参考beginTuple())赋给算子成员rid_
            rid_ = scan_->rid();
            try {
                auto rec = fh_->get_record(rid_, context_);
                // 利用eval_conds判断是否当前记录(rec.get())满足谓词条件
                if (eval_conds(cols_, fed_conds_, rec.get())) {
                    // 满足则中止循环
                    break;
                }
            } catch (RecordNotFoundError &e) {
                std::cerr << e.what() << std::endl;
            }
            // lab3 task2 todo End
        }
    }

    bool is_end() const override { return scan_->is_end(); }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta> &cols() const override { return cols_; }

    std::unique_ptr<RmRecord> Next() override {
        // lab3 task2 todo
        // 利用fh_得到记录record
        return fh_->get_record(rid_, context_);
        // lab3 task2 todo end
    }

    // 不需要重写
    void feed(const std::map<TabCol, Value> &feed_dict) {
        fed_conds_ = conds_;
        for (auto &cond : fed_conds_) {
            // eg. where B.id=A.id , this table is B
            // cond.rhs is not a val but a column
            // and the rhs column's tab name is not this tab
            // need to feed rhs col
            // set the A.id to a val,  the val is  current Table_A Tuple's map.getValue('id');
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
    /**
     * @brief 判断是否满足谓词条件
     *
     */
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