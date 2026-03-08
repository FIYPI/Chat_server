#include "main.pb.h"
#include <brpc/server.h>
#include <butil/logging.h>
#include <gflags/gflags.h>
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/callback.h>
#include <json2pb/pb_to_json.h>

DEFINE_int32(listen_port, 8000, "服务器地址信息");
DEFINE_int32(idle_timeout_s, -1, "空闲连接超时关闭时间：默认-1表示不关闭");
DEFINE_int32(thread_count, 3, "服务器启动线程数量");
class EchoServiceImpl : public example::EchoService {
public:
  EchoServiceImpl() {}
  ~EchoServiceImpl() {}
  void Echo(google::protobuf::RpcController *cntl_base,
            const example::EchoRequest *request,
            example::EchoResponse *response, google::protobuf::Closure *done) {
    brpc::ClosureGuard done_guard(done);
    brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);

    cntl->set_after_rpc_resp_fn(
        std::bind(&EchoServiceImpl::CallAfterRpc, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3));
    std::cout << " 请求内容--" << request->message() << std::endl;
    response->set_message(request->message() + "Hello");
  }

  static void CallAfterRpc(brpc::Controller *cntl,
                           const google::protobuf::Message *req,
                           const google::protobuf::Message *res) {
    std::string req_str;
    std::string res_str;
    json2pb::ProtoMessageToJson(*req, &req_str, NULL);
    json2pb::ProtoMessageToJson(*res, &res_str, NULL);
    std::cout << "req:" << req_str << std::endl;
    std::cout << "res:" << res_str << std::endl;
  }
};

int main(int argc, char *argv[]) {
  logging::LoggingSettings log_setting;
  log_setting.logging_dest = logging::LoggingDestination::LOG_TO_NONE;
  logging::InitLogging(log_setting);
  google::ParseCommandLineFlags(&argc, &argv, true);

  // 定义服务器
  brpc::Server server;
  std::cout << "定义服务器---" << std::endl;

  // 创建服务对象
  EchoServiceImpl echoservice_impl;

  // 将服务添加到服务器
  if (server.AddService(&echoservice_impl, brpc::SERVER_DOESNT_OWN_SERVICE) !=
      0) {
    std::cout << "add service failed!\n";
    return -1;
  }

  // 开始运行服务器
  brpc::ServerOptions options;
  options.idle_timeout_sec = FLAGS_idle_timeout_s;
  options.num_threads = FLAGS_thread_count;
  if (server.Start(FLAGS_listen_port, &options) != 0) {
    std::cout << "Fail to start EchoServer";
    return -1;
  }
  std::cout << "运行服务器---" << std::endl;

  server.RunUntilAskedToQuit();
  return 0;
}