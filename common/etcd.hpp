#pragma once

#include <etcd/KeepAlive.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Response.hpp>
#include <etcd/Value.hpp>
#include <etcd/Client.hpp>
#include <functional>
#include <thread>

#include "logger.hpp"

namespace wei_im{
// 服务注册客户端
class Registry{
    public:
        using ptr = std::shared_ptr<Registry>;
        Registry(const std::string& host):
            _client(make_shared<etcd::Client>(host)),
            _keepAlive(_client->leasekeepalive(3).get()),
            _leaseId(_keepAlive->Lease())
        {}
        ~Registry(){_keepAlive->Cancel();}

        bool serviceRegistry(const std::string& key, const std::string& val){
            auto resp = _client->put(key,val,_leaseId).get();
            if(resp.is_ok() == false){
                LOG_ERROR("服务新增失败:{}",resp.error_message());
                return false;
            }
            return true;
        }

    private:
        std::shared_ptr<etcd::Client> _client;
        std::shared_ptr<etcd::KeepAlive> _keepAlive;
        uint64_t _leaseId;  // 租约时间id
};


// 服务发现客户端
class Discovery{
    public:
        using ptr = std::shared_ptr<Discovery>;
        using NotifyCallBack = std::function<void(std::string, std:: string)>;
        Discovery(const std::string& host,
            const std::string &baseDir,
            const NotifyCallBack& put_cb,
            const NotifyCallBack& del_cb):
            _putCb(put_cb),
            _delCb(del_cb),
            _client(std::make_shared<etcd::Client>(host)),
            _watcher(std::make_shared<etcd::Watcher>(
                *_client.get(), baseDir, std::bind(&Discovery::callBack, this, std::placeholders::_1),
                true))
            {
                // 先进行服务发现，在进行服务watcher
                auto resp = _client->ls("/service").get();
                if(resp.is_ok() == false){
                    LOG_ERROR("获取服务信息失败:{}",resp.error_message());
                }

                int sz = resp.keys().size();
                for(int i = 0; i < sz; i++){
                    std::cout<< resp.value(i).as_string() << "可以提供" << resp.key(i) << "服务\n";
                }

            }
    private:
        void callBack(const etcd::Response & resp){
            if(resp.is_ok() == false){
                LOG_ERROR("错误事件通知: {}",resp.error_message());
                return ;
            }

            for(auto const& ev : resp.events()){
                if(ev.event_type() == etcd::Event::EventType::PUT){
                    if(_putCb)  _putCb(ev.kv().key(),ev.kv().as_string());
                    LOG_DEBUG("新增服务{}-{}",ev.kv().key(),ev.kv().as_string());
                }
                else if(ev.event_type() == etcd::Event::EventType::DELETE_){
                    LOG_DEBUG("下线服务{}-{}",ev.prev_kv().key(),ev.prev_kv().as_string());
                }
            }

        }

    private:
        NotifyCallBack _putCb;
        NotifyCallBack _delCb;
        std::shared_ptr<etcd::Client> _client;
        std::shared_ptr<etcd::Watcher> _watcher;


};
}
