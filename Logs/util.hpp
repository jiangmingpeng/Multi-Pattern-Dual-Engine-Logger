#pragma once
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <filesystem>

namespace MPLog
{
    namespace util // 实用类设计
    {
        class Date // 获取当前时间(ms)
        {
        public:
            static uint64_t GetNowTime()
            {
                struct timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);

                uint64_t ns = (uint64_t)ts.tv_nsec;
                uint64_t se = (uint64_t)ts.tv_sec;

                // 返回毫秒 秒*1000 + 纳秒/1e6
                return se * 1000 + ns / 1000000;
            }
        };
        /*  判断文件是否存在
            获取文件的所在目录路径
            创建目录*/
        class file
        {
        public:
            static bool Exit(const std::string &filename) // 把文件名传进来判断
            {
                // access(filename,F_OK);这个第一个参数是char* 开销最小
                struct stat st;
                return stat(filename.c_str(), &st) == 0; // string 与 char* 的之间转化 用to_string()  c_str()
            }
            static std::string GetFilePath(const std::string &name) // 获取当前文件的父级目录
            {
                if (name.empty())
                    return ".";
                std::filesystem::path parent = std::filesystem::path(name).parent_path();//库函数本身就已经具备跨平台性
                if (parent.empty())
                    return ".";
                return parent.string(); // "logs/2026" 而非 "logs/2026/" 如果需要的话可以+=上去
            }
            static bool CreateDir(const std::string& path)
            {
                namespace fs = std::filesystem;//命名空间重命名
                std::error_code ec;
                fs::create_directories(path,ec);//第二个参数是引用 输入输出型参数 
                if(ec)
                {
                    ec.message();
                    return false;
                }
                return true;
            }
        };

    }
}