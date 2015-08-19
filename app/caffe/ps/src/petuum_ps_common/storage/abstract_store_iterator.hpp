#pragma once

#include <boost/noncopyable.hpp>
#include <stdint.h>

//#
// ����ʹ����boost��noncopyable ���ͣ�˵��������һ����������
//
//  ����Ӧ��Ҫʵ�ֵĶ������Ƕ����еĴ洢�����͵�һ�������ĳ���
//  get_key
//  is_end
// #
namespace petuum {
template<typename V>
class AbstractIterator : boost::noncopyable {
public:
  AbstractIterator() { }
  virtual ~AbstractIterator() { }

  virtual int32_t get_key() = 0;
  virtual V & operator *() = 0;
  virtual void operator ++ () = 0;
  virtual bool is_end() = 0;
};

template<typename V>
class AbstractConstIterator : boost::noncopyable {
public:
  AbstractConstIterator() { }
  virtual ~AbstractConstIterator() { }

  virtual int32_t get_key() = 0;
  virtual V operator *() = 0;
  virtual void operator ++ () = 0;
  virtual bool is_end() = 0;
};

}
