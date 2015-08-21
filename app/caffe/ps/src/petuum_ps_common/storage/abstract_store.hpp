// author: jinliang

#pragma once

#include <petuum_ps_common/storage/abstract_store_iterator.hpp>

#include <glog/logging.h>

namespace petuum {

//#
//  翻译一下
//  这里需要这个数据类型是数字类型的
//
// #

//V是数字类型的，同时V也是data的类型和更新的类型

// V is an arithmetic type. V is the data type and also the update type.
// V needs to be a numeric type.
template<typename V>
class AbstractStore {
public:
  AbstractStore() { }
  virtual ~AbstractStore() { }

// 拷贝赋值函数
  AbstractStore(const AbstractStore<V> &other) { }

  // 直接赋值
  AbstractStore<V> & operator = (const AbstractStore<V> &other) { }

// 初始值需要配置，需要在启动的时候使用
  virtual void Init(size_t capacity) = 0;

  virtual size_t SerializedSize() const = 0;

  virtual size_t Serialize(void *bytes) const = 0;

  // May be called before Init().
  virtual void Deserialize(const void *data, size_t num_bytes) = 0;

  // Only called after Init().
  virtual void ResetData(const void *data, size_t num_bytes) {
    Deserialize(data, num_bytes);
  }

  virtual V Get (int32_t col_id) const = 0;

  virtual void Inc(int32_t col_id, V delta) = 0;

  virtual const void CopyToVector(void *to) const = 0;

  // contiguous memory
  virtual V *GetPtr(int32_t col_id) {
    LOG(FATAL) << "Not yet supported";
    return 0;
  }

  virtual const V *GetConstPtr(int32_t col_id) const {
    LOG(FATAL) << "Not yet supported";
    return 0;
  }

  virtual const void *GetDataPtr() const {
    LOG(FATAL) << "Not yet supported";
    return 0;
  }

  static_assert(std::is_pod<V>::value, "V must be POD");
};

}
