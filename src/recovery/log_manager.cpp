/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "log_manager.h"
#include <cstring>

/**
 * @description: 添加日志记录到日志缓冲区中，并返回日志记录号
 * @param {LogRecord*} log_record 要写入缓冲区的日志记录
 * @return {lsn_t} 返回该日志的日志记录号
 */
lsn_t LogManager::add_log_to_buffer(LogRecord* log_record) {
    std::unique_lock lock(latch_);

    auto log_len = log_record->log_tot_len_;

    // 循环等待，直到缓冲区有空闲空间
    if (log_buffer_.is_full(log_len)) {
        cv_.wait(lock);
        // 当条件变量被唤醒后，继续检查缓冲区是否满，以防止虚假唤醒
    }
    log_record->lsn_ = global_lsn_.fetch_add(1);

    auto dest = log_buffer_.buffer_ + log_buffer_.offset_;
    log_record->serialize(dest);
    log_buffer_.offset_ += log_record->log_tot_len_;

    return log_record->lsn_;
}

/**
 * @description: 把日志缓冲区的内容刷到磁盘中，由于目前只设置了一个缓冲区，因此需要阻塞其他日志操作
 */
void LogManager::flush_log_to_disk() {
    std::unique_lock lock(latch_);

    disk_manager_->write_log(log_buffer_.buffer_, log_buffer_.offset_);

    persist_lsn_ = global_lsn_;
    memset(log_buffer_.buffer_, 0, log_buffer_.offset_);
    log_buffer_.offset_ = 0;

    // 唤醒一个等待的线程，表示缓冲区有空闲空间
    cv_.notify_one();
}

void LogManager::set_global_lsn_(lsn_t lsn) { global_lsn_.store(lsn); };
