#include <petuum_ps_common/oplog/batch_inc_append_only_buffer.hpp>
#include <string.h>

namespace petuum {

bool BatchIncAppendOnlyBuffer::Inc(int32_t row_id, int32_t col_id, const void *delta) {
  if (size_ + sizeof(int32_t) + sizeof(int32_t) + update_size_
      > capacity_)
    return false;

  *(reinterpret_cast<int32_t*>(buff_.get() + size_)) = row_id;
  size_ += sizeof(int32_t);

  *(reinterpret_cast<int32_t*>(buff_.get() + size_)) = 1;
  size_ += sizeof(int32_t);

  *(reinterpret_cast<int32_t*>(buff_.get() + size_)) = col_id;
  size_ += sizeof(int32_t);

  memcpy(buff_.get() + size_, delta, update_size_);
  size_ += update_size_;

  return true;
}

bool BatchIncAppendOnlyBuffer::BatchInc(int32_t row_id, const int32_t *col_ids,
                                        const void *deltas, int32_t num_updates) {
  //#�������жϣ�����������������ƣ���ô���ݾ�д���ɹ������callerʹ�õ����������Ի��ƵĻ�����ô���߼���
  // ����д���̣߳�ֱ��д��ɹ�Ϊֹ
  // #
  if (size_ + sizeof(int32_t) + sizeof(int32_t) +
      (sizeof(int32_t) + update_size_)*num_updates > capacity_)
    return false;
//#д��row_id#
  *(reinterpret_cast<int32_t*>(buff_.get() + size_)) = row_id;
  size_ += sizeof(int32_t);
//#д����µ����ݹ�ģ#
  *(reinterpret_cast<int32_t*>(buff_.get() + size_)) = num_updates;
  size_ += sizeof(int32_t);
//#��Ҫ���µ��е�idȫ��#
  memcpy(buff_.get() + size_, col_ids, sizeof(int32_t)*num_updates);
  size_ += sizeof(int32_t)*num_updates;
//#�����µ�deltaд��buffer��#
  memcpy(buff_.get() + size_, deltas, num_updates);
  size_ += update_size_*num_updates;

  return true;
}

//#
// ���û�и���������е�id�б����Ǵ�һ��ƫ������ʼ�����¶������
//
// �Ӿ����ʵ����������Ҳ�ǽ��������ת��Ϊ��ͳ������Ĺ���
// #

bool BatchIncAppendOnlyBuffer::DenseBatchInc(int32_t row_id, const void *deltas,
                                             int32_t index_st,
                                             int32_t num_updates) {
  if (size_ + sizeof(int32_t) + sizeof(int32_t) +
      (sizeof(int32_t) + update_size_)*num_updates > capacity_)
    return false;

  *(reinterpret_cast<int32_t*>(buff_.get() + size_)) = row_id;
  size_ += sizeof(int32_t);

  *(reinterpret_cast<int32_t*>(buff_.get() + size_)) = num_updates;
  size_ += sizeof(int32_t);
//#��¼id ��list#
  for (int32_t i = 0; i < num_updates; ++i) {
    *(reinterpret_cast<int32_t*>(buff_.get() + size_)) = index_st + i;
    size_ += sizeof(int32_t);
  }
  //�������²���
  memcpy(buff_.get() + size_, deltas, num_updates);
  size_ += update_size_*num_updates;

  return true;
}


void BatchIncAppendOnlyBuffer::InitRead() {
  read_ptr_ = buff_.get();
}

//#���ص���һ��void���͵�ָ��#
const void *BatchIncAppendOnlyBuffer::Next(
    int32_t *row_id, int32_t const **col_ids, int32_t *num_updates) {
  if (read_ptr_ >= buff_.get() + size_)
    return 0;
    // #����row_id��Ȼ��ָ�����һλ#
  *row_id = *(reinterpret_cast<int32_t*>(read_ptr_));
  read_ptr_ += sizeof(int32_t);
//#�������µ�delta��Ŀ��ָ�����һλ#
  *num_updates = *(reinterpret_cast<int32_t*>(read_ptr_));
  read_ptr_ += sizeof(int32_t);
//#����col_ids��ָ���������λ#
  *col_ids = reinterpret_cast<int32_t*>(read_ptr_);
  read_ptr_ += sizeof(int32_t)*(*num_updates);

  void *update_ptr = read_ptr_;

  read_ptr_ += update_size_*(*num_updates);

  return update_ptr;
}

}
