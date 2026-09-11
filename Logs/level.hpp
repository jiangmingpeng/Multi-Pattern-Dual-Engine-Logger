#pragma once
#include <iostream> 
#include <string>

namespace MPLog
{
    class Level
    {
    public:
        enum class value//等级信息
        {
            UNKNOW = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            OFF
        };

        static std::string Level2String(Level::value level)//确保在外面也是这么传参数
        {
            switch (level)
            {
            case value::DEBUG:
                return "DEBUG";
            case value::INFO:
                return "INFO";
            case value::WARN:
                return "WARM";
            case value::ERROR:
                return "ERROR";
            case value::FATAL:
                return "FATAL";
            case value::OFF:
                return "OFF";
            }
            return "UNKNOW";          
       }
    };
}