/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "lock_manager.h"

/**
 * @description: 申请行级共享锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID 记录所在的表的fd
 * @param {int} tab_fd
 */
bool LockManager::lock_shared_on_record(Transaction* txn, const Rid& rid, int tab_fd) {
    // Todo:
    // 1. 通过mutex申请访问全局锁表
    std::unique_lock lock(latch_);
    // 2. 检查事务的状态
    if (txn->get_state() == TransactionState::ABORTED) {
        return false;
    }
    // 3. 查找当前事务是否已经申请了目标数据项上的锁，如果存在则根据锁类型进行操作，否则执行下一步操作
    auto lock_data_id = LockDataId(tab_fd, rid, LockDataType::RECORD);
    auto iter = lock_table_.find(lock_data_id);
    if (iter != lock_table_.end()) {
        auto& lock_request_q = iter->second.request_queue_;
        for (const auto& req : lock_request_q) {
            if (req.txn_id_ == txn->get_transaction_id()) {
                if (req.lock_mode_ == LockMode::SHARED) {
                    return true;
                }
            }
        }
        // 4. 将要申请的锁放入到全局锁表中，并通过组模式来判断是否可以成功授予锁
        // 5. 如果成功，更新目标数据项在全局锁表中的信息，否则阻塞当前操作
        // 提示：步骤5中的阻塞操作可以通过条件变量来完成，所有加锁操作都遵循上述步骤，在下面的加锁操作中不再进行注释提示
    }
    return true;
}

/**
 * @description: 申请行级排他锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID
 * @param {int} tab_fd 记录所在的表的fd
 */
bool LockManager::lock_exclusive_on_record(Transaction* txn, const Rid& rid, int tab_fd) { return true; }

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {
    std::unique_lock lock(latch_);
    auto lock_data_id = LockDataId(tab_fd, LockDataType::TABLE);
    auto iter = lock_table_.find(lock_data_id);
    // if(lock_table_.count(lock_data_id)>0){
    //     return true;
    // }
    if (iter != lock_table_.end()) {
        for (const auto& req : iter->second.request_queue_) {
            // 自己请求过锁，无论S锁还是X锁都可认为成功
            if (req.txn_id_ == txn->get_transaction_id()) {
                return true;
            }
            if (req.lock_mode_ == LockMode::EXLUCSIVE) {
                throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
            }
        }
    }

    txn->get_lock_set()->insert(lock_data_id);
    LockRequest* request = new LockRequest(txn->get_transaction_id(), LockMode::SHARED);
    lock_table_[lock_data_id].request_queue_.push_back(*request);
    lock_table_[lock_data_id].shared_lock_num_++;  //共享锁加一
    return true;
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {
    std::unique_lock lock(latch_);
    auto lock_data_id = LockDataId(tab_fd, LockDataType::TABLE);

    auto iter = lock_table_.find(lock_data_id);
    if (iter != lock_table_.end()) {
        for (const auto& req : iter->second.request_queue_) {
            if (req.txn_id_ != txn->get_transaction_id()) {
                throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
            }
        }
    }

    txn->get_lock_set()->insert(lock_data_id);
    LockRequest* request = new LockRequest(txn->get_transaction_id(), LockMode::EXLUCSIVE);
    lock_table_[lock_data_id].request_queue_.push_back(*request);

    return true;
}

/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) { return true; }

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) { return true; }

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {
    auto req_que = lock_table_.find(lock_data_id);
    if (req_que == lock_table_.end()) {
        return false;
    }
    auto req = lock_table_[lock_data_id].request_queue_.begin();
    while (req != lock_table_[lock_data_id].request_queue_.end()) {
        if (txn->get_transaction_id() == req->txn_id_) {
            req = lock_table_[lock_data_id].request_queue_.erase(req);
        } else {
            req++;
        }
    }

    if (lock_table_[lock_data_id].request_queue_.size() == 0) {
        lock_table_.erase(req_que);
    }
    return true;
}