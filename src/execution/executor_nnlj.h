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

// 没有任何优化的loopjoin
class NaiveNestedLoopJoinExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> left_;   // 左儿子节点（需要join的表）
    std::unique_ptr<AbstractExecutor> right_;  // 右儿子节点（需要join的表）
    size_t len_;                               // join后获得的每条记录的长度
    std::vector<ColMeta> cols_;                // join后获得的记录的字段

    std::vector<Condition> fed_conds_;  // join条件

    bool is_end_;

   public:
    NaiveNestedLoopJoinExecutor(std::unique_ptr<AbstractExecutor> left, std::unique_ptr<AbstractExecutor> right,
                                std::vector<Condition> conds) {
        left_ = std::move(left);
        right_ = std::move(right);
        len_ = left_->tupleLen() + right_->tupleLen();
        cols_ = left_->cols();
        auto right_cols = right_->cols();
        for (auto &col : right_cols) {
            col.offset += left_->tupleLen();
        }

        cols_.insert(cols_.end(), right_cols.begin(), right_cols.end());
        is_end_ = false;
        fed_conds_ = std::move(conds);
    }
    std::string getType() override { return "Join"; }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta> &cols() const override { return cols_; }

    void beginTuple() override {
        left_->beginTuple();
        if (left_->is_end()) {
            is_end_ = true;
            return;
        }
        feed_right();
        right_->beginTuple();
        while (right_->is_end()) {
            // lab3 task2 Todo
            // 如果当前innerTable(右表或算子)扫描完了
            // 你需要移动到outerTable(左表)下一个记录,然后把右表移动到第一个记录的位置
            left_->nextTuple();
            if (left_->is_end()) {
                is_end_ = true;
                break;
            }
            feed_right();
            right_->beginTuple();
            // lab3 task2 Todo end
        }
    }

    void nextTuple() override {
        assert(!is_end());
        right_->nextTuple();
        while (right_->is_end()) {
            // lab3 task2 Todo
            // 如果右节点扫描完了
            // 你需要把左表移动到下一个记录并把右节点回退到第一个记录
            left_->nextTuple();
            if (left_->is_end()) {
                is_end_ = true;
                break;
            }
            feed_right();
            right_->beginTuple();
            // lab3 task2 Todo end
        }
    }

    bool is_end() const override { return is_end_; }

    // stupid
    // TODO：optimize to block loop join 火山模型能做吗？
    /**
     * @brief 从左右算子的Next获取记录并拼接成一条新纪录
     * @note 左右算子的conditions已更新
     */
    std::unique_ptr<RmRecord> Next() override {
        assert(!is_end());
        auto record = std::make_unique<RmRecord>(len_);
        // lab3 task2 Todo
        // 调用左右算子的Next()获取下一个记录进行拼接赋给返回的连接结果std::make_unique<RmRecord>record中

        auto left_record = left_->Next();
        auto right_record = right_->Next();
        auto right_start = record->data + left_record->size;
        memcpy(record->data, left_record->data, left_record->size);
        memcpy(right_start, right_record->data, right_record->size);
        // lab3 task2 Todo End
        return record;
    }

    // 默认以right 作inner table
    /**
     * @brief 修改conditons
     * @note left拿到新值后都要做
     */
    void feed_right() {
        auto left_dict = rec2dict(left_->cols(), left_->Next().get());
        right_->feed(left_dict, fed_conds_);
    }

    Rid &rid() override { return _abstract_rid; }
};