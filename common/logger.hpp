#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>

// 对spd的二次封装
std::shared_ptr<spdlog::logger> g_logger;

// params mode：运行模式  filename 保存的文件名
void init_logger(bool mode, const std::string& filename, int32_t level)
{
    if(mode){
        g_logger = spdlog::basic_logger_mt("file-logger",filename);
        g_logger->set_level(spdlog::level::level_enum::trace);
        g_logger->flush_on(spdlog::level::level_enum::trace);
    }else{
        g_logger = spdlog::stdout_color_mt("default-logger");
        g_logger->set_level((spdlog::level::level_enum)level);
        g_logger->flush_on((spdlog::level::level_enum)level);
    }
    // g_logger->set_pattern("[%n][%H:%M:%S][%t][%-8l]%v"); // 可加可不加
}

#define LOG_TRACE(format, ...) g_logger->trace(std::string("[{}:{}]")+format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) g_logger->debug(std::string("[{}:{}]")+format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(format, ...) g_logger->info(std::string("[{}:{}]")+format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(format, ...) g_logger->warn(std::string("[{}:{}]")+format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) g_logger->error(std::string("[{}:{}]")+format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) g_logger->critical(std::string("[{}:{}]")+format, __FILE__, __LINE__, ##__VA_ARGS__)
