#include "main.pb.h"
#include <brpc/channel.h>
#include <brpc/controller.h>
#include <brpc/server.h>
#include <butil/logging.h>
#include <butil/time.h>
#include <cmath>
#include <gflags/gflags.h>
#include <google/protobuf/service.h>
#include <iostream>

DEFINE_string(protocol, "baidu_std", "通信协议类型，默认使用brpc自定制协议");
DEFINE_string(server_host, "127.0.0.1:8000", "服务器地址信息");
DEFINE_int32(timeout_ms, 500, "Rpc请求超时时间-毫秒");
DEFINE_int32(max_retry, 3, "请求重试次数");

// void callback(google::protobuf::RpcController *cntl,
//               const example::EchoResponse *response) {
//                 std::cout << "收到相应" << response->message << std::endl;
//                 delete response;
//                 delete cntl;
//               }
int main(int argc, char *argv[]) {
  // 解析命令行
  google::ParseCommandLineFlags(&argc, &argv, true);
  brpc::Channel channel;

  brpc::ChannelOptions options;
  options.protocol = FLAGS_protocol;
  options.timeout_ms = FLAGS_timeout_ms;
  options.max_retry = FLAGS_max_retry;
  if (channel.Init(FLAGS_server_host.c_str(), &options) != 0) {
    LOG(ERROR) << "Fail to initialize channel";
    return -1;
  }
  example::EchoService_Stub stub(&channel);

  // 创建请求、响应、控制对象
  example::EchoRequest request;
  example::EchoResponse response;
  brpc::Controller cntl;
  // 构造请求响应
  request.set_message("hello world");

  // 由于“done”（最后一个参数）为NULL，表示阻塞等待响应
  stub.Echo(&cntl, &request, &response, NULL);
  if (cntl.Failed()) {
    std::cout << "请求失败: " << cntl.ErrorText() << std::endl;
    return -1;
  }
  std::cout << "响应：" << response.message() << std::endl;
  return 0;
}