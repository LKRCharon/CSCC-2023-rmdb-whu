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
        while (outer_->isBufferEnd()) {
            if (outer_->isBbmEnd()) {
                is_end_ = true;
                return;
            }
            outer_->bbmNext();
            outer_->beginTuple();
        }
        feed_inner();
        inner_->beginTuple();
        if (inner_->isBufferEnd()) {
            nextTuple();
        }
    }

    // 每次调用next都是循环的结束，循环开始前会检查是否end，一般不会有问题
    void nextTuple() override {
        // assert(!is_end());
        while (!(outer_->isBbmEnd() && outer_->isBufferEnd())) {
            while (!(inner_->isBbmEnd() && inner_->isBufferEnd() && outer_->isBufferEnd())) {
                while (!outer_->isBufferEnd()) {
                    while (!inner_->isBufferEnd()) {
                        inner_->nextTuple();
                        if (!inner_->isBufferEnd()) {
                            return;
                        }
                    }
                    outer_->nextTuple();
                    if (!outer_->isBufferEnd()) {
                        feed_inner();
                        inner_->beginTuple();
                        if (!inner_->isBufferEnd()) {
                            return;
                        }
                    }
                }
                //第二层循环，inner表要换下一块，outer回到开头
                if (inner_->isBbmEnd() && inner_->isBufferEnd() && outer_->isBufferEnd()) {
                    // outer的buffer已经扫完所有记录
                    // 如果inner表没有下一块了，退出到第一层循环
                    break;
                }
                outer_->beginTuple();
                while (outer_->isBufferEnd()) {
                    if (outer_->isBbmEnd()) {
                        is_end_ = true;
                        return;
                    }
                    outer_->bbmNext();
                    outer_->beginTuple();
                }
                feed_inner();
                inner_->bbmNext();
                inner_->beginTuple();
                if (!inner_->isBufferEnd()) {
                    return;
                }
                // 如果inner是bufferend，说明outer这一行在这inner的这一块中没有合适记录
                // outer下一行 inner回开头
            }
            // 最外层循环，当inner表扫到表尾，outer表扫到buf尾，outer表换下一块

            // where t2.id<t1.id and t1.id<2
            // t1.id先扫到1，t2直接bufferend，进到此处
            if (outer_->isBbmEnd()) {
                is_end_ = true;
                return;
            }
            outer_->bbmNext();
            outer_->beginTuple();
            while (outer_->isBufferEnd()) {
                if (outer_->isBbmEnd()) {
                    is_end_ = true;
                    return;
                }
                outer_->bbmNext();
                outer_->beginTuple();
            }
            feed_inner();
            inner_->bbmNext();
            inner_->beginTuple();
            if (!inner_->isBufferEnd()) {
                return;
            }
        }
        is_end_ = true;
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