#include <json/json.h>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

bool serialize(const Json::Value& val,std::string &dst)
{
    Json::StreamWriterBuilder swb;
    std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
    std::stringstream ss;
    int ret = sw->write(val,&ss);
    if(ret != 0)
    {
        std::cout<<"Json 反序列失败"<<std::endl;
        return false;
    }
    dst = ss.str();
    return true;
}
int main()
{
    char name[] = "shuawei";
    int age = 11;
    float score[3] = {22,89.5,100};
    Json::Value stu;
    stu["姓名"] = name;
    stu["年龄"] = age;
    stu["成绩"].append(score[0]);
    stu["成绩"].append(score[1]);
    stu["成绩"].append(score[2]);
    std::string stu_str;
    bool ret = serialize(stu,stu_str);
    std::cout<<stu_str<<std::endl;
    if(ret == false) return -1;

}
