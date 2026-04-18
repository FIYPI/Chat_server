#include <cstdlib>
#include <iostream>
#include "../../common/dms_client.hpp"
#include <curl/curl.h>

using namespace std;
// using namespace wei_im;


// // 发送验证码的函数
// bool callMailProxy(const std::string& email, const std::string& code) {
//     CURL* curl = curl_easy_init();
//     if (!curl) return false;

//     std::string url = "http://127.0.0.1:8888/send_email";
//     // 构造 JSON 数据
//     std::string jsonData = "{\"email\":\"" + email + "\", \"code\":\"" + code + "\"}";

//     struct curl_slist* headers = NULL;
//     headers = curl_slist_append(headers, "Content-Type: application/json");

//     curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
//     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

//     CURLcode res = curl_easy_perform(curl);
//     curl_easy_cleanup(curl);

//     return (res == CURLE_OK);
// }

int main( int argc, char** argv )
{
    // std::string userEmail = "17780747710@163.com";
    // std::string verifyCode = "668822"; // 实际应用中应由随机数生成器产生

    // if (callMailProxy(userEmail, verifyCode)) {
    //     std::cout << "验证码已成功发送至邮箱！" << std::endl;
    // } else {
    //     std::cerr << "发送失败，请检查 Python 中转服务是否启动。" << std::endl;
    // }
    auto dms_client = std::make_shared<wei_im::DMSClient>();
    dms_client->send("17780747710@163.com", "666666");
    return 0;    
}