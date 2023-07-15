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
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"

class UpdateExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_;  // 投影节点的儿子节点
    TabMeta tab_;
    std::vector<Condition> conds_;
    RmFileHandle *fh_;
    std::vector<Rid> rids_;
    std::string tab_name_;
    std::vector<SetClause> set_clauses_;
    SmManager *sm_manager_;

   public:
    UpdateExecutor(SmManager *sm_manager, const std::string &tab_name, std::vector<SetClause> set_clauses,
                   std::vector<Condition> conds, std::vector<Rid> rids, Context *context) {
        sm_manager_ = sm_manager;
        tab_name_ = tab_name;
        set_clauses_ = set_clauses;
        tab_ = sm_manager_->db_.get_table(tab_name);
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        conds_ = conds;
        rids_ = rids;
        context_ = context;
    }
    // https://github.com/ruc-deke/rucbase-lab/blob/main/src/execution/executor_update.h
    std::unique_ptr<RmRecord> Next() override {
        // Update each rid from record file and index file
        for (auto &rid : rids_) {
            auto rec = fh_->get_record(rid, context_);
            // old rec 保存原有data数据 防止update 失败
            RmRecord old_rec = RmRecord(rec->size);
            // char *old_data = new char[rec->size];
            memcpy(old_rec.data,rec->data,rec->size);

            // 按照setclause修改rec的数据
            for (auto &set_clause : set_clauses_) {
                auto lhs_col = tab_.get_col(set_clause.lhs.col_name);
                auto val = set_clause.rhs;
                // bigint
                if (lhs_col->type == TYPE_INT && val.type == TYPE_BIGINT) {
                    val.type = TYPE_INT;
                } else if (lhs_col->type == TYPE_DATETIME && val.type == TYPE_STRING) {
                    val.set_datetime(DatetimeStrToLL(val.str_val));
                }
                val.init_raw(lhs_col->len);
                memcpy(rec->data + lhs_col->offset, val.raw->data, lhs_col->len);
            }
            
            // 判断新数据是否与原有索引重复
            for (size_t i = 0; i < tab_.indexes.size(); ++i) {
                auto &index = tab_.indexes.at(i);
                auto ih =
                    sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                int offset = 0;
                char *old_key = new char[index.col_total_len];
                char *new_key = new char[index.col_total_len];
                for (int j = 0; j < index.col_num; j++) {
                    memcpy(new_key + offset, rec->data + index.cols[j].offset, index.cols[j].len);
                    memcpy(old_key + offset, old_rec.data + index.cols[j].offset, index.cols[j].len);
                    offset += index.cols[j].len;
                }
                // ih->delete_entry(old_key, context_->txn_);
                std::vector<Rid> old_rids;
                if(memcmp(new_key,old_key,index.col_total_len)==0){
                    // 索引涉及的列s ，前后key完全一致，不用判断是否有重复索引
                    break;
                }
                if(ih->get_value(new_key,&old_rids,context_->txn_)){
                    // 要update的index已存在
                    throw IndexEntryRepeatError();
                }
                // new出来的字符数组，delete
                delete[] old_key;
                delete[] new_key;
            }
            // Update record in record file
            fh_->update_record(rid, rec->data, context_);

            // 更新索引
            for (size_t i = 0; i < tab_.indexes.size(); ++i) {
                auto &index = tab_.indexes.at(i);
                auto ih =
                    sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                char *insert_key = new char[index.col_total_len];
                char *delete_key = new char[index.col_total_len];
                int offset = 0;
                for (int j = 0; j < index.col_num; j++) {
                    memcpy(insert_key + offset, rec->data + index.cols[j].offset, index.cols[j].len);
                    memcpy(delete_key + offset, old_rec.data + index.cols[j].offset, index.cols[j].len);
                    offset += index.cols[j].len;
                }
                ih->delete_entry(delete_key,context_->txn_);
                bool is_insert = ih->insert_entry(insert_key, rid, context_->txn_);
                if (!is_insert) {
                    // fh_->delete_record(rid, context_);
                    // 这里应该走不到
                    throw IndexEntryRepeatError();
                }
                delete[] insert_key;
                delete[] delete_key;
            }
            WriteRecord *write_rec = new WriteRecord(WType::UPDATE_TUPLE,tab_name_,rid,old_rec);
            context_->txn_->append_write_record(write_rec);
        }
        return nullptr;
    }

    Rid &rid() override { return _abstract_rid; }
};