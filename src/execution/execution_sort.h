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
#include <cmath>

#include "common/common.h"
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"

class SortExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_;
    std::vector<ColMeta> cols_;  // 框架中只支持一个键排序，需要自行修改数据结构支持多个键排序
    size_t tuple_num;            // 用于跟踪已处理的元组数量
    std::vector<bool> is_desc_;

    size_t used_tuple;  // 用于跟踪已使用的元组的索引
    // std::vector<size_t> used_tuple;           // 用于跟踪已使用的元组的索引
    std::unique_ptr<RmRecord> current_tuple;  // 用于跟踪当前处理的元组
    // TODO
    std::vector<std::unique_ptr<RmRecord>> tuples;  // 收集待排序的元组数据
    size_t limit_ = 0;
    // std::vector<std::unique_ptr<RmRecord>>::iterator iterator_t;//当前元组指向
    std::unique_ptr<RmRecord> last_record = nullptr;  // 记录上一次返回的 RmRecord
    // DONE ----------------------------------------------------------------------------
   public:
    SortExecutor(std::unique_ptr<AbstractExecutor> prev, std::vector<TabCol> sel_cols, std::vector<bool> is_desc,
                 int limit) {
        prev_ = std::move(prev);
        auto &prev_cols = prev_->cols();
        size_t curr_offset = 0;
        for (auto sel_col : sel_cols) {
            auto pos = get_col(prev_cols, sel_col);
            auto col = *pos;
            cols_.push_back(col);
        }
        is_desc_ = is_desc;
        limit_ = static_cast<size_t>(limit);
    }

    virtual std::string getType() override { return "SortExecutor"; };

    void beginTuple() override {
        tuple_num = 0;
        used_tuple = 0;
        prev_->beginTuple();
        while (!prev_->is_end()) {
            current_tuple = prev_->Next();  // 从前一个执行器中获取下一个元组
            auto val_a = current_tuple->data[cols_[0].offset];
            tuples.push_back(std::move(current_tuple));
            prev_->nextTuple();
            tuple_num++;
        }
        std::sort(tuples.begin(), tuples.end(),
                  [&](const std::unique_ptr<RmRecord> &a, const std::unique_ptr<RmRecord> &b) {
                      return compares(a->data, b->data);
                  });
    }

    void nextTuple() override { used_tuple++; }
    bool is_end() const override {
        if (limit_ == 0) {
            return used_tuple == tuple_num;
        }
        return used_tuple == std::min(tuple_num, limit_);
    }

    std::unique_ptr<RmRecord> Next() override { return std::move(tuples.at(used_tuple)); }
    const std::vector<ColMeta> &cols() const override { return prev_->cols(); };
    Rid &rid() override { return _abstract_rid; }

    int compare(const char *a, const char *b, ColType type, int col_len) {
        switch (type) {
            case TYPE_INT: {
                int ia = *(int *)a;
                int ib = *(int *)b;
                return (ia < ib) ? -1 : ((ia > ib) ? 1 : 0);
            }
            case TYPE_BIGINT: {
                long long ia = *(long long *)a;
                long long ib = *(long long *)b;
                return (ia < ib) ? -1 : ((ia > ib) ? 1 : 0);
            }
            case TYPE_DATETIME: {
                long long ia = *(long long *)a;
                long long ib = *(long long *)b;
                return (ia < ib) ? -1 : ((ia > ib) ? 1 : 0);
            }
            case TYPE_FLOAT: {
                double fa = *(double *)a;
                double fb = *(double *)b;
                return (fa < fb) ? -1 : ((fa > fb) ? 1 : 0);
            }
            case TYPE_STRING:
                return memcmp(a, b, col_len);
            default:
                throw InternalError("Unexpected data type");
        }
    }

    bool compares(const char *a, const char *b) {
        for (size_t i = 0; i < cols_.size(); ++i) {
            int res = compare(a + cols_.at(i).offset, b + cols_.at(i).offset, cols_[i].type, cols_[i].len);
            if (res != 0) {
                return is_desc_.at(i) ? (res > 0) : (res < 0);
            }
        }
        return false;
    }
};