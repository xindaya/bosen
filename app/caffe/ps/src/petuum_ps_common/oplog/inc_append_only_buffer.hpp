// Author: Jinliang
#pragma once

#include <stdint.h>
#include <petuum_ps_common/oplog/abstract_append_only_buffer.hpp>

namespace petuum {

/*
 * This is the append-only buffer type that's specially optimized for INC
 * operations. The buffer memory is organized as a list of (row_id, col_id,
 * update) tuples.
 */
//# 这也是一个只做append的buffer
// 不过和为批量操作不同的是，它不是为了批量优化，而是更新单个变量
// 也是每个thread绑定的
//
// #
class IncAppendOnlyBuffer : public AbstractAppendOnlyBuffer {
public:
  IncAppendOnlyBuffer(int32_t thread_id, size_t capacity, size_t update_size):
      AbstractAppendOnlyBuffer(thread_id, capacity, update_size) { }

  virtual ~IncAppendOnlyBuffer() { }

  // return true if succeed, otherwise false
  bool Inc(int32_t row_id, int32_t col_id, const void *delta);
  bool BatchInc(int32_t row_id, const int32_t *col_ids,
                const void *deltas, int32_t num_updates) ;
  bool DenseBatchInc(int32_t row_id, const void *updates, int32_t index_st,
                     int32_t num_updates);
  void InitRead();
  const void *Next(int32_t *row_id, int32_t const **col_ids,
                   int32_t *num_updates);
};

}
