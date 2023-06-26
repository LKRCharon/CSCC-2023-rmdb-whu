/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/common.h"

long long DatetimeStrToLL(const std::string &str) {
    // 长度不对
    if (str.size() != 19) {
        throw DatetimeFormatError(str);
    }
    long long time = 0;
    const char *delimiter = "-: ";
    std::string temp;
    temp.assign(str);
    // 下面是判断数据范围所用
    long long downsides[10] = {1000, 1, 1, 0, 0, 0};
    long long upsides[10] = {9999, 12, 31, 23, 59, 59};
    int index = 0;
    // 对2月特判
    bool isFeb = false;
    ;
    // 开始转换
    char *token = std::strtok(const_cast<char *>(temp.c_str()), delimiter);
    while (token != nullptr) {
        long long value = std::strtoll(token, nullptr, 10);
        time += value;
        // 检查数据范围：
        if (value > upsides[index] || value < downsides[index]) {
            throw DatetimeFormatError(str);
        }
        if (index == 1 && value == 2) {
            isFeb = true;
        }
        if (index == 2 && isFeb && value > 29) {
            throw DatetimeFormatError(str);
        }

        token = std::strtok(nullptr, delimiter);
        if (token != nullptr) {
            time *= 100;
            index++;
        }
    }
    if (index != 5) {
        throw DatetimeFormatError(str);
    }
    return time;
}

std::string DatetimeLLtoStr(long long ll) {
    std::string str = std::to_string(ll);
    // 20201231123456-> 2020-12-31 12:34:56
    str.insert(4, 1, '-');
    str.insert(7, 1, '-');
    str.insert(10, 1, ' ');
    str.insert(13, 1, ':');
    str.insert(16, 1, ':');
    return str;
}