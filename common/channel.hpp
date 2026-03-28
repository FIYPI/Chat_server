#pragma once

#include "logger.hpp"
#include <brpc/channel.h>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

DEFINE_string(protocol, "baidu_std", "通信协议类型，默认使用brpc自定制协议");
DEFINE_string(server_host, "127.0.0.1:8000", "服务器地址信息");
DEFINE_int32(timeout_ms, 500, "Rpc请求超时时间-毫秒");
DEFINE_int32(max_retry, 3, "请求重试次数");

class ServiceChannel {
public:
  using ChannelPtr = std::shared_ptr<brpc::Channel>;
  using ptr = std::shared_ptr<ServiceChannel>;
  ServiceChannel(const std::string &name) : _service_name(name), _index(0) {}
  void append(const std::string &host) {
    auto channel = std::make_shared<brpc::Channel>();
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;
    int ret = channel->Init(host.c_str(), &options);
    if (ret == -1) {
        LOG_ERROR("初始化{}-{}信道失败!", _service_name, host);
    }

    std::unique_lock<std::mutex> locker(_mutex);
    _hosts.insert(std::make_pair(host, channel));
    _channels.push_back(channel);
  }
  void remove(const std::string &host) {
    std::unique_lock<std::mutex> locker(_mutex);
    auto it = _hosts.find(host);
    if (it == _hosts.end()) {
      LOG_WARN("{}节点删除通道时，没有找到该管道信息", _service_name, host);
      return;
    }

    for (auto vit = _channels.begin(); vit != _channels.end(); vit++) {
      if (*vit == it->second) {
        _channels.erase(vit);
      }
      _hosts.erase(it);
    }
  }

  // 通过rr轮转获取到服务对应的管道
  ServiceChannel::ChannelPtr choose() {
    std::unique_lock<std::mutex> locker(_mutex);
    if (_channels.size() == 0) {
      LOG_ERROR("没有找到对应的{}服务的节点", _service_name);
      return ServiceChannel::ChannelPtr();
    }
    int32_t idx = _index++ % _channels.size();
    return _channels[idx];
  }

private:
  std::mutex _mutex;
  int32_t _index;
  std::string _service_name;
  std::vector<ChannelPtr> _channels;
  std::unordered_map<std::string, ChannelPtr> _hosts;
};

// 服务管理
class ServiceManager {
public:
  using ptr = std::shared_ptr<ServiceManager>;
  ServiceManager() {}

  // 获取指定服务的管道信息
  ServiceChannel::ChannelPtr choose(const std::string &service_name) {
    std::unique_lock<std::mutex> locker(_mutex);
    auto sit = _services.find(service_name);
    if (sit == _services.end()) {
      LOG_ERROR("没有找到对应的{}服务的节点", service_name);
      return ServiceChannel::ChannelPtr();
    }
    return sit->second->choose();
  }
  void declared(const std::string &service_name) {
    std::unique_lock<std::mutex> locker(_mutex);
    _attention_services.insert(service_name);
  }

  // 服务上线的回调接口
  void onServiceOnline(const std::string &service_instance,const std::string &host) {
    std::string service_name = getServiceName(service_instance);
      ServiceChannel::ptr service;
    {

      std::unique_lock<std::mutex> locker(_mutex);
      auto ait = _attention_services.find(service_name);
      if (ait == _attention_services.end()) {
        LOG_DEBUG("{}服务上线，但是当前并不关心", service_name);
        return;
      }
      auto sit = _services.find(service_name);
      if (sit == _services.end()) {
        service = std::make_shared<ServiceChannel>(service_name);
        _services.insert(std::make_pair(service_name, service));
      }else{
        service = sit->second;
      }
    }
    if (!service) {
      LOG_ERROR("新增{}管理节点失败", service_name);
      return;
    }

    service->append(host);
    LOG_DEBUG("{}-{} 服务上线，进行添加管理",service_name,host);
  }
  void onServiceOffline(const std::string &service_instance, const std::string &host) {
    std::string service_name = getServiceName(service_instance);
      ServiceChannel::ptr service;
    {
        std::unique_lock<std::mutex> locker(_mutex);
        auto sit = _services.find(service_name);
        if (sit == _services.end()) {
            LOG_WARN("删除{}节点失败，没有找到对应的管理对象", service_name);
            return;
        }
        service = sit->second;
    }
    service->remove(host);
    LOG_DEBUG("{}-{} 服务下线，进行删除管理",service_name,host);

  }
private:
  std::string getServiceName(const std::string& service_instance){
    auto pos = service_instance.find_last_of('/');
    if (pos == std::string::npos) return service_instance;
    return service_instance.substr(0,pos);
  }
private:
  std::mutex _mutex;
  std::unordered_set<std::string> _attention_services;
  std::unordered_map<std::string, ServiceChannel::ptr> _services;
};
