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

#include "common/context.h"
#include "index/ix.h"
#include "record/rm_file_handle.h"
#include "sm_defs.h"
#include "sm_meta.h"

class Context;

struct ColDef {
    std::string name;  // Column name
    ColType type;      // Type of column
    int len;           // Length of column
};

/* 系统管理器，负责元数据管理和DDL语句的执行 */
class SmManager {
   public:
    DbMeta db_;  // 当前打开的数据库的元数据
    std::unordered_map<std::string, std::unique_ptr<RmFileHandle>>
        fhs_;  // file name -> record file handle, 当前数据库中每张表的数据文件
    std::unordered_map<std::string, std::unique_ptr<IxIndexHandle>>
        ihs_;  // file name -> index file handle, 当前数据库中每个索引的文件
   private:
    DiskManager* disk_manager_;
    BufferPoolManager* bpm_;
    RmManager* rm_manager_;
    IxManager* ix_manager_;

   public:
    SmManager(DiskManager* disk_manager, BufferPoolManager* buffer_pool_manager, RmManager* rm_manager,
              IxManager* ix_manager)
        : disk_manager_(disk_manager), bpm_(buffer_pool_manager), rm_manager_(rm_manager), ix_manager_(ix_manager) {}

    ~SmManager() {}

    BufferPoolManager* get_bpm() { return bpm_; }

    RmManager* get_rm_manager() { return rm_manager_; }

    IxManager* get_ix_manager() { return ix_manager_; }

    bool is_dir(const std::string& db_name);

    void create_db(const std::string& db_name);

    void drop_db(const std::string& db_name);

    void open_db(const std::string& db_name);

    void close_db();

    void reset_db();

    void flush_meta();

    void show_tables(Context* context);

    void desc_table(const std::string& tab_name, Context* context);

    void create_table(const std::string& tab_name, const std::vector<ColDef>& col_defs, Context* context);

    void drop_table(const std::string& tab_name, Context* context);

    void create_index(const std::string& tab_name, const std::vector<std::string>& col_names, Context* context);

    void drop_index(const std::string& tab_name, const std::vector<std::string>& col_names, Context* context);

    void drop_index(const std::string& tab_name, const std::vector<ColMeta>& col_names, Context* context);

    void show_index(const std::string& tab_name, Context* context);

    // void show_index(const std::string& tab_name);

    // https://github.com/ruc-deke/rucbase-lab/blob/main/src/system/sm_manager.h
    // Transaction rollback management
    /**
     * @brief rollback the insert operation
     *
     * @param tab_name the name of the table
     * @param rid the rid of the record
     * @param txn the transaction
     */
    void rollback_insert(const std::string& tab_name, const Rid& rid, Context* context);

    /**
     * @brief rollback the delete operation
     *
     * @param tab_name the name of the table
     * @param txn the transaction
     */
    void rollback_delete(const std::string& tab_name, const RmRecord& record, Context* context);

    /**
     * @brief rollback the delete operation
     * @param tab_name the name of the table
     * @param rid the value of the deleted record
     * @param txn the transaction
     */
    void rollback_delete(const std::string& tab_name, const RmRecord& record, Rid& rid, Context* context);

    /**
     * @brief rollback the update operation
     *
     * @param tab_name the name of the table
     * @param rid the rid of the record
     * @param record record的旧值
     * @param txn the transaction
     */
    void rollback_update(const std::string& tab_name, const Rid& rid, const RmRecord& record, Context* context);

    /**
     * @brief rollback the table drop operation
     *
     * @param tab_name the name of the deleted table
     */
    void rollback_drop_table(const std::string& tab_name, Context* context);

    /**
     * @brief rollback the table create operation
     *
     * @param tab_name the name of the created table
     */
    void rollback_create_table(const std::string& tab_name, Context* context);

    /**
     * @brief rollback the create index operation
     *
     * @param tab_name the name of the table
     * @param col_name the name of the column on which index is created
     */
    void rollback_create_index(const std::string& tab_name, const std::string& col_name, Context* context);

    /**
     * @brief rollback the drop index operation
     *
     * @param tab_name the name of the table
     * @param col_name the name of the column on which index is created
     */
    void rollback_drop_index(const std::string& tab_name, const std::string& col_name, Context* context);
};