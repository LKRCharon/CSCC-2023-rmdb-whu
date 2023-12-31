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
#include <string>
#include "common/common.h"
#include "common/logger.h"
#include "error.h"
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"
class InsertExecutor : public AbstractExecutor {
   private:
    TabMeta tab_;                // 表的元数据
    std::vector<Value> values_;  // 需要插入的数据
    RmFileHandle *fh_;           // 表的数据文件句柄
    std::string tab_name_;       // 表名称
    Rid rid_;  // 插入的位置，由于系统默认插入时不指定位置，因此当前rid_在插入后才赋值
    SmManager *sm_manager_;

   public:
    InsertExecutor(SmManager *sm_manager, const std::string &tab_name, std::vector<Value> values, Context *context) {
        sm_manager_ = sm_manager;
        tab_ = sm_manager_->db_.get_table(tab_name);
        values_ = values;
        tab_name_ = tab_name;
        if (values.size() != tab_.cols.size()) {
            throw InvalidValueCountError();
        }
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        context_ = context;
        context_->lock_mgr_->lock_IX_on_table(context_->txn_, fh_->GetFd());
    };

    std::unique_ptr<RmRecord> Next() override {
        // Make record buffer
        RmRecord rec(fh_->get_file_hdr().record_size);
        // 准备好要写入的data
        for (size_t i = 0; i < values_.size(); i++) {
            auto &col = tab_.cols[i];
            auto &val = values_[i];
            if (col.type != val.type) {
                // 整数都是正则读的，默认为bigint类型
                if (col.type == TYPE_INT && val.type == TYPE_BIGINT) {
                    val.type = TYPE_INT;
                } else if (col.type == TYPE_DATETIME && val.type == TYPE_STRING) {
                    // 同理 datetime也都是以str的形式正则读进来的
                    // val.type = TYPE_DATETIME;
                    val.set_datetime(DatetimeStrToLL(val.str_val));
                } else {
                    throw IncompatibleTypeError(coltype2str(col.type), coltype2str(val.type));
                }
            }
            val.init_raw(col.len);
            memcpy(rec.data + col.offset, val.raw->data, col.len);
        }
        // Insert into record file
        rid_ = fh_->insert_record(rec.data, context_);
        // 写日志放在插入后，要先插入拿到rid才行
        auto log_rec = new InsertLogRecord(context_->txn_->get_transaction_id(), rec, rid_, tab_name_, false);
        log_rec->prev_lsn_ = context_->txn_->get_prev_lsn();
        context_->txn_->set_prev_lsn(context_->log_mgr_->add_log_to_buffer(log_rec));
        fh_->update_page_lsn(rid_.page_no, log_rec->lsn_);
        delete log_rec;

        // Insert into index
        for (size_t i = 0; i < tab_.indexes.size(); ++i) {
            auto &index = tab_.indexes[i];
            auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
            char *key = new char[index.col_total_len];
            int offset = 0;
            for (int j = 0; j < index.col_num; j++) {
                memcpy(key + offset, rec.data + index.cols[j].offset, index.cols[j].len);
                offset += index.cols[j].len;
            }
            bool is_insert = ih->insert_entry(key, rid_, context_->txn_);
            if (!is_insert) {
                fh_->delete_record(rid_, context_);
                throw IndexEntryRepeatError();
            }
        }
        // context中记录insert语句,放在索引之后，如果repeat了就不会记录
        WriteRecord *write_rec = new WriteRecord(WType::INSERT_TUPLE, tab_name_, rid_);
        context_->txn_->append_write_record(write_rec);

        return nullptr;
    }
    Rid &rid() override { return rid_; }
};