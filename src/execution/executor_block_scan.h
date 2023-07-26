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
#include "record/rm_file_handle.h"
#include "system/sm.h"

/**
 * @brief  管理Join开辟的Buffer里的page
 * 作用：复制页到buffer，供上层blockscanner进行扫描
 */
class BlockBufferManager {
   private:
    const RmFileHandle *file_handle_;
    Rid rid_;              // 用来读page的rid，不需要给上层返回
    Page *pages_;          // pages 起始地址,已经考虑了偏移
    int max_size_;         // size，内表为1，外表N-1
    int size_;             // buffer 可能占不满
    bool is_end = false;   // 该表是否到达结尾
    bool is_full = false;  // pages是否是满的

   public:
    BlockBufferManager(const RmFileHandle *file_handle, Page *pages, int size)
        : file_handle_(file_handle), pages_(pages), max_size_(size) {
        // 初始化file_handle和rid（指向第一个存放了记录的位置）
        // 同时把页记到pages_里
        rid_.page_no = RM_FIRST_RECORD_PAGE;
        rid_.slot_no = -1;
        next();
    }
    /**
     * @brief  重置buffer里的pages
     */
    void reset() {
        memset(pages_, 0, max_size_ * sizeof(Page));
        is_full = false;
        if (is_end) {
            is_end = false;
            rid_.page_no = RM_FIRST_RECORD_PAGE;
            rid_.slot_no = -1;
        }
    }
    // 重新获取新的buffer，不能由scanner调用，应该在join里，判断两个scan都到末尾
    void next() {
        reset();
        // 找到文件中下一个存放了记录的非空闲位置，用rid_来指向这个位置
        size_ = 0;
        auto max_n = file_handle_->file_hdr_.num_records_per_page;
        // join buffer没满，且没扫完表最后一个页
        while (size_ < max_size_ && rid_.page_no < file_handle_->file_hdr_.num_pages) {
            auto page_handle = file_handle_->fetch_page_handle(rid_.page_no);
            rid_.slot_no = Bitmap::next_bit(true, page_handle.bitmap, max_n, rid_.slot_no);
            // 页里有数据
            if (rid_.slot_no < max_n) {
                // TODO page存到pages里
                memcpy(pages_ + size_, page_handle.page, sizeof(Page));
                size_++;
                // DONE
            }
            file_handle_->bpm_->unpin_page(page_handle.page->get_page_id(), false);
            //没数据 直接下一个页
            rid_.slot_no = -1;
            rid_.page_no++;
        }
        // while结束两种情况：buffer满了，或表扫完了
        if (size_ == max_size_) {
            is_full = true;
        }
        if (rid_.page_no == file_handle_->file_hdr_.num_pages) {
            is_end = true;
        }
    }

    bool isEnd() const { return is_end; };
    bool isFull() const { return is_full; };
    int getSize() { return size_; };
};

// 用于块嵌套的scanner
// 封装左右孩子的scan算子，
class BlockScanner {
   private:
    std::unique_ptr<AbstractExecutor> scanner_;
    RmFileHdr file_hdr_;
    Page *pages_;
    int size_;
    BlockBufferManager *bbm_;
    // RmPageHandle* page_handle_;
    int position_ = 0;
    int slot_no_ = -1;
    int max_n_;
    bool is_buffer_end = false;

   public:
    BlockScanner(std::unique_ptr<AbstractExecutor> scanner, Page *pages, int size) : pages_(pages), size_(size) {
        scanner_ = std::move(scanner);
        file_hdr_ = scanner_->GetFileHandle()->get_file_hdr();
        bbm_ = new BlockBufferManager(scanner_->GetFileHandle(), pages, size);
        max_n_ = file_hdr_.num_records_per_page;
    }


    ~BlockScanner() { delete bbm_; }

    void bbmNext() { bbm_->next(); }

    void beginTuple() {
        is_buffer_end = false;
        position_ = 0;
        
        auto page = pages_ + position_;
        auto page_handle = RmPageHandle(&file_hdr_, page);
        slot_no_ = Bitmap::next_bit(true, page_handle.bitmap, max_n_, slot_no_);

        while (!isBufferEnd()) {
            auto rec = getTuple();
            if (eval_conds(scanner_->cols(), scanner_->get_conds(), rec.get())) {
                break;
            }
            nextTuple();
        }
        // auto rec = getTuple();
        // eval_conds(scanner_->cols(),scanner_->get_conds(),rec.get());
    }

    //找到下一条记录，不一定是满足条件的
    void next() {
        while (!isBufferEnd()) {
            //先找下一条记录
            auto page = pages_ + position_;
            auto page_handle = RmPageHandle(&file_hdr_, page);
            slot_no_ = Bitmap::next_bit(true, page_handle.bitmap, max_n_, slot_no_);
            if (slot_no_ < max_n_) {
                return;
            }
            slot_no_ = -1;
            position_++;
            if (position_ == bbm_->getSize()) {
                is_buffer_end = true;
                break;
            }
        }
    }
    void nextTuple() {
        next();
        while (!isBufferEnd()) {
            auto rec = getTuple();
            if (eval_conds(scanner_->cols(), scanner_->get_conds(), rec.get())) {
                break;
            };
            next();
        }
    }
    std::unique_ptr<RmRecord> getTuple() {
        auto page = pages_ + position_;
        auto page_handle = RmPageHandle(&file_hdr_, page);
        auto record_size = page_handle.file_hdr->record_size;
        auto record = std::make_unique<RmRecord>(record_size, page_handle.get_slot(slot_no_));
        return record;
    }

    bool isBbmEnd() { return bbm_->isEnd(); }
    bool isBufferEnd() { return is_buffer_end; }

    void feed(const std::map<TabCol, Value> &feed_dict, const std::vector<Condition> &fed_conds) {
        scanner_->feed(feed_dict, fed_conds);
    }

    /**
     * @brief 判断是否满足谓词条件
     *
     */
    bool eval_cond(const std::vector<ColMeta> &rec_cols, const Condition &cond, const RmRecord *rec) {
        auto lhs_col = scanner_->get_col(rec_cols, cond.lhs_col);
        char *lhs = rec->data + lhs_col->offset;
        char *rhs;
        ColType rhs_type;
        if (cond.is_rhs_val) {
            rhs_type = cond.rhs_val.type;
            rhs = cond.rhs_val.raw->data;
        } else {
            // rhs is a column
            auto rhs_col = scanner_->get_col(rec_cols, cond.rhs_col);
            rhs_type = rhs_col->type;
            rhs = rec->data + rhs_col->offset;
        }
        assert(rhs_type == lhs_col->type);  // TODO convert to common type
        int cmp = ix_compare(lhs, rhs, rhs_type, lhs_col->len);
        if (cond.op == OP_EQ) {
            return cmp == 0;
        } else if (cond.op == OP_NE) {
            return cmp != 0;
        } else if (cond.op == OP_LT) {
            return cmp < 0;
        } else if (cond.op == OP_GT) {
            return cmp > 0;
        } else if (cond.op == OP_LE) {
            return cmp <= 0;
        } else if (cond.op == OP_GE) {
            return cmp >= 0;
        } else {
            throw InternalError("Unexpected op type");
        }
    }

    bool eval_conds(const std::vector<ColMeta> &rec_cols, const std::vector<Condition> &conds, const RmRecord *rec) {
        return std::all_of(conds.begin(), conds.end(),
                           [&](const Condition &cond) { return eval_cond(rec_cols, cond, rec); });
    }

    const std::vector<ColMeta> &cols() const { return scanner_->cols(); }
};
