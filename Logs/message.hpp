#pragma once
#include "level.hpp"
#include "util.hpp"
#include <thread>

namespace MPLog
{
    /* [2026-09-11] [tid] [main.c:115] [DUBUG] - "This is an logger message.."*/
    class Message
    {
    public:
        using Ptr = std::shared_ptr<Message>;

        Message() {}//默认构造
        Message(const std::string logname,const std::string filename, const int line
                ,Level::value level,std::string message)
        : _time(util::Date::GetNowTime())
        , _tid(std::this_thread::get_id())
        , _logname(logname)
        , _filename(filename)
        , _line(line)
        , _level(level)
        , _message(message)
        {   }

        time_t _time; // time_t和前面的获取时间可以匹配的上
        std::thread::id _tid;
        std::string _logname;//日志器名称
        std::string _filename;
        int _line;
        Level::value _level;
        std::string _message;
    };
}