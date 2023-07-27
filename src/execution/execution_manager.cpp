/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "execution_manager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "common/config.h"
#include "executor_aggregate.h"
#include "executor_delete.h"
#include "executor_index_scan.h"
#include "executor_insert.h"
#include "executor_nestedloop_join.h"
#include "executor_projection.h"
#include "executor_seq_scan.h"
#include "executor_update.h"
#include "index/ix.h"
#include "record_printer.h"

const char *help_info =
    "Supported SQL syntax:\n"
    "  command ;\n"
    "command:\n"
    "  CREATE TABLE table_name (column_name type [, column_name type ...])\n"
    "  DROP TABLE table_name\n"
    "  CREATE INDEX table_name (column_name)\n"
    "  DROP INDEX table_name (column_name)\n"
    "  INSERT INTO table_name VALUES (value [, value ...])\n"
    "  DELETE FROM table_name [WHERE where_clause]\n"
    "  UPDATE table_name SET column_name = value [, column_name = value ...] [WHERE where_clause]\n"
    "  SELECT selector FROM table_name [WHERE where_clause]\n"
    "type:\n"
    "  {INT | FLOAT | CHAR(n)}\n"
    "where_clause:\n"
    "  condition [AND condition ...]\n"
    "condition:\n"
    "  column op {column | value}\n"
    "column:\n"
    "  [table_name.]column_name\n"
    "op:\n"
    "  {= | <> | < | > | <= | >=}\n"
    "selector:\n"
    "  {* | column [, column ...]}\n";

// 主要负责执行DDL语句
void QlManager::run_mutli_query(std::shared_ptr<Plan> plan, Context *context) {
    if (auto x = std::dynamic_pointer_cast<DDLPlan>(plan)) {
        switch (x->tag) {
            case T_CreateTable: {
                sm_manager_->create_table(x->tab_name_, x->cols_, context);
                break;
            }
            case T_DropTable: {
                sm_manager_->drop_table(x->tab_name_, context);
                break;
            }
            case T_CreateIndex: {
                sm_manager_->create_index(x->tab_name_, x->tab_col_names_, context);
                break;
            }
            case T_DropIndex: {
                sm_manager_->drop_index(x->tab_name_, x->tab_col_names_, context);
                break;
            }
            default:
                throw InternalError("Unexpected field type");
                break;
        }
    }
}

// 执行help; show tables; desc table; begin; commit; abort;语句
void QlManager::run_cmd_utility(std::shared_ptr<Plan> plan, txn_id_t *txn_id, Context *context) {
    if (auto x = std::dynamic_pointer_cast<OtherPlan>(plan)) {
        switch (x->tag) {
            case T_Help: {
                memcpy(context->data_send_ + *(context->offset_), help_info, strlen(help_info));
                *(context->offset_) = strlen(help_info);
                break;
            }
            case T_ShowTable: {
                sm_manager_->show_tables(context);
                break;
            }
            case T_ShowIndex: {
                sm_manager_->show_index(x->tab_name_, context);
                break;
            }
            case T_DescTable: {
                sm_manager_->desc_table(x->tab_name_, context);
                break;
            }
            case T_Transaction_begin: {
                // 显示开启一个事务
                context->txn_->set_txn_mode(true);
                break;
            }
            case T_Transaction_commit: {
                context->txn_ = txn_mgr_->get_transaction(*txn_id);
                txn_mgr_->commit(context->txn_, context->log_mgr_);
                break;
            }
            case T_Transaction_rollback: {
                context->txn_ = txn_mgr_->get_transaction(*txn_id);
                txn_mgr_->abort(context->txn_, context->log_mgr_);
                break;
            }
            case T_Transaction_abort: {
                context->txn_ = txn_mgr_->get_transaction(*txn_id);
                txn_mgr_->abort(context->txn_, context->log_mgr_);
                break;
            }
            default:
                throw InternalError("Unexpected field type");
                break;
        }
    }
}

void QlManager::run_load_data(std::shared_ptr<Plan> plan, Context *context) {
    if (auto x = std::dynamic_pointer_cast<LoadPlan>(plan)) {
        // 获取数据库的表
        TabMeta &tab_ = sm_manager_->db_.get_table(x->tab_name_);
        RmFileHandle *fh_ = sm_manager_->fhs_.at(x->tab_name_).get();
        Context *context_ = context;
        Rid rid_;
        // 不知道需不需要加锁也不知道要不要加锁，先注释掉得了
        //  context_->lock_mgr_->lock_IX_on_table(context_->txn_, fh_->GetFd());

        std::vector<ColType> type_list;
        std::vector<int> len_list;
        std::vector<int> offset_list;

        // 打开csv文件
        std::ifstream inFile(x->file_name_);

        std::string lineStr;

        int is_header = 1;

        while (getline(inFile, lineStr)) {
            if (!lineStr.empty() && lineStr.back() == '\r') {
                lineStr.pop_back();
            }
            std::stringstream ss(lineStr);
            std::string str;
            std::vector<std::string> lineArray;
            while (getline(ss, str, ',')) {
                lineArray.push_back(str);
            }

            if (is_header) {  // 头部进行判断
                is_header = 0;

                if (lineArray.size() != tab_.cols.size()) {
                    throw LoadNotMatchError();
                }

                for (size_t i = 0; i < tab_.cols.size(); i++) {
                    type_list.push_back(tab_.cols[i].type);
                    len_list.push_back(tab_.cols[i].len);
                    offset_list.push_back(tab_.cols[i].offset);

                    if (lineArray[i] != tab_.cols[i].name) {
                        throw LoadNotMatchError();
                    }
                }
                continue;
            } else {
                RmRecord rec(fh_->get_file_hdr().record_size);
                for (size_t i = 0; i < tab_.cols.size(); i++) {
                    Value value;
                    if (type_list[i] == TYPE_INT) {
                        int data = std::stoi(lineArray[i]);
                        value.set_int(data);
                    } else if (type_list[i] == TYPE_BIGINT) {
                        long long data = std::stoll(lineArray[i]);
                        value.set_int(data);
                    } else if (type_list[i] == TYPE_FLOAT) {
                        double data = std::stod(lineArray[i]);
                        value.set_float(data);
                    } else if (type_list[i] == TYPE_DATETIME) {
                        long long data = DatetimeStrToLL(lineArray[i]);
                        value.set_datetime(data);
                    } else {
                        // if (lineArray[i].length() > len_list[i]) {
                        //     value.set_str(lineArray[i].substr(0, len_list[i]));
                        // } else {
                        //     value.set_str(lineArray[i]);
                        // }
                        value.set_str(lineArray[i]);
                    }
                    value.init_raw(len_list[i]);
                    memcpy(rec.data + offset_list[i], value.raw->data, len_list[i]);
                }
                rid_ = fh_->insert_record(rec.data, context_);

                // 索引，从insert复制过来的
                for (size_t i = 0; i < tab_.indexes.size(); ++i) {
                    auto &index = tab_.indexes[i];
                    auto ih =
                        sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(x->tab_name_, index.cols))
                            .get();
                    char *key = new char[index.col_total_len];
                    int offset = 0;
                    for (int j = 0; j < index.col_num; j++) {
                        memcpy(key + offset, rec.data + index.cols[j].offset, index.cols[j].len);
                        offset += index.cols[j].len;
                    }
                    bool is_insert = ih->insert_entry(key, rid_, context_->txn_);
                    if (!is_insert) {
                        fh_->delete_record(rid_, context_);
                        throw IndexEntryRepeatError();
                    }
                    delete[] key;
                }
            }
        }
    }
}

// 执行select语句，select语句的输出除了需要返回客户端外，还需要写入output.txt文件中
void QlManager::select_from(std::unique_ptr<AbstractExecutor> executorTreeRoot, std::vector<TabCol> sel_cols,
                            Context *context) {
    std::vector<std::string> captions;
    captions.reserve(sel_cols.size());
    for (auto &sel_col : sel_cols) {
        captions.push_back(sel_col.col_name);
    }

    // Print header into buffer
    RecordPrinter rec_printer(sel_cols.size());
    rec_printer.print_separator(context);
    rec_printer.print_record(captions, context);
    rec_printer.print_separator(context);
    // print header into file
    std::fstream outfile;
    if (is_need_output) {
        outfile.open("output.txt", std::ios::out | std::ios::app);
        outfile << "|";
        for (int i = 0; i < captions.size(); ++i) {
            outfile << " " << captions[i] << " |";
        }
        outfile << "\n";
    }

    // Print records
    size_t num_rec = 0;
    // 执行query_plan
    executorTreeRoot->beginTuple();
    while (!executorTreeRoot->is_end()) {
        auto Tuple = executorTreeRoot->Next();
        std::vector<std::string> columns;
        for (auto &col : executorTreeRoot->cols()) {
            std::string col_str;
            char *rec_buf = Tuple->data + col.offset;
            if (col.type == TYPE_INT) {
                col_str = std::to_string(*(int *)rec_buf);
            } else if (col.type == TYPE_BIGINT) {
                col_str = std::to_string(*(long long *)rec_buf);
            } else if (col.type == TYPE_DATETIME) {
                col_str = DatetimeLLtoStr(*(long long *)rec_buf);
            } else if (col.type == TYPE_FLOAT) {
                col_str = std::to_string(*(double *)rec_buf);
            } else if (col.type == TYPE_STRING) {
                col_str = std::string((char *)rec_buf, col.len);
                col_str.resize(strlen(col_str.c_str()));
            }
            columns.push_back(col_str);
        }
        // print record into buffer
        rec_printer.print_record(columns, context);
        if (is_need_output) {
            // print record into file
            outfile << "|";
            for (int i = 0; i < columns.size(); ++i) {
                outfile << " " << columns[i] << " |";
            }
            outfile << "\n";
        }
        num_rec++;
        executorTreeRoot->nextTuple();
    }
    if (is_need_output) {
        outfile.close();
    }
    // Print footer into buffer
    rec_printer.print_separator(context);
    // Print record count into buffer
    RecordPrinter::print_record_count(num_rec, context);
}

void QlManager::agg_select_from(std::unique_ptr<AbstractExecutor> executorTreeRoot, std::vector<TabCol> sel_cols,
                                Context *context) {
    std::vector<std::string> captions;
    captions.reserve(sel_cols.size());
    for (auto &sel_col : sel_cols) {
        captions.push_back(sel_col.col_name);
    }

    // Print header into buffer
    RecordPrinter rec_printer(sel_cols.size());
    rec_printer.print_separator(context);
    rec_printer.print_record(captions, context);
    rec_printer.print_separator(context);
    // print header into file
    std::fstream outfile;
    if (is_need_output) {
        outfile.open("output.txt", std::ios::out | std::ios::app);
        outfile << "|";
        for (int i = 0; i < captions.size(); ++i) {
            outfile << " " << captions[i] << " |";
        }
        outfile << "\n";
    }

    // Print records
    size_t num_rec = 0;
    // 执行query_plan

    auto Tuple = executorTreeRoot->Next();
    std::vector<std::string> columns;

    std::string col_str;
    char *rec_buf = Tuple->data;

    col_str = std::string((char *)rec_buf, Tuple->size);
    col_str.resize(strlen(col_str.c_str()));

    columns.push_back(col_str);

    // print record into buffer
    rec_printer.print_record(columns, context);
    num_rec++;
    if (is_need_output) {
        // print record into file
        outfile << "|";
        for (int i = 0; i < columns.size(); ++i) {
            outfile << " " << columns[i] << " |";
        }
        outfile << "\n";
        outfile.close();
    }
    // Print footer into buffer
    rec_printer.print_separator(context);
    // Print record count into buffer
    RecordPrinter::print_record_count(num_rec, context);
}

// 执行DML语句

void QlManager::run_dml(std::unique_ptr<AbstractExecutor> exec) { exec->Next(); }