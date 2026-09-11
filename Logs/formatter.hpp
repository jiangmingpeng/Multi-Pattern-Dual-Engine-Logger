#pragma once
#include "level.hpp"
#include "message.hpp"
#include "util.hpp"
#include <ctime>
#include <vector>
#include <cassert>

namespace MPLog
{

    class FormatItem // 作为基类---这里的格式化是向外提供的一个格式接口 每个派生类
    {
    public:
        using Ptr = std::shared_ptr<FormatItem>;
        virtual ~FormatItem() {}
        virtual void format(std::ostream &os, const Message &msg) = 0;
    };

    class MsgFormatItem : public FormatItem
    {
    public:
        MsgFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const Message &msg) // 编译报错：流不可拷贝
        {
            os << msg._message;
        }
    };

    class LevelFormatItem : public FormatItem
    {
    public:
        LevelFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const Message &msg)
        {
            os << Level::Level2String(msg._level);
        }
    };
    class NameFormatItem : public FormatItem
    {
    public:
        NameFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const Message &msg)
        {
            os << msg._logname;
        }
    };
    class ThreadFormatItem : public FormatItem
    {
    public:
        ThreadFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const Message &msg)
        {
            os << msg._tid;
        }
    };

    class TimeFormatItem : public FormatItem
    {
    private:
        std::string _format;

    public:
        TimeFormatItem(const std::string &format = "%H:%M:%S") : _format(format)
        {
            if (format.empty())
                _format = "%H:%M:%S";
        }
        virtual void format(std::ostream &os, const Message &msg)
        {
            time_t t = msg._time;
            struct tm lt;
            localtime_r(&t, &lt); // 输入输出型参数 出来之后结构体tm的成员就已经被时间戳转化为年月日等一系列时间
            char tmp[128];
            strftime(tmp, 127, _format.c_str(), &lt); // 把时间段的内容格式化输出到temp
            os << tmp;
        }
    };
    class CFileFormatItem : public FormatItem
    {
    public:
        CFileFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const Message &msg)
        {
            os << msg._filename;
        }
    };
    class CLineFormatItem : public FormatItem
    {
    public:
        CLineFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const Message &msg)
        {
            os << msg._line;
        }
    };
    class TabFormatItem : public FormatItem
    {
    public:
        TabFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const Message &msg)
        {
            os << "\t";
        }
    };
    class NLineFormatItem : public FormatItem
    {
    public:
        NLineFormatItem(const std::string &str = "") {}
        virtual void format(std::ostream &os, const Message &msg)
        {
            os << "\n";
        }
    };
    class OtherFormatItem : public FormatItem
    {
    private:
        std::string _str;

    public:
        OtherFormatItem(const std::string &str = "") : _str(str) {}
        virtual void format(std::ostream &os, const Message &msg)
        {
            os << _str;
        }
    };

    class Formatter
    {
    /*
        %d ⽇期
        %T 缩进
        %t 线程id
        %p ⽇志级别
        %c ⽇志器名称
        %f ⽂件名
        %l ⾏号
        %m ⽇志消息
        %n 换⾏
    */
    public:
        Formatter(const std::string& pattern = "[%d{%H:%M:%S}][%t][%p][%c][%f:%l] %m%n")
        :_pattern(pattern)
        {
        //    assert(); 
        }

    private:
        std::string _pattern;
        std::vector<FormatItem::Ptr> _items;

    };
}
