/*
 * abstract_table_group.hpp
 * author: jinliang
 */

#pragma once

#include <map>
#include <cstdint>

#include <petuum_ps_common/include/configs.hpp>
#include <petuum_ps_common/include/table.hpp>
#include <petuum_ps_common/include/abstract_row.hpp>
#include <petuum_ps_common/util/class_register.hpp>

#include <petuum_ps_common/client/abstract_client_table.hpp>

#include <boost/noncopyable.hpp>
//从这个名字来看，是对table的管理
namespace petuum {

class AbstractTableGroup {
public:
  AbstractTableGroup() { }

  virtual ~AbstractTableGroup() { }

// 创建表的时候，还要自己给表整个id?
// 太不人性化了
// #
  virtual bool CreateTable(int32_t table_id,
                           const ClientTableConfig& table_config) = 0;

  virtual void CreateTableDone() = 0;

// table需要注册thread，真是奇怪了，到底怎么搞的？
  virtual void WaitThreadRegister() = 0;

  virtual AbstractClientTable *GetTableOrDie(int32_t table_id) = 0;

  virtual int32_t RegisterThread() = 0;

  virtual void DeregisterThread() = 0;

  virtual void Clock() = 0;

  virtual void GlobalBarrier() = 0;

  virtual void TurnOnEarlyComm() = 0;

  virtual void TurnOffEarlyComm() = 0;

};

}   // namespace petuum
