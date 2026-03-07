#include "../common/etcd.hpp"
#include <gflags/gflags.h>
#include <thread>

DEFINE_bool(run_mode, false, "程序的运行模式，false-调试； true-发布；");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志输出等级");

DEFINE_string(etcd_host, "http://127.0.0.1:2379", "服务注册中心地址");
DEFINE_string(base_service, "/service", "服务监控根目录");
DEFINE_string(instance_name, "/friend/instance", "当前实例名称");
DEFINE_string(access_name, "127.0.0.1:9090", "当前实例外部访问地址");


int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    Registry::ptr rclient = std::make_shared<Registry>(FLAGS_etcd_host);
    rclient->serviceRegistry(FLAGS_base_service + FLAGS_instance_name,FLAGS_access_name);

    std::this_thread::sleep_for(chrono::seconds(600));
    return 0;
}