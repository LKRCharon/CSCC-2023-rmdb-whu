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

#include <iostream>
#include <map>

// 此处重载了<<操作符，在ColMeta中进行了调用
template <typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
std::ostream &operator<<(std::ostream &os, const T &enum_val) {
    os << static_cast<int>(enum_val);
    return os;
}

template <typename T, typename = typename std::enable_if<std::is_enum<T>::value, T>::type>
std::istream &operator>>(std::istream &is, T &enum_val) {
    int int_val;
    is >> int_val;
    enum_val = static_cast<T>(int_val);
    return is;
}

struct Rid {
    int page_no;
    int slot_no;

    friend bool operator==(const Rid &x, const Rid &y) { return x.page_no == y.page_no && x.slot_no == y.slot_no; }

    friend bool operator!=(const Rid &x, const Rid &y) { return !(x == y); }
};

enum ColType { TYPE_INT, TYPE_BIGINT, TYPE_FLOAT, TYPE_STRING, TYPE_DATETIME };

inline std::string coltype2str(ColType type) {
    std::map<ColType, std::string> m = {{TYPE_INT, "INT"},
                                        {TYPE_BIGINT, "BIGINT"},
                                        {TYPE_FLOAT, "FLOAT"},
                                        {TYPE_STRING, "STRING"},
                                        {TYPE_DATETIME, "DATETIME"}};
    return m.at(type);
}

typedef enum PlanTag {
    T_Invalid = 1,
    T_Help,
    T_ShowTable,
    T_ShowIndex,
    T_DescTable,
    T_CreateTable,
    T_DropTable,
    T_CreateIndex,
    T_DropIndex,
    T_Insert,
    T_Update,
    T_Delete,
    T_select,
    T_Transaction_begin,
    T_Transaction_commit,
    T_Transaction_abort,
    T_Transaction_rollback,
    T_SeqScan,
    T_IndexScan,
    T_NestLoop,
    T_Sort,
    T_Projection
} PlanTag;
inline std::string plantag2str(PlanTag tag) {
    std::map<PlanTag, std::string> m = {{T_Invalid, "Invalid"},
                                        {T_Help, "Help"},
                                        {T_ShowTable, "ShowTable"},
                                        {T_ShowIndex, "ShowIndex"},
                                        {T_DescTable, "DescTable"},
                                        {T_CreateTable, "CreateTable"},
                                        {T_DropTable, "DropTable"},
                                        {T_CreateIndex, "CreateIndex"},
                                        {T_DropIndex, "DropIndex"},
                                        {T_Insert, "Insert"},
                                        {T_Update, "Update"},
                                        {T_Delete, "delete"},
                                        {T_select, "select"},
                                        {T_Transaction_begin, "Transaction_begin"},
                                        {T_Transaction_commit, "Transaction_commit"},
                                        {T_Transaction_abort, "Transaction_abort"},
                                        {T_Transaction_rollback, "Transaction_rollback"},
                                        {T_SeqScan, "SeqScan"},
                                        {T_IndexScan, "IndexScan"},
                                        {T_NestLoop, "NestLoop"},
                                        {T_Sort, "Sort"},
                                        {T_Projection, "Projection"}};
    return m.at(tag);
}

class RecScan {
   public:
    virtual ~RecScan() = default;

    virtual void next() = 0;

    virtual bool is_end() const = 0;

    virtual Rid rid() const = 0;
};
