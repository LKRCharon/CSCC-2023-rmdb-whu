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
#include "executor_block_scan.h"
#include "index/ix.h"
#include "record/rm_file_handle.h"
#include "system/sm.h"

// 使用块嵌套优化
class NestedLoopJoinExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> left_;   // 左儿子节点（需要join的表）
    std::unique_ptr<AbstractExecutor> right_;  // 右儿子节点（需要join的表）
    size_t len_;                               // join后获得的每条记录的长度
    std::vector<ColMeta> cols_;                // join后获得的记录的字段

    std::vector<Condition> fed_conds_;  // join条件
    bool is_end_;

    // 块嵌套
    Page *pages_ = new Page[JOIN_BUFFER_SIZE];
    std::unique_ptr<BlockScanner> inner_;
    std::unique_ptr<BlockScanner> outer_;

   public:
    NestedLoopJoinExecutor(std::unique_ptr<AbstractExecutor> left, std::unique_ptr<AbstractExecutor> right,
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

        // TODO

        // FixMe：搞明白到底move还是传地址
        outer_ = std::make_unique<BlockScanner>(std::move(right_), pages_, 1);
        inner_ = std::make_unique<BlockScanner>(std::move(left_), pages_ + 1, JOIN_BUFFER_SIZE - 1);
    }

    ~NestedLoopJoinExecutor() { delete[] pages_; }

    std::string getType() override { return "NestedLoopJoin"; }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta> &cols() const override { return cols_; }

    void beginTuple() override {
        // 如果有个空表，直接end
        outer_->beginTuple();
        if (outer_->isTableEnd()) {
            is_end_ = true;
            return;
        }
        feed_inner();
        inner_->beginTuple();
        while (inner_->isBufferEnd()) {
            nextTuple();
        }
    }

    void nextTuple() override {
        assert(!is_end());

        inner_->nextTuple();
        while (inner_->isBufferEnd() || outer_->isBufferEnd()) {
            // 内表buffer结束
            if (inner_->isBufferEnd()) {
                if (!outer_->isBufferEnd()) {
                    // 内表buffer扫完，外表扫下一个tuple
                    outer_->nextTuple();
                    if (outer_->isBufferEnd()) {
                        continue;
                    }
                    feed_inner();
                    inner_->beginTuple();
                } else {
                    if (!inner_->isTableEnd()) {
                        // 内表没结束，刷新缓冲，外表回到第一个元组
                        outer_->beginTuple();
                        if (outer_->isBufferEnd()) {
                            continue;
                        }
                        feed_inner();
                        inner_->bbmNext();
                        inner_->beginTuple();
                    } else {
                        if (outer_->isTableEnd()) {
                            // 内外标都扫完，end
                            is_end_ = true;
                            return;
                        } else {
                            // 外表没扫完，buffer满了；内表扫完了
                            outer_->bbmNext();
                            outer_->beginTuple();
                            if (outer_->isBufferEnd()) {
                                continue;
                            }
                            feed_inner();
                            // 内表从头开始，buffer从第一个开始读
                            inner_->bbmNext();
                            inner_->beginTuple();
                        }
                    }
                }
            }
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

        auto left_record = inner_->getTuple();
        auto right_record = outer_->getTuple();
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
    void feed_inner() {
        auto left_dict = rec2dict(outer_->cols(), outer_->getTuple().get());
        inner_->feed(left_dict, fed_conds_);
    }

    Rid &rid() override { return _abstract_rid; }
};