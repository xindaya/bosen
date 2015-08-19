// author: jinliang

#pragma once

#include <vector>
#include <glog/logging.h>

#include <petuum_ps_common/storage/abstract_store.hpp>
#include <petuum_ps_common/storage/abstract_store_iterator.hpp>

// #
// 总而言之
// vector作为最底层的存储，实现起来比map好像简单一点
// #


namespace petuum {
// V is an arithmetic type. V is the data type and also the update type.
// V needs to be POD.
// When Init(), memory is zero-ed out.
template<typename V>
class VectorStore : public AbstractStore<V> {
public:
  VectorStore();
  ~VectorStore();

  VectorStore(const VectorStore<V> &other);
  VectorStore<V> & operator = (const VectorStore<V> &other);

  // When init(), the memory is zeroed out.
  void Init(size_t capacity);
  size_t SerializedSize() const;
  size_t Serialize(void *bytes) const;
  void Deserialize(const void *data, size_t num_bytes);
  void ResetData(const void *data, size_t num_bytes);

  V Get (int32_t col_id) const;
  void Inc(int32_t col_id, V delta);

  V *GetPtr(int32_t col_id);
  const V *GetConstPtr(int32_t col_id) const;

  size_t get_capacity() const;

  const void CopyToVector(void *to) const;
  const void CopyToMem(void *to) const;

  const void *GetDataPtr() const;

private:
  std::vector<V> data_;
};

template<typename V>
VectorStore<V>::VectorStore() { }

template<typename V>
VectorStore<V>::~VectorStore() { }


// #复制创建新的结构#
template<typename V>
VectorStore<V>::VectorStore(const VectorStore<V> &other):
    data_(other.data_) { }

// #获取引用地址#
template<typename V>
VectorStore<V> & VectorStore<V>::operator = (const VectorStore<V> &other) {
  data_ = other.data_;
  return *this;
}

//#没想到这里使用的是memset来做初始化
// 怎么什么技术都用呢？
// #
template<typename V>
void VectorStore<V>::Init(size_t capacity) {
  data_.resize(capacity);
  memset(data_.data(), 0, capacity * sizeof(V));
}

template<typename V>
size_t VectorStore<V>::SerializedSize() const {
  return data_.size() * sizeof(V);
}

template<typename V>
size_t VectorStore<V>::Serialize(void *bytes) const {
  size_t num_bytes = data_.size()*sizeof(V);
  memcpy(bytes, data_.data(), num_bytes);
  return num_bytes;
}

template<typename V>
void VectorStore<V>::Deserialize(const void *data, size_t num_bytes) {
  size_t num_entries = num_bytes / sizeof(V);
  data_.resize(num_entries);
  memcpy(data_.data(), data, num_bytes);
}

template<typename V>
void VectorStore<V>::ResetData(const void *data, size_t num_bytes) {
  memcpy(data_.data(), data, num_bytes);
}

// #
// 按照列号码来取数据
// #
template<typename V>
V VectorStore<V>::Get (int32_t col_id) const {
  V v = data_[col_id];
  return v;
}

template<typename V>
void VectorStore<V>::Inc(int32_t col_id, V delta) {
  data_[col_id] += delta;
}

// #
// 没想到啊vector还提供这么底层的接口啊
// #
template<typename V>
V* VectorStore<V>::GetPtr(int32_t col_id) {
  return data_.data() + col_id;
}

// #
// 与上一个接口不同的是，这个不能修改是const，常量类型的
// #

template<typename V>
const V* VectorStore<V>::GetConstPtr(int32_t col_id) const {
  return data_.data() + col_id;
}

template<typename V>
size_t VectorStore<V>::get_capacity() const {
  return data_.size();
}

// #
// 拷贝到另外一个数组
// #
template<typename V>
const void VectorStore<V>::CopyToVector(void *to) const {
  auto &vec = *(reinterpret_cast<std::vector<V>*>(to));
  memcpy(vec.data(), data_.data(), data_.size()*sizeof(V));
}

// #
// 可以拷贝到一个指定的内存中，这个接口不错
// #

template<typename V>
const void VectorStore<V>::CopyToMem(void *to) const {
  memcpy(to, data_.data(), data_.size()*sizeof(V));
}

// #
// 获取整块数据的指针
// #
template<typename V>
const void *VectorStore<V>::GetDataPtr() const {
  return &data_;
}

}
