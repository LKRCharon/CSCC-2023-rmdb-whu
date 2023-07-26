/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "log_recovery.h"
#include "common/config.h"

/**
 * @description: analyze阶段，需要获得脏页表（DPT）和未完成的事务列表（ATT）
 */
void RecoveryManager::analyze() {
    //用来从日志文件中读取
    char *read_buf = new char[LOG_BUFFER_SIZE];
    // read_buf的偏置
    int buffer_offset = 0;
    //定位文件的偏置
    int file_offset = 0;
    //每次读入的字节数
    int num_bytes = 0;
    //循环读取日志文件
    while (true) {
        memset(read_buf, 0, LOG_BUFFER_SIZE);
        buffer_offset = 0;
        num_bytes = disk_manager_->read_log(read_buf, LOG_BUFFER_SIZE, file_offset);
        if (num_bytes < 0) {
            throw InternalError("read log file error!");
        }
        // 先判断read_buf里有无下一条log的完整长度信息
        while (buffer_offset + OFFSET_LOG_TOT_LEN + sizeof(uint32_t) < num_bytes) {
            // 获取log的size
            int log_size = *reinterpret_cast<uint32_t *>(read_buf + buffer_offset + OFFSET_LOG_TOT_LEN);
            if (buffer_offset + log_size > num_bytes) {
                // read_buf里log信息不全，刷新重读
                break;
            }
            LogType log_type = *reinterpret_cast<LogType *>(read_buf + buffer_offset + OFFSET_LOG_TYPE);
            is_with_txn = true;
            switch (log_type) {
                case LogType::BEGIN: {
                    BeginLogRecord *log_rec = new BeginLogRecord();
                    log_rec->deserialize(read_buf + buffer_offset);
                    lsn_offsets_[log_rec->lsn_] = file_offset;
                    log_lens_[log_rec->lsn_] = log_rec->log_tot_len_;
                    buffer_offset += log_rec->log_tot_len_;
                    file_offset += log_rec->log_tot_len_;
                    undo_lsns_[log_rec->log_tid_] = log_rec->lsn_;
                    txn_manager_->set_next_txn_id(log_rec->log_tid_);
                    log_manager_->set_global_lsn_(log_rec->lsn_);
                    delete log_rec;
                    break;
                }
                case LogType::COMMIT: {
                    CommitLogRecord *log_rec = new CommitLogRecord();
                    log_rec->deserialize(read_buf + buffer_offset);
                    lsn_offsets_[log_rec->lsn_] = file_offset;
                    log_lens_[log_rec->lsn_] = log_rec->log_tot_len_;
                    buffer_offset += log_rec->log_tot_len_;
                    file_offset += log_rec->log_tot_len_;
                    undo_lsns_.erase(log_rec->log_tid_);
                    txn_manager_->set_next_txn_id(log_rec->log_tid_);
                    log_manager_->set_global_lsn_(log_rec->lsn_);
                    delete log_rec;
                    break;
                }
                case LogType::ABORT: {
                    AbortLogRecord *log_rec = new AbortLogRecord();
                    log_rec->deserialize(read_buf + buffer_offset);
                    lsn_offsets_[log_rec->lsn_] = file_offset;
                    log_lens_[log_rec->lsn_] = log_rec->log_tot_len_;
                    buffer_offset += log_rec->log_tot_len_;
                    file_offset += log_rec->log_tot_len_;
                    undo_lsns_.erase(log_rec->log_tid_);
                    txn_manager_->set_next_txn_id(log_rec->log_tid_);

                    log_manager_->set_global_lsn_(log_rec->lsn_);
                    delete log_rec;
                    break;
                }
                case LogType::INSERT: {
                    InsertLogRecord *log_rec = new InsertLogRecord();
                    log_rec->deserialize(read_buf + buffer_offset);
                    lsn_offsets_[log_rec->lsn_] = file_offset;
                    log_lens_[log_rec->lsn_] = log_rec->log_tot_len_;
                    buffer_offset += log_rec->log_tot_len_;
                    file_offset += log_rec->log_tot_len_;
                    undo_lsns_[log_rec->log_tid_] = log_rec->lsn_;
                    redo_logs_.push_back({log_rec->lsn_, true});
                    txn_manager_->set_next_txn_id(log_rec->log_tid_);
                    log_manager_->set_global_lsn_(log_rec->lsn_);
                    delete log_rec;
                    break;
                }
                case LogType::DELETE: {
                    DeleteLogRecord *log_rec = new DeleteLogRecord();
                    log_rec->deserialize(read_buf + buffer_offset);
                    lsn_offsets_[log_rec->lsn_] = file_offset;
                    log_lens_[log_rec->lsn_] = log_rec->log_tot_len_;
                    buffer_offset += log_rec->log_tot_len_;
                    file_offset += log_rec->log_tot_len_;
                    undo_lsns_[log_rec->log_tid_] = log_rec->lsn_;
                    redo_logs_.push_back({log_rec->lsn_, true});

                    txn_manager_->set_next_txn_id(log_rec->log_tid_);

                    log_manager_->set_global_lsn_(log_rec->lsn_);
                    delete log_rec;
                    break;
                }
                case LogType::UPDATE: {
                    UpdateLogRecord *log_rec = new UpdateLogRecord();
                    log_rec->deserialize(read_buf + buffer_offset);
                    lsn_offsets_[log_rec->lsn_] = file_offset;
                    log_lens_[log_rec->lsn_] = log_rec->log_tot_len_;
                    buffer_offset += log_rec->log_tot_len_;
                    file_offset += log_rec->log_tot_len_;
                    undo_lsns_[log_rec->log_tid_] = log_rec->lsn_;
                    redo_logs_.push_back({log_rec->lsn_, true});

                    txn_manager_->set_next_txn_id(log_rec->log_tid_);

                    log_manager_->set_global_lsn_(log_rec->lsn_);
                    delete log_rec;
                    break;
                }
                default:
                    break;
            }
        }
        if (num_bytes < LOG_BUFFER_SIZE) {
            break;
        }
    }

    // 读取完全部日志文件，根据undo删掉redo里的
    for (auto it = undo_lsns_.begin(); it != undo_lsns_.end(); it++) {
        lsn_t lsn = it->second;
        std::vector<std::pair<lsn_t, bool>>::reverse_iterator vit = redo_logs_.rbegin();
        while (lsn != INVALID_LSN) {
            int log_len = log_lens_[lsn];
            char *log_buf = new char[log_len];
            memset(log_buf, 0, log_len);
            disk_manager_->read_log(log_buf, log_len, lsn_offsets_[lsn]);
            LogType log_type = *(LogType *)log_buf;
            switch (log_type) {
                case LogType::BEGIN: {
                    lsn = -1;
                    break;
                }
                case LogType::INSERT: {
                    InsertLogRecord *log_rec = new InsertLogRecord();
                    log_rec->deserialize(log_buf);
                    while (vit != redo_logs_.rend() && vit->first != lsn) {
                        vit++;
                    }
                    vit->second = false;
                    // vit++;
                    lsn = log_rec->prev_lsn_;
                    delete log_rec;

                    break;
                }
                case LogType::DELETE: {
                    DeleteLogRecord *log_rec = new DeleteLogRecord();
                    log_rec->deserialize(log_buf);
                    while (vit != redo_logs_.rend() && vit->first != lsn) {
                        vit++;
                    }
                    vit->second = false;
                    // vit++;
                    lsn = log_rec->prev_lsn_;
                    delete log_rec;

                    break;
                }
                case LogType::UPDATE: {
                    UpdateLogRecord *log_rec = new UpdateLogRecord();
                    log_rec->deserialize(log_buf);
                    while (vit != redo_logs_.rend() && vit->first != lsn) {
                        vit++;
                    }
                    vit->second = false;
                    // vit++;
                    lsn = log_rec->prev_lsn_;
                    delete log_rec;

                    break;
                }
                default:
                    // 其他情况的处理
                    break;
            }
            delete[] log_buf;
        }
    }

    delete[] read_buf;
}

/**
 * @description: 重做所有操作
 */
void RecoveryManager::redo() {
    for (auto pair : redo_logs_) {
        if (pair.second == false) {
            continue;
        }
        //准备读入log
        auto rd = pair.first;
        int log_len = log_lens_[rd];
        char *log_buf = new char[log_len];
        memset(log_buf, 0, log_len);
        disk_manager_->read_log(log_buf, log_len, lsn_offsets_[rd]);
        LogType log_type = *(LogType *)log_buf;
        switch (log_type) {
            case LogType::INSERT: {
                InsertLogRecord *log_rec = new InsertLogRecord();
                log_rec->deserialize(log_buf);
                std::string tab_name(log_rec->table_name_, log_rec->table_name_size_);
                // auto page_handle = sm_manager_->fhs_.at(tab_name)->fetch_page_handle(log_rec->rid_.page_no);
                // if (page_handle.page->get_page_lsn() > log_rec->lsn_) {
                //     sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), false);
                //     break;
                // }
                // 根据是否为回滚里的insert采用不同的插入函数
                if (log_rec->is_rollback_) {
                    sm_manager_->fhs_.at(tab_name)->insert_record(log_rec->rid_, log_rec->insert_value_.data);
                } else {
                    log_rec->rid_ = sm_manager_->fhs_.at(tab_name)->insert_record(log_rec->insert_value_.data, nullptr);
                }
                // sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), true);

                TabMeta &tab = sm_manager_->db_.get_table(tab_name);

                for (size_t i = 0; i < tab.indexes.size(); ++i) {
                    auto &index = tab.indexes.at(i);
                    auto ih =
                        sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols)).get();
                    char *key = new char[index.col_total_len];
                    int offset = 0;
                    for (int j = 0; j < index.col_num; j++) {
                        memcpy(key + offset, log_rec->insert_value_.data + index.cols[j].offset, index.cols[j].len);
                        offset += index.cols[j].len;
                    }
                    bool is_insert = ih->insert_entry(key, log_rec->rid_, nullptr);
                    if (!is_insert) {
                        sm_manager_->fhs_.at(tab_name)->delete_record(log_rec->rid_, nullptr);
                    }
                    delete[] key;
                }
                delete log_rec;
                break;
            }
            case LogType::DELETE: {
                DeleteLogRecord *log_rec = new DeleteLogRecord();
                log_rec->deserialize(log_buf);
                std::string tab_name(log_rec->table_name_, log_rec->table_name_size_);
                // auto page_handle = sm_manager_->fhs_.at(tab_name)->fetch_page_handle(log_rec->rid_.page_no);
                // if (page_handle.page->get_page_lsn() > log_rec->lsn_) {
                //     sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), false);
                //     break;
                // }

                TabMeta &tab = sm_manager_->db_.get_table(tab_name);
                for (size_t i = 0; i < tab.indexes.size(); ++i) {
                    auto &index = tab.indexes.at(i);
                    auto ih =
                        sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols)).get();
                    char *key = new char[index.col_total_len];
                    int offset = 0;
                    for (int j = 0; j < index.col_num; j++) {
                        memcpy(key + offset, log_rec->delete_value_.data + index.cols[j].offset, index.cols[j].len);
                        offset += index.cols[j].len;
                    }
                    ih->delete_entry(key, nullptr);
                    delete[] key;
                }
                sm_manager_->fhs_.at(tab_name)->delete_record(log_rec->rid_, nullptr);
                // sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), true);
                delete log_rec;
                break;
            }
            case LogType::UPDATE: {
                UpdateLogRecord *log_rec = new UpdateLogRecord();
                log_rec->deserialize(log_buf);
                std::string tab_name(log_rec->table_name_, log_rec->table_name_size_);
                // auto page_handle = sm_manager_->fhs_.at(tab_name)->fetch_page_handle(log_rec->rid_.page_no);
                // if (page_handle.page->get_page_lsn() > log_rec->lsn_) {
                //     sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), false);
                //     break;
                // }
                // 开始update索引：
                TabMeta &tab = sm_manager_->db_.get_table(tab_name);
                // 1. 判断新数据是否与原有索引重复
                for (size_t i = 0; i < tab.indexes.size(); ++i) {
                    auto &index = tab.indexes.at(i);
                    auto ih =
                        sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols)).get();
                    int offset = 0;
                    char *old_key = new char[index.col_total_len];
                    char *new_key = new char[index.col_total_len];
                    for (int j = 0; j < index.col_num; j++) {
                        memcpy(new_key + offset, log_rec->after_value_.data + index.cols[j].offset, index.cols[j].len);
                        memcpy(old_key + offset, log_rec->before_value_.data + index.cols[j].offset, index.cols[j].len);
                        offset += index.cols[j].len;
                    }

                    std::vector<Rid> old_rids;
                    if (memcmp(new_key, old_key, index.col_total_len) == 0) {
                        // 索引涉及的列s，前后key完全一致，不用判断是否有重复索引
                        break;
                    }
                    if (ih->get_value(new_key, &old_rids, nullptr)) {
                        // 要update的index已存在
                        throw IndexEntryRepeatError();
                    }
                    // new出来的字符数组，delete
                    delete[] old_key;
                    delete[] new_key;
                }
                // 更新记录数据
                sm_manager_->fhs_.at(tab_name)->update_record(log_rec->rid_, log_rec->after_value_.data, nullptr);
                // sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), true);

                // 2. 更新索引
                for (size_t i = 0; i < tab.indexes.size(); ++i) {
                    auto &index = tab.indexes.at(i);
                    auto ih =
                        sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols)).get();
                    char *insert_key = new char[index.col_total_len];
                    char *delete_key = new char[index.col_total_len];
                    int offset = 0;
                    for (int j = 0; j < index.col_num; j++) {
                        memcpy(insert_key + offset, log_rec->after_value_.data + index.cols[j].offset,
                               index.cols[j].len);
                        memcpy(delete_key + offset, log_rec->before_value_.data + index.cols[j].offset,
                               index.cols[j].len);
                        offset += index.cols[j].len;
                    }
                    ih->delete_entry(delete_key, nullptr);
                    bool is_insert = ih->insert_entry(insert_key, log_rec->rid_, nullptr);
                    if (!is_insert) {
                        // fh_->delete_record(rid, context_);
                        // 这里应该走不到
                        throw IndexEntryRepeatError();
                    }
                    delete[] insert_key;
                    delete[] delete_key;
                }
                delete log_rec;
                break;
            }
            default:
                break;
        }
        delete[] log_buf;
    }
}

/**
 * @description: 回滚未完成的事务
 */
void RecoveryManager::undo() {
    for (auto it = undo_lsns_.begin(); it != undo_lsns_.end(); it++) {
        lsn_t lsn = it->second;
        while (lsn != INVALID_LSN) {
            int log_len = log_lens_[lsn];
            char *log_buf = new char[log_len];
            memset(log_buf, 0, log_len);
            disk_manager_->read_log(log_buf, log_len, lsn_offsets_[lsn]);
            LogType log_type = *(LogType *)log_buf;
            switch (log_type) {
                case LogType::BEGIN: {
                    BeginLogRecord *log_rec = new BeginLogRecord();
                    log_rec->deserialize(log_buf);
                    lsn = log_rec->prev_lsn_;
                    delete log_rec;
                    break;
                }
                case LogType::INSERT: {
                    InsertLogRecord *log_rec = new InsertLogRecord();
                    log_rec->deserialize(log_buf);
                    lsn = log_rec->prev_lsn_;
                    std::string tab_name(log_rec->table_name_, log_rec->table_name_size_);
                    // auto page_handle = sm_manager_->fhs_.at(tab_name)->fetch_page_handle(log_rec->rid_.page_no);
                    // if (page_handle.page->get_page_lsn() > log_rec->lsn_) {
                    //     sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), false);
                    //     break;
                    // }

                    TabMeta &tab = sm_manager_->db_.get_table(tab_name);
                    for (size_t i = 0; i < tab.indexes.size(); ++i) {
                        auto &index = tab.indexes.at(i);
                        auto ih =
                            sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols))
                                .get();
                        char *key = new char[index.col_total_len];
                        int offset = 0;
                        for (int j = 0; j < index.col_num; j++) {
                            memcpy(key + offset, log_rec->insert_value_.data + index.cols[j].offset, index.cols[j].len);
                            offset += index.cols[j].len;
                        }
                        ih->delete_entry(key, nullptr);
                        delete[] key;
                    }

                    sm_manager_->fhs_.at(tab_name)->delete_record(log_rec->rid_, nullptr);
                    // sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), true);
                    delete log_rec;
                    break;
                }
                case LogType::DELETE: {
                    DeleteLogRecord *log_rec = new DeleteLogRecord();
                    log_rec->deserialize(log_buf);
                    lsn = log_rec->prev_lsn_;
                    std::string tab_name(log_rec->table_name_, log_rec->table_name_size_);
                    // auto page_handle = sm_manager_->fhs_.at(tab_name)->fetch_page_handle(log_rec->rid_.page_no);
                    // if (page_handle.page->get_page_lsn() > log_rec->lsn_) {
                    //     sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), false);
                    //     break;
                    // }

                    sm_manager_->fhs_.at(tab_name)->insert_record(log_rec->rid_, log_rec->delete_value_.data);
                    // sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), true);

                    TabMeta &tab = sm_manager_->db_.get_table(tab_name);
                    for (size_t i = 0; i < tab.indexes.size(); ++i) {
                        auto &index = tab.indexes.at(i);
                        auto ih =
                            sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols))
                                .get();
                        char *key = new char[index.col_total_len];
                        int offset = 0;
                        for (int j = 0; j < index.col_num; j++) {
                            memcpy(key + offset, log_rec->delete_value_.data + index.cols[j].offset, index.cols[j].len);
                            offset += index.cols[j].len;
                        }
                        bool is_insert = ih->insert_entry(key, log_rec->rid_, nullptr);
                        if (!is_insert) {
                            sm_manager_->fhs_.at(tab_name)->delete_record(log_rec->rid_, nullptr);
                        }
                        delete[] key;
                    }
                    delete log_rec;
                    break;
                }
                case LogType::UPDATE: {
                    UpdateLogRecord *log_rec = new UpdateLogRecord();
                    log_rec->deserialize(log_buf);
                    lsn = log_rec->prev_lsn_;
                    std::string tab_name(log_rec->table_name_, log_rec->table_name_size_);
                    // auto page_handle = sm_manager_->fhs_.at(tab_name)->fetch_page_handle(log_rec->rid_.page_no);
                    // if (page_handle.page->get_page_lsn() > log_rec->lsn_) {
                    //     sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), false);
                    //     break;
                    // }
                    // 开始update索引：
                    TabMeta &tab = sm_manager_->db_.get_table(tab_name);
                    // 1. 判断新数据是否与原有索引重复
                    for (size_t i = 0; i < tab.indexes.size(); ++i) {
                        auto &index = tab.indexes.at(i);
                        auto ih =
                            sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols))
                                .get();
                        int offset = 0;
                        char *old_key = new char[index.col_total_len];
                        char *new_key = new char[index.col_total_len];
                        for (int j = 0; j < index.col_num; j++) {
                            memcpy(new_key + offset, log_rec->before_value_.data + index.cols[j].offset,
                                   index.cols[j].len);
                            memcpy(old_key + offset, log_rec->after_value_.data + index.cols[j].offset,
                                   index.cols[j].len);
                            offset += index.cols[j].len;
                        }

                        std::vector<Rid> old_rids;
                        if (memcmp(new_key, old_key, index.col_total_len) == 0) {
                            // 索引涉及的列s，前后key完全一致，不用判断是否有重复索引
                            break;
                        }
                        if (ih->get_value(new_key, &old_rids, nullptr)) {
                            // 要update的index已存在
                            throw IndexEntryRepeatError();
                        }
                        // new出来的字符数组，delete
                        delete[] old_key;
                        delete[] new_key;
                    }
                    sm_manager_->fhs_.at(tab_name)->update_record(log_rec->rid_, log_rec->before_value_.data, nullptr);
                    // sm_manager_->get_bpm()->unpin_page(page_handle.page->get_page_id(), true);

                    // 2. 更新索引
                    for (size_t i = 0; i < tab.indexes.size(); ++i) {
                        auto &index = tab.indexes.at(i);
                        auto ih =
                            sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name, index.cols))
                                .get();
                        char *insert_key = new char[index.col_total_len];
                        char *delete_key = new char[index.col_total_len];
                        int offset = 0;
                        for (int j = 0; j < index.col_num; j++) {
                            memcpy(insert_key + offset, log_rec->before_value_.data + index.cols[j].offset,
                                   index.cols[j].len);
                            memcpy(delete_key + offset, log_rec->after_value_.data + index.cols[j].offset,
                                   index.cols[j].len);
                            offset += index.cols[j].len;
                        }
                        ih->delete_entry(delete_key, nullptr);
                        bool is_insert = ih->insert_entry(insert_key, log_rec->rid_, nullptr);
                        if (!is_insert) {
                            // fh_->delete_record(rid, context_);
                            // 这里应该走不到
                            throw IndexEntryRepeatError();
                        }
                        delete[] insert_key;
                        delete[] delete_key;
                    }
                    delete log_rec;
                    break;
                }

                default:
                    break;
            }
            delete[] log_buf;
        }
    }
}

// void RecoveryManager::flush() { bpm_->flush_all_pages(); }