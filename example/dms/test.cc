#include <cstdlib>
#include <iostream>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/core/CommonRequest.h>
#include <alibabacloud/core/CommonClient.h>
#include <alibabacloud/core/CommonResponse.h>
#include <alibabacloud/open_api.hpp>
#include <alibabacloud/dypnsapi20170525.hpp>


using namespace std;
using namespace wei_im;

int main( int argc, char** argv )
{
    // AlibabaCloud::InitializeSdk();
    // AlibabaCloud::ClientConfiguration configuration( "cn-chengdu" );
    // // specify timeout when create client.
    // configuration.setConnectTimeout(1500);
    // configuration.setReadTimeout(4000);
    // std::string access_key_id = "LTAI5t6NF7vt499UeqYX6LB9";
    // std::string access_key_secret = "5hx1qvpXHDKfQDk73aJs6j53Q8KcF2";
    // AlibabaCloud::Credentials credential(access_key_id, access_key_secret);
    // /* use STS Token
    // credential.setSessionToken( getenv("ALIBABA_CLOUD_SECURITY_TOKEN") );
    // */
    // AlibabaCloud::CommonClient client( credential, configuration );
    // AlibabaCloud::CommonRequest request(AlibabaCloud::CommonRequest::RequestPattern::RpcPattern);
    // request.setHttpMethod(AlibabaCloud::HttpRequest::Method::Post);
    // request.setDomain("dysmsapi.aliyuncs.com");
    // request.setVersion("2017-05-25");
    // request.setQueryParameter("Action", "SendSms");
    // request.setQueryParameter("SignName", "bitejiuyeke");
    // request.setQueryParameter("TemplateCode", "SMS_465324787");
    // request.setQueryParameter("PhoneNumbers", "15929917272");
    // request.setQueryParameter("TemplateParam", "{\"code\":\"1234\"}");

    // auto response = client.commonResponse(request);
    // if (response.isSuccess()) {
    //     printf("request success.\n");
    //     printf("result: %s\n", response.result().payload().c_str());
    // } else {
    //     printf("error: %s\n", response.error().errorMessage().c_str());
    //     printf("request id: %s\n", response.error().requestId().c_str());
    // }

    // AlibabaCloud::ShutdownSdk();
    // return 0;


    Alibabacloud_OpenApi::Config config;
    // Please ensure that the environment variables ALIBABA_CLOUD_ACCESS_KEY_ID and ALIBABA_CLOUD_ACCESS_KEY_SECRET are set.
    std::string accessKeyId = "LTAI5tSEtdmRhGrMKRxMcGDT";
    config.accessKeyId = make_shared<string>(accessKeyId);
    std::string accessKeySecret ="PLkerQWFfewCcbRzE4d34Y2S1cTHl4";
    config.accessKeySecret = make_shared<string>(accessKeySecret);
    config.regionId = make_shared<string>("<RegionId>");
    config.endpoint = make_shared<string>("dypnsapi.aliyuncs.com");

    Alibabacloud_Dypnsapi20170525::Client client(make_shared<Alibabacloud_OpenApi::Config>(config));

    Alibabacloud_Dypnsapi20170525::CreateVerifySchemeRequest request;

    /* 该参数值为假设值，请您根据实际情况进行填写 */
    request.schemeName = "your_value";

    /* 该参数值为假设值，请您根据实际情况进行填写 */
    request.appName = "your_value";

    Alibabacloud_Dypnsapi20170525::CreateVerifySchemeResponse response = client.createVerifyScheme(
        make_shared<Alibabacloud_Dypnsapi20170525::CreateVerifySchemeRequest>(request)
    );

    printf("%s", response.Body.requestId.c_str());
}