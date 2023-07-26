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
#include "parser/ast.h"
#include "limits.h"

class AggregateExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_;  // 投影节点的儿子节点
    ast::AggType aggType_;//聚合类型
    std::string asName_;//聚合出来的数据名称
    std::vector<TabCol> aggCols_;

    long long num;
    long long intRecord;
    double floatRecord;
    std::string charRecord;


   public:
    AggregateExecutor(std::unique_ptr<AbstractExecutor> prev, std::vector<TabCol> aggCols, ast::AggType aggType, std::string asName) {
        prev_ = std::move(prev);
        aggCols_ = std::move(aggCols);
        aggType_ = std::move(aggType);
        asName_ = std::move(asName);

        if (aggType_ != ast::AGGTYPE_COUNT) { //验证一下，如果是MAX,MIN,SUM的话只能有一列
            assert(prev_->cols().size() == 1);//这里假设了聚合的子节点一定是projection
        }
    }

    std::unique_ptr<RmRecord> Next() override {
        if (prev_->cols()[0].type == TYPE_INT) { //int型数据
            long long temp;
            if (aggType_ == ast::AGGTYPE_COUNT) {
                temp = 0;
                prev_->beginTuple();
                while(!prev_->is_end()) {
                    temp++;
                    auto Tuple = prev_->Next();
                    prev_->nextTuple();
                }

                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;

            } else if (aggType_ == ast::AGGTYPE_MAX) {
                bool init_ = true; //true的时候初始化

                prev_->beginTuple();
                while(!prev_->is_end()) {
                    auto &col = prev_->cols()[0];
                    auto Tuple = prev_->Next();
                    char *rec_buf = Tuple->data + col.offset;
                    int data_ = *(int *)rec_buf;
                    if (init_) {
                        temp = (long long)data_;
                        init_ = !init_;
                    } else {
                        temp = std::max(temp, (long long)data_);
                    }
                    prev_->nextTuple();
                }

                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;

            } else if (aggType_ == ast::AGGTYPE_MIN) {
                bool init_ = true; //true的时候初始化

                prev_->beginTuple();
                while(!prev_->is_end()) {
                    auto &col = prev_->cols()[0];
                    auto Tuple = prev_->Next();
                    char *rec_buf = Tuple->data + col.offset;
                    int data_ = *(int *)rec_buf;
                    if (init_) {
                        temp = (long long)data_;
                        init_ = !init_;
                    } else {
                        temp = std::min(temp, (long long)data_);
                    }
                    prev_->nextTuple();
                }
                
                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;
                
            } else if (aggType_ == ast::AGGTYPE_SUM) {
                temp = 0;
                prev_->beginTuple();
                while(!prev_->is_end()) {
                    auto &col = prev_->cols()[0];
                    auto Tuple = prev_->Next();
                    char *rec_buf = Tuple->data + col.offset;
                    int data_ = *(int *)rec_buf;
                    temp += (long long)data_;
                    prev_->nextTuple();
                    }
                
                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;
                
            }
        } else if (prev_->cols()[0].type == TYPE_FLOAT) {
            double temp;
            if (aggType_ == ast::AGGTYPE_COUNT) {
                int temp = 0;
                prev_->beginTuple();
                while(!prev_->is_end()) {
                    temp++;
                    auto Tuple = prev_->Next();
                    prev_->nextTuple();
                }
                
                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;
                
            } else if (aggType_ == ast::AGGTYPE_MAX) {
                bool init_ = true; //true的时候初始化

                prev_->beginTuple();
                while(!prev_->is_end()) {
                    auto &col = prev_->cols()[0];
                    auto Tuple = prev_->Next();
                    char *rec_buf = Tuple->data + col.offset;
                    double data_ = *(double *)rec_buf;
                    if (init_) {
                        temp = data_;
                        init_ = !init_;
                    } else {
                        temp = std::max(temp, data_);
                    }
                    prev_->nextTuple();
                }
                
                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;
                
            } else if (aggType_ == ast::AGGTYPE_MIN) {
                bool init_ = true; //true的时候初始化

                prev_->beginTuple();
                while(!prev_->is_end()) {
                    auto &col = prev_->cols()[0];
                    auto Tuple = prev_->Next();
                    char *rec_buf = Tuple->data + col.offset;
                    double data_ = *(double *)rec_buf;
                    if (init_) {
                        temp = data_;
                        init_ = !init_;
                    } else {
                        temp = std::min(temp, data_);
                    }
                    prev_->nextTuple();
                }
                
                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;
                
            } else if (aggType_ == ast::AGGTYPE_SUM) {
                temp = 0;
                prev_->beginTuple();
                while(!prev_->is_end()) {
                    auto &col = prev_->cols()[0];
                    auto Tuple = prev_->Next();
                    char *rec_buf = Tuple->data + col.offset;
                    double data_ = *(double *)rec_buf;
                    temp += data_;
                    prev_->nextTuple();
                    }
                
                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;
                
            }
        } else if (prev_->cols()[0].type == TYPE_STRING) {
            std::string temp;
            if (aggType_ == ast::AGGTYPE_COUNT) {
                int temp = 0;
                prev_->beginTuple();
                while(!prev_->is_end()) {
                    temp++;
                    prev_->nextTuple();
                    
                }
                
                std::string output = std::to_string(temp);
                auto rec = std::make_unique<RmRecord>(output.length());
                memcpy(rec->data, output.c_str(), output.length());

                return rec;
                
            } else if (aggType_ == ast::AGGTYPE_MAX) {
                bool init_ = true; //true的时候初始化

                prev_->beginTuple();
                while(!prev_->is_end()) {
                    auto &col = prev_->cols()[0];
                    auto Tuple = prev_->Next();
                    char *rec_buf = Tuple->data + col.offset;
                    std::string data_ = std::string((char *)rec_buf, col.len);
                    if (init_) {
                        temp = data_;
                        init_ = !init_;
                    } else {
                        int val = temp.compare(data_);
                        if (val<0) {
                            temp = data_;
                        }
                    }
                    prev_->nextTuple();
                }
                
                auto rec = std::make_unique<RmRecord>(temp.length());
                memcpy(rec->data, temp.c_str(), temp.length());

                return rec;
                
            } else if (aggType_ == ast::AGGTYPE_MIN) {
                bool init_ = true; //true的时候初始化

                prev_->beginTuple();
                while(!prev_->is_end()) {
                    auto &col = prev_->cols()[0];
                    auto Tuple = prev_->Next();
                    char *rec_buf = Tuple->data + col.offset;
                    std::string data_ = std::string((char *)rec_buf, col.len);
                    if (init_) {
                        temp = data_;
                        init_ = !init_;
                    } else {
                        int val = temp.compare(data_);
                        if (val>0) {
                            temp = data_;
                        }
                    }
                    prev_->nextTuple();
                }
                
                auto rec = std::make_unique<RmRecord>(temp.length());
                memcpy(rec->data, temp.c_str(), temp.length());

                return rec;
                
            }
        }
    }

    void beginTuple() override { prev_->beginTuple(); };
    
    void nextTuple() override {
        assert(!prev_->is_end());
        prev_->nextTuple();
    }

    bool is_end() const override { return true; } 

    Rid &rid() override { return _abstract_rid; }

    
};