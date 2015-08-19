#pragma once

#include <string>
#include <glog/logging.h>
// #HostInfo ����ṹ�壬���ǽ�localhost�ļ��е����ݸ�ʽ�������˽��������ڷ���#
namespace petuum {

struct HostInfo {
  int32_t id;
  std::string ip;
  std::string port;
  
  HostInfo() {}
  ~HostInfo() { }
  HostInfo(int32_t _id, std::string _ip, std::string _port):
    id(_id),
    ip(_ip),
    port(_port) {};

  HostInfo(const HostInfo &other):
    id(other.id),
    ip(other.ip),
    port(other.port) {}

  HostInfo & operator = (const HostInfo &other) {
    id = other.id;
    ip = other.ip;
    port = other.port;
    return *this;
  }
};

}
