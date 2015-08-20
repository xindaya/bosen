#pragma once

#include <petuum_ps/server/name_node_thread.hpp>
#include <pthread.h>
//#定义了几个基本的接口，启动，关闭#
//#具体定义在name_node_thread.hpp中#
//#这个pthread_barrier_t 是一个用来同步线程的工具#
namespace petuum {

class NameNode {
public:
  static void Init();
  static void ShutDown();
private:
  static NameNodeThread *name_node_thread_;
  static pthread_barrier_t init_barrier_;
};

}
