#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <curl/curl.h>
#include "logger.hpp"

namespace wei_im{
class DMSClient {
public:
    using ptr = std::shared_ptr<DMSClient>;

    // 构造函数：
    DMSClient(const std::string &proxy_url = "http://127.0.0.1:8888/send_email") 
        : _proxy_url(proxy_url) {
        // 全局初始化 curl (如果在项目其他地方已初始化，这里可省略)
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~DMSClient() {
        // 全局清理
        curl_global_cleanup();
    }

    /**
     * @brief 发送邮件验证码
     * @param email 接收者邮箱
     * @param code  验证码数字
     */
    bool send(const std::string &email, const std::string &code) {
        CURL* curl = curl_easy_init();
        if (!curl) {
            // LOG_ERROR("Curl 初始化失败");
            return false;
        }

        // 1. 构造 JSON 负载
        // 注意：原先是 phone，现在改为 email
        std::string jsonData = "{\"email\":\"" + email + "\", \"code\":\"" + code + "\"}";

        // 2. 设置 HTTP 头部
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // 3. 配置 CURL 选项
        curl_easy_setopt(curl, CURLOPT_URL, _proxy_url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2L); // 2秒连接超时
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);        // 5秒请求超时

        // 4. 执行请求
        CURLcode res = curl_easy_perform(curl);
        
        long response_code = 0;
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        }

        // 5. 清理资源
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK || response_code != 200) {
            LOG_ERROR("邮件验证码请求失败：curl_code={}, http_code={}", (int)res, response_code);
            return false;
        }

        return true;
    }

private:
    std::string _proxy_url;
};
}