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
#include <algorithm>

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

    // 3. 查找当前事务是否已经申请了目标数据项上的锁，如果存在则根据锁类型进行操作，否则执行下一步操作
    auto lock_data_id = LockDataId(tab_fd, rid, LockDataType::RECORD);
    auto iter = lock_table_.find(lock_data_id);

    if (iter != lock_table_.end()) {
        auto& request_q = iter->second.request_queue_;
        for (const auto& req : request_q) {
            if (req.txn_id_ == txn->get_transaction_id()) {
                // 自己请求过锁，无论S锁还是X锁都可认为成功
                return true;
            } else {
                if (req.lock_mode_ == LockMode::EXLUCSIVE) {
                    throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
                }
            }
        }
    }
    // 4. 请求队列里没有本事务的锁，将要申请的锁放入到全局锁表中，并通过组模式来判断是否可以成功授予锁
    // 构造req
    LockRequest request(txn->get_transaction_id(), LockMode::SHARED);
    auto& request_q = lock_table_[lock_data_id];
    // 5. 如果成功，更新目标数据项在全局锁表中的信息，否则abort

    txn->get_lock_set()->insert(lock_data_id);

    request.granted_ = true;
    request_q.shared_lock_num_++;  //共享锁加一
    request_q.request_queue_.emplace_back(request);

    return true;
}

/**
 * @description: 申请行级排他锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID
 * @param {int} tab_fd 记录所在的表的fd
 */
bool LockManager::lock_exclusive_on_record(Transaction* txn, const Rid& rid, int tab_fd) {
    // 1. 通过mutex申请访问全局锁表
    std::unique_lock lock(latch_);
    // 2. 检查事务的状态

    // 3. 查找当前事务是否已经申请了目标数据项上的锁，如果存在则根据锁类型进行操作，否则执行下一步操作
    auto lock_data_id = LockDataId(tab_fd, rid, LockDataType::RECORD);
    auto iter = lock_table_.find(lock_data_id);

    if (iter != lock_table_.end()) {
        auto& request_q = iter->second;
        for (auto& req : request_q.request_queue_) {
            if (req.txn_id_ == txn->get_transaction_id()) {
                // 自己有X锁
                if (req.lock_mode_ == LockMode::EXLUCSIVE) {
                    return true;
                } else {
                    // 本事务S锁，且只有这一把锁才能升级
                    if (request_q.request_queue_.size() == 1) {
                        req.lock_mode_ = LockMode::EXLUCSIVE;
                    }
                }
            } else {
                // 别的事务有锁，abort
                throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
            }
        }
    }

    LockRequest request(txn->get_transaction_id(), LockMode::EXLUCSIVE);
    auto& request_q = lock_table_[lock_data_id];

    request.granted_ = true;
    request_q.request_queue_.emplace_back(request);

    txn->get_lock_set()->insert(lock_data_id);

    return true;
}

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
    // 3. 查找当前事务是否已经申请了目标数据项上的锁，如果存在则根据锁类型进行操作，否则执行下一步操作
    if (iter != lock_table_.end()) {
        auto& request_q = iter->second;
        for (const auto& req : request_q.request_queue_) {
            if (req.txn_id_ == txn->get_transaction_id()) {
                return true;
            }
            // 别的事务加了X，IX，SIX三种，Abort
            if (req.lock_mode_ == LockMode::EXLUCSIVE || req.lock_mode_ == LockMode::INTENTION_EXCLUSIVE ||
                req.lock_mode_ == LockMode::S_IX) {
                throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
            }
        }
    }

    LockRequest request(txn->get_transaction_id(), LockMode::SHARED);
    request.granted_ = true;
    auto& request_q = lock_table_[lock_data_id];
    txn->get_lock_set()->insert(lock_data_id);
    request_q.request_queue_.emplace_back(request);
    request_q.shared_lock_num_++;  //共享锁加一
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
        auto& request_q = iter->second;

        for (auto& req : request_q.request_queue_) {
            if (req.txn_id_ == txn->get_transaction_id()) {
                // 自己有X锁
                if (req.lock_mode_ == LockMode::EXLUCSIVE) {
                    return true;
                } else {
                    // 本事务非X锁，且只有这一把锁才能升级
                    if (request_q.request_queue_.size() == 1) {
                        req.lock_mode_ = LockMode::EXLUCSIVE;
                    }
                }
            } else {
                // 别的事务有锁，abort
                throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
            }
        }
    }

    txn->get_lock_set()->insert(lock_data_id);
    LockRequest request(txn->get_transaction_id(), LockMode::EXLUCSIVE);
    request.granted_ = true;
    lock_table_[lock_data_id].request_queue_.emplace_back(request);

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
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {
    std::unique_lock lock(latch_);

    auto lock_data_id = LockDataId(tab_fd, LockDataType::TABLE);
    auto iter = lock_table_.find(lock_data_id);
    // 3. 查找当前事务是否已经申请了目标数据项上的锁，如果存在则根据锁类型进行操作，否则执行下一步操作
    if (iter != lock_table_.end()) {
        auto& request_q = iter->second;
        for (auto& req : request_q.request_queue_) {
            if (req.txn_id_ == txn->get_transaction_id()) {
                //  该事务已经持有IX或者更高级的锁
                if (req.lock_mode_ == LockMode::INTENTION_EXCLUSIVE || req.lock_mode_ == LockMode::S_IX ||
                    req.lock_mode_ == LockMode::EXLUCSIVE) {
                    return true;
                }
                // 有S锁，升级成SIX，不能有别的事务持有S锁
                if (req.lock_mode_ == LockMode::SHARED) {
                    auto S_count =
                        std::count_if(request_q.request_queue_.begin(), request_q.request_queue_.end(),
                                      [](const LockRequest req) { return req.lock_mode_ == LockMode::SHARED; });
                    if (S_count > 1) {
                        throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
                    }
                    req.lock_mode_ = LockMode::S_IX;
                    return true;
                }
                // TODO:考虑IS
            }

            if (req.lock_mode_ == LockMode::EXLUCSIVE || req.lock_mode_ == LockMode::SHARED ||
                req.lock_mode_ == LockMode::S_IX) {
                throw TransactionAbortException(txn->get_transaction_id(), AbortReason::DEADLOCK_PREVENTION);
            }
        }
    }
    // 既没有升级锁，也没有因为死锁预防abort，可以正常加锁
    LockRequest request(txn->get_transaction_id(), LockMode::INTENTION_EXCLUSIVE);
    request.granted_ = true;
    auto& request_q = lock_table_[lock_data_id];
    txn->get_lock_set()->insert(lock_data_id);
    request_q.request_queue_.emplace_back(request);

    return true;
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {
    std::unique_lock lock(latch_);

    auto req_q = lock_table_.find(lock_data_id);
    if (req_q == lock_table_.end()) {
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
        lock_table_.erase(req_q);
    }
    return true;
}