// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.

#pragma once

#include <vector>
#include "rocksdb/immutable_options.h"
#include "rocksdb/options.h"
#include "util/compression.h"

namespace rocksdb {

struct MutableCFOptions {
  MutableCFOptions(const Options& options, const ImmutableCFOptions& ioptions)
      : write_buffer_size(options.write_buffer_size),
        max_write_buffer_number(options.max_write_buffer_number),
        arena_block_size(options.arena_block_size),
        memtable_prefix_bloom_size_ratio(
            options.memtable_prefix_bloom_size_ratio),
        memtable_prefix_bloom_huge_page_tlb_size(
            options.memtable_prefix_bloom_huge_page_tlb_size),
        max_successive_merges(options.max_successive_merges),
        filter_deletes(options.filter_deletes),
        inplace_update_num_locks(options.inplace_update_num_locks),
        disable_auto_compactions(options.disable_auto_compactions),
        soft_pending_compaction_bytes_limit(
            options.soft_pending_compaction_bytes_limit),
        hard_pending_compaction_bytes_limit(
            options.hard_pending_compaction_bytes_limit),
        level0_file_num_compaction_trigger(
            options.level0_file_num_compaction_trigger),
        level0_slowdown_writes_trigger(options.level0_slowdown_writes_trigger),
        level0_stop_writes_trigger(options.level0_stop_writes_trigger),
        compaction_pri(options.compaction_pri),
        max_grandparent_overlap_factor(options.max_grandparent_overlap_factor),
        expanded_compaction_factor(options.expanded_compaction_factor),
        source_compaction_factor(options.source_compaction_factor),
        target_file_size_base(options.target_file_size_base),
        target_file_size_multiplier(options.target_file_size_multiplier),
        max_bytes_for_level_base(options.max_bytes_for_level_base),
        max_bytes_for_level_multiplier(options.max_bytes_for_level_multiplier),
        max_bytes_for_level_multiplier_additional(
            options.max_bytes_for_level_multiplier_additional),
        verify_checksums_in_compaction(options.verify_checksums_in_compaction),
        max_subcompactions(options.max_subcompactions),
        max_sequential_skip_in_iterations(
            options.max_sequential_skip_in_iterations),
        paranoid_file_checks(options.paranoid_file_checks),
        report_bg_io_stats(options.report_bg_io_stats),
        compression(options.compression),
        min_partial_merge_operands(options.min_partial_merge_operands),
        compaction_options_fifo(ioptions.compaction_options_fifo) {
    RefreshDerivedOptions(ioptions);
  }
  MutableCFOptions()
      : write_buffer_size(0),
        max_write_buffer_number(0),
        arena_block_size(0),
        memtable_prefix_bloom_size_ratio(0),
        memtable_prefix_bloom_huge_page_tlb_size(0),
        max_successive_merges(0),
        filter_deletes(false),
        inplace_update_num_locks(0),
        disable_auto_compactions(false),
        soft_pending_compaction_bytes_limit(0),
        hard_pending_compaction_bytes_limit(0),
        level0_file_num_compaction_trigger(0),
        level0_slowdown_writes_trigger(0),
        level0_stop_writes_trigger(0),
        compaction_pri(kByCompensatedSize),
        max_grandparent_overlap_factor(0),
        expanded_compaction_factor(0),
        source_compaction_factor(0),
        target_file_size_base(0),
        target_file_size_multiplier(0),
        max_bytes_for_level_base(0),
        max_bytes_for_level_multiplier(0),
        verify_checksums_in_compaction(false),
        max_subcompactions(1),
        max_sequential_skip_in_iterations(0),
        paranoid_file_checks(false),
        report_bg_io_stats(false),
        compression(Snappy_Supported() ? kSnappyCompression : kNoCompression),
        min_partial_merge_operands(2) {}

  // Must be called after any change to MutableCFOptions
  void RefreshDerivedOptions(const ImmutableCFOptions& ioptions);

  // Get the max file size in a given level.
  uint64_t MaxFileSizeForLevel(int level) const;
  // Returns maximum total overlap bytes with grandparent
  // level (i.e., level+2) before we stop building a single
  // file in level->level+1 compaction.
  uint64_t MaxGrandParentOverlapBytes(int level) const;
  uint64_t ExpandedCompactionByteSizeLimit(int level) const;
  int MaxBytesMultiplerAdditional(int level) const {
    if (level >=
        static_cast<int>(max_bytes_for_level_multiplier_additional.size())) {
      return 1;
    }
    return max_bytes_for_level_multiplier_additional[level];
  }

  void Dump(Logger* log) const;

  // Memtable related options
  size_t write_buffer_size;
  int max_write_buffer_number;
  size_t arena_block_size;
  double memtable_prefix_bloom_size_ratio;
  size_t memtable_prefix_bloom_huge_page_tlb_size;
  size_t max_successive_merges;
  bool filter_deletes;
  size_t inplace_update_num_locks;

  // Compaction related options
  bool disable_auto_compactions;
  uint64_t soft_pending_compaction_bytes_limit;
  uint64_t hard_pending_compaction_bytes_limit;
  int level0_file_num_compaction_trigger;
  int level0_slowdown_writes_trigger;
  int level0_stop_writes_trigger;
  CompactionPri compaction_pri;
  int max_grandparent_overlap_factor;
  int expanded_compaction_factor;
  int source_compaction_factor;
  uint64_t target_file_size_base;
  int target_file_size_multiplier;
  uint64_t max_bytes_for_level_base;
  int max_bytes_for_level_multiplier;
  std::vector<int> max_bytes_for_level_multiplier_additional;
  bool verify_checksums_in_compaction;
  int max_subcompactions;

  // Misc options
  uint64_t max_sequential_skip_in_iterations;
  bool paranoid_file_checks;
  bool report_bg_io_stats;
  CompressionType compression;
  uint32_t min_partial_merge_operands;
  CompactionOptionsFIFO compaction_options_fifo;

  // Derived options
  // Per-level target file size.
  std::vector<uint64_t> max_file_size;
};

uint64_t MultiplyCheckOverflow(uint64_t op1, int op2);

}  // namespace rocksdb
