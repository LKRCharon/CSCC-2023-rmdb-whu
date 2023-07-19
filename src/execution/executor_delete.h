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

class DeleteExecutor : public AbstractExecutor {
   private:
    TabMeta tab_;                   // 表的元数据
    std::vector<Condition> conds_;  // delete的条件
    RmFileHandle *fh_;              // 表的数据文件句柄
    std::vector<Rid> rids_;         // 需要删除的记录的位置
    std::string tab_name_;          // 表名称
    SmManager *sm_manager_;

   public:
    DeleteExecutor(SmManager *sm_manager, const std::string &tab_name, std::vector<Condition> conds,
                   std::vector<Rid> rids, Context *context) {
        sm_manager_ = sm_manager;
        tab_name_ = tab_name;
        tab_ = sm_manager_->db_.get_table(tab_name);
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        conds_ = conds;
        rids_ = rids;
        context_ = context;
        context_->lock_mgr_->lock_IX_on_table(context_->txn_,fh_->GetFd());
    }

    std::unique_ptr<RmRecord> Next() override {
        // 翻转记录删除操作的顺序
        std::deque<WriteRecord*> dq4reverse;
        // Delete each rid from record file and index file
        for (auto &rid : rids_) {
            auto rec = fh_->get_record(rid, context_);
            for (size_t i = 0; i < tab_.indexes.size(); ++i) {
                auto &index = tab_.indexes.at(i);
                auto ih =
                    sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                char *key = new char[index.col_total_len];
                int offset = 0;
                for (int j = 0; j < index.col_num; j++) {
                    memcpy(key + offset, rec->data + index.cols[j].offset, index.cols[j].len);
                    offset += index.cols[j].len;
                }
                ih->delete_entry(key, context_->txn_);
                delete[] key;
            }
            fh_->delete_record(rid, context_);

            // 记一下删了那些rec
            RmRecord old_rec(rec->size);
            memcpy(old_rec.data,rec->data,rec->size);

            WriteRecord *write_rec = new WriteRecord(WType::DELETE_TUPLE,tab_name_,rid,old_rec);
            dq4reverse.push_back(write_rec);
        }
        context_->txn_->append_write_records_reverse(dq4reverse);
        return nullptr;
    }

    Rid &rid() override { return _abstract_rid; }
};