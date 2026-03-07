#include <iostream>
#include <gflags/gflags.h>
#include "logger.hpp"

DEFINE_bool(run_mode, false, "程序的运行模式，--false(开发模式)， --true(发布模式)");
DEFINE_string(file_name, "", "发布模式下的文件名");
DEFINE_int32(level, 0, "指定的日志等级");


int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_file_name, FLAGS_level);

    LOG_TRACE("你好：{}","帅威");
    LOG_DEBUG("你好：{}","帅威");
    LOG_INFO("你好：{}","帅威");
    LOG_WARN("你好：{}","帅威");
    LOG_FATAL("你好：{}","帅威");

    return 0;
}