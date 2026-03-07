#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <iostream>
int main()
{
    // 设置全局刷新
    // 每秒刷新
    spdlog::flush_every(std::chrono::seconds(1));
    // 遇到debug以上刷新
    spdlog::flush_on(spdlog::level::level_enum::debug);
    // 设置全局的日志输出等级
    spdlog::set_level(spdlog::level::level_enum::debug);

    // 设置同步日志器
    auto logger = spdlog::stdout_color_mt<spdlog::async_factory>("default-logger");
    // auto filelogger = spdlog::basic_logger_mt("file-logger","test.log");
    // 日志器名称
    logger->set_pattern("[%n][%H:%M:%S][%t][%-8l] %v");

    logger->trace("你好！{}","帅威");
    logger->debug("你好！{}","帅威");
    logger->info("你好！{}","帅威");
    logger->warn("你好！{}","帅威");
    logger->critical("你好！{}","帅威");
    std::cout<<"hahah\n";

    return 0;
}