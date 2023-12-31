/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "sm_manager.h"

#include <sys/stat.h>
#include <unistd.h>

#include <fstream>

#include "index/ix.h"
#include "record/rm.h"
#include "record_printer.h"

/**
 * @description: 判断是否为一个文件夹
 * @return {bool} 返回是否为一个文件夹
 * @param {string&} db_name 数据库文件名称，与文件夹同名
 */
bool SmManager::is_dir(const std::string& db_name) {
    struct stat st;
    return stat(db_name.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

/**
 * @description: 创建数据库，所有的数据库相关文件都放在数据库同名文件夹下
 * @param {string&} db_name 数据库名称
 */
void SmManager::create_db(const std::string& db_name) {
    if (is_dir(db_name)) {
        throw DatabaseExistsError(db_name);
    }
    // 为数据库创建一个子目录
    std::string cmd = "mkdir " + db_name;
    if (system(cmd.c_str()) < 0) {  // 创建一个名为db_name的目录
        throw UnixError();
    }
    if (chdir(db_name.c_str()) < 0) {  // 进入名为db_name的目录
        throw UnixError();
    }
    // 创建系统目录
    DbMeta* new_db = new DbMeta();
    new_db->name_ = db_name;

    // 注意，此处ofstream会在当前目录创建(如果没有此文件先创建)和打开一个名为DB_META_NAME的文件
    std::ofstream ofs(DB_META_NAME);

    // 将new_db中的信息，按照定义好的operator<<操作符，写入到ofs打开的DB_META_NAME文件中
    ofs << *new_db;  // 注意：此处重载了操作符<<

    delete new_db;

    // 创建日志文件
    disk_manager_->create_file(LOG_FILE_NAME);

    // 回到根目录
    if (chdir("..") < 0) {
        throw UnixError();
    }
}

/**
 * @description: 删除数据库，同时需要清空相关文件以及数据库同名文件夹
 * @param {string&} db_name 数据库名称，与文件夹同名
 */
void SmManager::drop_db(const std::string& db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    std::string cmd = "rm -r " + db_name;
    if (system(cmd.c_str()) < 0) {
        throw UnixError();
    }
}

/**
 * @description: 打开数据库，找到数据库对应的文件夹，并加载数据库元数据和相关文件
 * @param {string&} db_name 数据库名称，与文件夹同名
 */
void SmManager::open_db(const std::string& db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    // cd to database dir
    if (chdir(db_name.c_str()) < 0) {
        throw UnixError();
    }
    // Load meta
    // 打开一个名为DB_META_NAME的文件
    std::ifstream ifs(DB_META_NAME);
    // 将ofs打开的DB_META_NAME文件中的信息，按照定义好的operator>>操作符，读出到db_中
    ifs >> db_;  // 注意：此处重载了操作符>>
    // Open all record files & index files
    for (auto& entry : db_.tabs_) {
        auto& tab = entry.second;
        // fhs_[tab.name] = rm_manager_->open_file(tab.name);
        fhs_.emplace(tab.name, rm_manager_->open_file(tab.name));
        for (size_t i = 0; i < tab.cols.size(); i++) {
            auto& col = tab.cols[i];
        }
        for (auto& index : tab.indexes) {
            auto index_handle = ix_manager_->open_index(tab.name, index.cols);
            auto index_name = ix_manager_->get_index_name(tab.name, index.cols);
            ihs_.emplace(index_name, std::move(index_handle));
        }
    }
}

/**
 * @description: 把数据库相关的元数据刷入磁盘中
 */
void SmManager::flush_meta() {
    // 默认清空文件
    std::ofstream ofs(DB_META_NAME);
    ofs << db_;
}

/**
 * @description: 关闭数据库并把数据落盘
 */
void SmManager::close_db() {
    // 刷盘子
    flush_meta();
    // 清理db_
    db_.name_.clear();
    db_.tabs_.clear();
    // 关闭rm_manager_ ix_manager_文件
    for (auto& entry : fhs_) {
        rm_manager_->close_file(entry.second.get());
    }
    for (auto& entry : ihs_) {
        ix_manager_->close_index(entry.second.get());
    }
    // 清理fhs_, ihs_
    fhs_.clear();
    ihs_.clear();

    // 回到根目录
    if (chdir("..") < 0) {
        throw UnixError();
    }
}

/**
 * @description: 显示所有的表,通过测试需要将其结果写入到output.txt,详情看题目文档
 * @param {Context*} context
 */
void SmManager::show_tables(Context* context) {
    std::fstream outfile;
    if (is_need_output) {
        outfile.open("output.txt", std::ios::out | std::ios::app);
        outfile << "| Tables |\n";
        RecordPrinter printer(1);
        printer.print_separator(context);
        printer.print_record({"Tables"}, context);
        printer.print_separator(context);
        for (auto& entry : db_.tabs_) {
            auto& tab = entry.second;
            printer.print_record({tab.name}, context);
            outfile << "| " << tab.name << " |\n";
        }
        printer.print_separator(context);
        outfile.close();
    }
}

/**
 * @description: 显示表的元数据
 * @param {string&} tab_name 表名称
 * @param {Context*} context
 */
void SmManager::desc_table(const std::string& tab_name, Context* context) {
    TabMeta& tab = db_.get_table(tab_name);

    std::vector<std::string> captions = {"Field", "Type", "Index"};
    RecordPrinter printer(captions.size());
    // Print header
    printer.print_separator(context);
    printer.print_record(captions, context);
    printer.print_separator(context);
    // Print fields
    for (auto& col : tab.cols) {
        std::vector<std::string> field_info = {col.name, coltype2str(col.type), col.index ? "YES" : "NO"};
        printer.print_record(field_info, context);
    }
    // Print footer
    printer.print_separator(context);
}

/**
 * @description: 创建表
 * @param {string&} tab_name 表的名称
 * @param {vector<ColDef>&} col_defs 表的字段
 * @param {Context*} context
 */
void SmManager::create_table(const std::string& tab_name, const std::vector<ColDef>& col_defs, Context* context) {
    if (db_.is_table(tab_name)) {
        throw TableExistsError(tab_name);
    }
    // Create table meta
    int curr_offset = 0;
    TabMeta tab;
    tab.name = tab_name;
    for (auto& col_def : col_defs) {
        ColMeta col = {.tab_name = tab_name,
                       .name = col_def.name,
                       .type = col_def.type,
                       .len = col_def.len,
                       .offset = curr_offset,
                       .index = false};
        curr_offset += col_def.len;
        tab.cols.push_back(col);
    }
    // Create & open record file
    int record_size = curr_offset;  // record_size就是col meta所占的大小（表的元数据也是以记录的形式进行存储的）
    rm_manager_->create_file(tab_name, record_size);
    db_.tabs_[tab_name] = tab;
    // fhs_[tab_name] = rm_manager_->open_file(tab_name);
    fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));
    context->lock_mgr_->lock_exclusive_on_table(context->txn_, fhs_.at(tab_name)->GetFd());
    flush_meta();
}

/**
 * @description: 删除表
 * @param {string&} tab_name 表的名称
 * @param {Context*} context
 */
void SmManager::drop_table(const std::string& tab_name, Context* context) {
    if (!db_.is_table(tab_name)) {
        throw TableNotFoundError(tab_name);
    }
    context->lock_mgr_->lock_exclusive_on_table(context->txn_, fhs_.at(tab_name)->GetFd());
    // Close & destroy record file
    auto file_handle_iter = fhs_.find(tab_name);
    // 应该是不需要判断 只要没抛出异常 fhs里一定有tabname
    if (file_handle_iter != fhs_.end()) {
        rm_manager_->close_file(file_handle_iter->second.get());
        fhs_.erase(tab_name);
    }
    TabMeta& tab = db_.get_table(tab_name);
    for (auto& index : tab.indexes) {
        drop_index(tab_name, index.cols, context);
    }
    rm_manager_->destroy_file(tab_name);
    db_.tabs_.erase(tab_name);

    flush_meta();
}

/**
 * @description: 创建索引
 * @param {string&} tab_name 表的名称
 * @param {vector<string>&} col_names 索引包含的字段名称
 * @param {Context*} context
 */
void SmManager::create_index(const std::string& tab_name, const std::vector<std::string>& col_names, Context* context) {
    if (!db_.is_table(tab_name)) {
        throw TableNotFoundError(tab_name);
    }
    context->lock_mgr_->lock_shared_on_table(context->txn_, fhs_.at(tab_name)->GetFd());

    TabMeta& tab_meta = db_.get_table(tab_name);
    if (tab_meta.is_index(col_names)) {
        throw IndexExistsError(tab_name, col_names);
    }
    // void create_index(const std::string &filename, const std::vector<ColMeta> &index_cols)
    std::vector<ColMeta> index_cols;
    int total_len = 0;
    for (const auto& col_name : col_names) {
        auto col = tab_meta.get_col(col_name);
        total_len += col->len;
        index_cols.push_back(*col);
    }
    IndexMeta index_meta = {.tab_name = tab_name};
    index_meta.col_num = int(col_names.size());
    index_meta.col_total_len = total_len;
    index_meta.cols = index_cols;
    tab_meta.indexes.push_back(index_meta);

    ix_manager_->create_index(tab_name, index_cols);

    auto index_handle = ix_manager_->open_index(tab_name, index_cols);
    auto file_handle = fhs_.at(tab_name).get();
    for (RmScan scanner(file_handle); !scanner.is_end(); scanner.next()) {
        auto rec = file_handle->get_record(scanner.rid(), context);
        // 以下逻辑参考自
        char* key = new char[total_len];
        int offset = 0;
        for (auto col : index_cols) {
            memcpy(key + offset, rec->data + col.offset, col.len);
            offset += col.len;
        }
        index_handle->insert_entry(key, scanner.rid(), context->txn_);
        delete[] key;
    }

    auto index_name = ix_manager_->get_index_name(tab_name, index_cols);
    assert(ihs_.count(index_name) == 0);
    // ix_manager_->close_index(index_handle.get());
    ihs_.emplace(index_name, std::move(index_handle));

    flush_meta();
}

/**
 * @description: 删除索引
 * @param {string&} tab_name 表名称
 * @param {vector<string>&} col_names 索引包含的字段名称
 * @param {Context*} context
 */
void SmManager::drop_index(const std::string& tab_name, const std::vector<std::string>& col_names, Context* context) {
    if (!db_.is_table(tab_name)) {
        throw TableNotFoundError(tab_name);
    }
    context->lock_mgr_->lock_shared_on_table(context->txn_, fhs_.at(tab_name)->GetFd());

    TabMeta& tab_meta = db_.get_table(tab_name);
    if (!tab_meta.is_index(col_names)) {
        throw IndexNotFoundError(tab_name, col_names);
    }
    auto index_meta_iter = tab_meta.get_index_meta(col_names);
    auto index_name = ix_manager_->get_index_name(tab_name, index_meta_iter->cols);
    auto index_handle = ix_manager_->open_index(tab_name, index_meta_iter->cols);

    ix_manager_->destroy_index(tab_name, index_meta_iter->cols, index_handle->get_fd());
    ihs_.erase(index_name);
    tab_meta.indexes.erase(index_meta_iter);

    flush_meta();
}

/**
 * @description: 删除索引 drop table 时调用
 * @param {string&} tab_name 表名称
 * @param {vector<ColMeta>&} 索引包含的字段元数据
 * @param {Context*} context
 */
void SmManager::drop_index(const std::string& tab_name, const std::vector<ColMeta>& cols, Context* context) {
    auto index_name = ix_manager_->get_index_name(tab_name, cols);
    ihs_.erase(index_name);
    auto index_handle = ix_manager_->open_index(tab_name, cols);
    ix_manager_->destroy_index(tab_name, cols, index_handle->get_fd());
}

/**
 * @description: 显示索引
 * @param {string&} tab_name 表名称
 * @param {Context*} context
 */
void SmManager::show_index(const std::string& tab_name, Context* context) {
    TabMeta& tab_meta = db_.get_table(tab_name);
    if (is_need_output) {
        std::fstream outfile;
        outfile.open("output.txt", std::ios::out | std::ios::app);
        RecordPrinter printer(3);
        printer.print_separator(context);
        for (auto& entry : tab_meta.indexes) {
            std::string all_columns = entry.GetAllColumnsString();
            std::vector<std::string> index_info = {entry.tab_name, "unique", all_columns};
            printer.print_record(index_info, context);
            outfile << "| " << entry.tab_name << " | "
                    << "unique"
                    << " | " << all_columns << " |\n";
        }
        printer.print_separator(context);
        outfile.close();
    }
}

void SmManager::rollback_insert(const std::string& tab_name, const Rid& rid, Context* context) {
    auto rec = fhs_.at(tab_name)->get_record(rid, context);
    TabMeta& tab = db_.get_table(tab_name);
    // TODO：考虑删索引
    // insert entry

    for (size_t i = 0; i < tab.indexes.size(); ++i) {
        auto& index = tab.indexes.at(i);
        auto ih = ihs_.at(get_ix_manager()->get_index_name(tab_name, index.cols)).get();
        char* key = new char[index.col_total_len];
        int offset = 0;
        for (int j = 0; j < index.col_num; j++) {
            memcpy(key + offset, rec->data + index.cols[j].offset, index.cols[j].len);
            offset += index.cols[j].len;
        }
        ih->delete_entry(key, context->txn_);
        delete[] key;
    }
    Rid tmp = rid;
    auto log_rec = new DeleteLogRecord(context->txn_->get_transaction_id(), *rec, tmp, tab_name);
    log_rec->prev_lsn_ = context->txn_->get_prev_lsn();
    context->txn_->set_prev_lsn(context->log_mgr_->add_log_to_buffer(log_rec));
    delete log_rec;

    // 删记录
    fhs_.at(tab_name)->delete_record(rid, context);
}
// 旧版，不根据rid插入 暂时废弃
void SmManager::rollback_delete(const std::string& tab_name, const RmRecord& record, Context* context) {
    auto rid = fhs_.at(tab_name)->insert_record(record.data, context);
    TabMeta& tab = db_.get_table(tab_name);
    // log
    auto log_rec = new InsertLogRecord(context->txn_->get_transaction_id(), const_cast<RmRecord&>(record),
                                       const_cast<Rid&>(rid), tab_name);
    log_rec->prev_lsn_ = context->txn_->get_prev_lsn();
    context->txn_->set_prev_lsn(context->log_mgr_->add_log_to_buffer(log_rec));
    delete log_rec;

    for (size_t i = 0; i < tab.indexes.size(); ++i) {
        auto& index = tab.indexes.at(i);
        auto ih = ihs_.at(get_ix_manager()->get_index_name(tab_name, index.cols)).get();
        char* key = new char[index.col_total_len];
        int offset = 0;
        for (int j = 0; j < index.col_num; j++) {
            memcpy(key + offset, record.data + index.cols[j].offset, index.cols[j].len);
            offset += index.cols[j].len;
        }
        ih->insert_entry(key, rid, context->txn_);
        delete[] key;
    }
}

// 新逻辑，回滚时根据rid插入
void SmManager::rollback_delete(const std::string& tab_name, const RmRecord& record, Rid& rid, Context* context) {
    TabMeta& tab = db_.get_table(tab_name);
    // log
    auto log_rec =
        new InsertLogRecord(context->txn_->get_transaction_id(), const_cast<RmRecord&>(record), rid, tab_name, true);
    log_rec->prev_lsn_ = context->txn_->get_prev_lsn();
    context->txn_->set_prev_lsn(context->log_mgr_->add_log_to_buffer(log_rec));
    delete log_rec;
    // 按照rid插入值
    fhs_.at(tab_name)->insert_record(rid, record.data);

    // 回滚，插入索引
    for (size_t i = 0; i < tab.indexes.size(); ++i) {
        auto& index = tab.indexes.at(i);
        auto ih = ihs_.at(get_ix_manager()->get_index_name(tab_name, index.cols)).get();
        char* key = new char[index.col_total_len];
        int offset = 0;
        for (int j = 0; j < index.col_num; j++) {
            memcpy(key + offset, record.data + index.cols[j].offset, index.cols[j].len);
            offset += index.cols[j].len;
        }
        ih->insert_entry(key, rid, context->txn_);
        delete[] key;
    }
}

void SmManager::rollback_update(const std::string& tab_name, const Rid& rid, const RmRecord& record, Context* context) {
    // 1. 更新完的new rec，需要被回滚删掉,在反update记录之前拿
    auto new_rec = fhs_.at(tab_name)->get_record(rid, context);
    // abort log

    auto log_rec = new UpdateLogRecord(context->txn_->get_transaction_id(), *new_rec, const_cast<RmRecord&>(record),
                                       const_cast<Rid&>(rid), tab_name);
    log_rec->prev_lsn_ = context->txn_->get_prev_lsn();
    context->txn_->set_prev_lsn(context->log_mgr_->add_log_to_buffer(log_rec));
    delete log_rec;

    fhs_.at(tab_name)->update_record(rid, record.data, context);
    TabMeta& tab = db_.get_table(tab_name);
    // 把索引再改回去
    for (size_t i = 0; i < tab.indexes.size(); ++i) {
        auto& index = tab.indexes.at(i);
        auto ih = ihs_.at(get_ix_manager()->get_index_name(tab_name, index.cols)).get();
        char* insert_key = new char[index.col_total_len];
        char* delete_key = new char[index.col_total_len];
        int offset = 0;
        for (int j = 0; j < index.col_num; j++) {
            memcpy(insert_key + offset, record.data + index.cols[j].offset, index.cols[j].len);
            memcpy(delete_key + offset, new_rec->data + index.cols[j].offset, index.cols[j].len);
            offset += index.cols[j].len;
        }
        ih->delete_entry(delete_key, context->txn_);
        bool is_insert = ih->insert_entry(insert_key, rid, context->txn_);
        if (!is_insert) {
            // fh_->delete_record(rid, context_);
            // 这里应该走不到
            throw IndexEntryRepeatError();
        }
        delete[] insert_key;
        delete[] delete_key;
    }
}

// 把表和索引全删了，再新建新的
void SmManager::reset_db() {
    for (auto& tab : db_.tabs_) {
        auto tab_name = tab.first;
        TabMeta& tab_meta = db_.get_table(tab_name);
        for (auto& index : tab_meta.indexes) {
            drop_index(tab_name, index.cols, nullptr);
        }
        auto file_handle_iter = fhs_.find(tab_name);
        rm_manager_->close_file(file_handle_iter->second.get());
        rm_manager_->destroy_file(tab_name);
        fhs_.erase(tab_name);

        int curr_offset = 0;
        for (auto& col : tab_meta.cols) {
            curr_offset += col.len;
        }
        rm_manager_->create_file(tab_name, curr_offset);
        fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));
        for (auto& index : tab_meta.indexes) {
            ix_manager_->create_index(tab_name, index.cols);
            auto index_handle = ix_manager_->open_index(tab_name, index.cols);
            auto index_name = ix_manager_->get_index_name(tab_name, index.cols);
            ihs_.emplace(index_name, std::move(index_handle));
        }
    }
}