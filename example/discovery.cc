#include "../common/etcd.hpp"
#include "../common/channel.hpp"
#include "main.pb.h"
#include <gflags/gflags.h>
#include <ostream>
#include <thread>

DEFINE_bool(run_mode, false, "程序的运行模式，false-调试； true-发布；");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(call_service, "/service/echo", "调服务监控目录");



int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    auto sm = std::make_shared<ServiceManager>();
    sm->declared(FLAGS_call_service);
    std::cout<<"declared"<<std::endl;
    // 构造服务发现对象
    // 处理回调函数
    std::cout<<"callback"<<std::endl;
    auto put_cb = std::bind(&ServiceManager::onServiceOnline,sm.get(),std::placeholders::_1,std::placeholders::_2);
    auto del_cb = std::bind(&ServiceManager::onServiceOffline,sm.get(),std::placeholders::_1,std::placeholders::_2);
    Discovery::ptr dclient = std::make_shared<Discovery>(FLAGS_etcd_host,FLAGS_base_service,put_cb, del_cb);
    // 发起echoRpc调用
    std::cout<<"发起调用···"<<std::endl;
    while(1){
        // 通过rpc管理信道
        auto channel = sm->choose(FLAGS_call_service);
        if(!channel) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return -1;
        }
        example::EchoService_Stub stub(channel.get());

        std::cout<<"创建对象，响应···"<<std::endl;
        // 创建请求、响应、控制对象
        example::EchoRequest request;
        request.set_message("hello hw");
        example::EchoResponse *response = new example::EchoResponse();
        brpc::Controller *cntl = new brpc::Controller();
        stub.Echo(cntl, &request, response, nullptr);
        if (cntl->Failed() == true) {
            std::cout << "请求失败: " << cntl->ErrorText() << std::endl;
            delete cntl;
            delete response;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;

        }
        std::cout << "响应：" << response->message() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
