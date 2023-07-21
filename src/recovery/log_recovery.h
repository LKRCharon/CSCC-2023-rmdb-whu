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

#include <map>
#include <unordered_map>
#include "log_manager.h"
#include "storage/disk_manager.h"
#include "system/sm_manager.h"

class RedoLogsInPage {
   public:
    RedoLogsInPage() { table_file_ = nullptr; }
    RmFileHandle* table_file_;
    std::vector<lsn_t> redo_logs_;  // 在该page上需要redo的操作的lsn
};

class RecoveryManager {
   public:
    RecoveryManager(DiskManager* disk_manager, BufferPoolManager* buffer_pool_manager, SmManager* sm_manager,
                    LogManager* log_manager) {
        disk_manager_ = disk_manager;
        bpm_ = buffer_pool_manager;
        sm_manager_ = sm_manager;
        log_manager_ = log_manager;
    }

    void analyze();
    void redo();
    void undo();

   private:
    LogBuffer buffer_;                               // 读入日志
    DiskManager* disk_manager_;                      // 用来读写文件
    BufferPoolManager* bpm_;                         // 对页面进行读写
    SmManager* sm_manager_;                          // 访问数据库元数据
    std::unordered_map<lsn_t, int> lsn_offsets_;     // 记录lsn与offset
    std::unordered_map<lsn_t, int> log_lens_;        // 记录每个log的size
    std::unordered_map<txn_id_t, lsn_t> undo_lsns_;  // 需要undo的事务的最后一条lsn   ATT
    std::vector<lsn_t> redo_logs_;                   // 需要redo的log集合 DPT
    LogManager* log_manager_;                        //用来维护全局的lsn
};
