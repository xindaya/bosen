#pragma once

#include <boost/thread.hpp>
#include <unordered_map>
#include <cstdint>
#include <utility>
#include <vector>
#include <glog/logging.h>

namespace petuum {


//#
// 多线程的稀疏行，支持不约束，或者说是无限制的列
// 这里没有非零的entry在map类型的存储中
// 如果遇到有0的情况，比如说在ApplyInc 或者ApplyBatchInc中，就会将
// 0的情况删除
// #
// Multi-threaded sparse row, support unbounded # of columns.  There is never
// non-zero entry in the map storage, as ApplyInc / ApplyBatchInc
// removes the zero-writes.


    template<typename V>
class MapStore : public AbstractStore<V> {
public:

  MapStore() { }
  ~MapStore() { }


  MapStore(const MapStore<V> &other):
      data_(other.data_) { }

    //#
    // & operator 原来是引用啊
    // #
  MapStore<V> & operator = (const MapStore<V> &other) {
    data_ = other.data_;
    return *this;
  }

  void Init(size_t capacity) { }

  size_t SerializedSize() const;
  size_t Serialize(void *bytes) const;
  void Deserialize(const void *data, size_t num_bytes);

  V Get(int32_t col_id) const;
  void Inc(int32_t col_id, V delta);

  const void CopyToVector(void *to) const;

  size_t num_entries() const;

private:
    //#
    // 使用无序map来作为data的存储
    // 1. 简单
    // 2. 方便
    // #
  std::unordered_map<int32_t, V> data_;
};

// ================= Implementation =================

    // 传入的是key，得到的是value，同时对数据类型做了处理
template<typename V>
V MapStore<V>::Get(int32_t col_id) const {
  auto it = data_.find(col_id);
  if (it == data_.cend()) {
    return V(0);
  }
  return it->second;
}

template<typename V>
void MapStore<V>::Inc(int32_t col_id, V delta) {
  V &val = data_[col_id];
  val += delta;
  if (val == V(0))
    data_.erase(col_id);
}

template<typename V>
const void MapStore<V>::CopyToVector(void *to) const {
  std::vector<std::pair<int32_t, V> > *vec
      = reinterpret_cast<std::vector<std::pair<int32_t, V> >*>(to);
  vec->resize(data_.size());
  vec->clear();

    //将数据一条一条的灌入vec中
  for (const auto & pair : data_) {
    vec->push_back(pair);
  }
}

template<typename V>
size_t MapStore<V>::num_entries() const {
  return data_.size();
}

//序列化value需要的空间大小
template<typename V>
size_t MapStore<V>::SerializedSize() const {
  return data_.size() * (sizeof(int32_t) + sizeof(V));
}

//#
// 这个应该没有实现具体的方法，需要todo吧
// TODO
// #
template<typename V>
size_t MapStore<V>::Serialize(void* bytes) const {
  void* data_ptr = bytes;
  for (const std::pair<int32_t, V>& entry : data_) {
    int32_t* col_ptr = reinterpret_cast<int32_t*>(data_ptr);
    *col_ptr = entry.first;
    ++col_ptr;
    V* val_ptr = reinterpret_cast<V*>(col_ptr);
    *val_ptr = entry.second;
    data_ptr = reinterpret_cast<void*>(++val_ptr);
  }
  return SerializedSize();
}

template<typename V>
void MapStore<V>::Deserialize(const void* data, size_t num_bytes) {
  data_.clear();

  int32_t num_bytes_per_entry = (sizeof(int32_t) + sizeof(V));

  int32_t num_entries = num_bytes / num_bytes_per_entry;

  const uint8_t* data_ptr = reinterpret_cast<const uint8_t*>(data);
  for (int i = 0; i < num_entries; ++i) {
    const int32_t* col_ptr = reinterpret_cast<const int32_t*>(data_ptr);
    int32_t col_id = *col_ptr;
    ++col_ptr;
    const V* val_ptr = reinterpret_cast<const V*>(col_ptr);
    V val = *val_ptr;
    data_ptr = reinterpret_cast<const uint8_t*>(++val_ptr);
    data_[col_id] = val;
  }
}

}  // namespace petuum
