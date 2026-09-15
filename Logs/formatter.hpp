#pragma once
#include "level.hpp"
#include "message.hpp"
#include "util.hpp"
#include <ctime>
#include <vector>
#include <cassert>
#include <tuple>

namespace MPLog
{
    // 这里由FormatItem 所出来的派生类 其作用是把对日志消息类对象 进行各个部分解析操作
    // 然后把解析出来的内容用流放到os对象里 等待调用此派生类方法时 进行输出得到目标
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
            time_t t = msg._time/1000;//注意time_t 记得是秒 这里在获取的时候是毫秒所以要注意
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
    //-----------------------------------------------------------------------
    // 系统的默认⽇志输出格式：%d{%H:%M:%S}%T[%t]%T[%p]%T[%c]%T%f:%l%T%m%n
    class Formatter
    {

    public:
        Formatter(const std::string &pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p] %m%n")
            : _pattern(pattern)
        {
            assert(parsePattern());
        }

        const std::string pattern()
        {
            return _pattern;
        }

        // 下面是两个获取日志消息消息的核心内容(不关心非核心内容)
        std::string format(const Message &msg)
        {
            std::stringstream str;
            for (auto &item : _items)
            {
                // std::cout<<"Debug";
                item->format(str, msg);
            }
            return str.str(); // 注意是stringstream对象的成员函数 是str()来实现对string的转化
        }
        std::ostream &format(std::ostream &os, const Message &msg)
        {
            for (auto &item : _items)
            {
                item->format(os, msg);
            }
            return os;
        }

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
        // 这个函数应该是根据传值被调用的 在条件下 然后再进行push_back()到items成员变量里
        FormatItem::Ptr CreateItem(const std::string &formattype, const std::string &submit)
        {

            // 这里其实就是根据参数(%后的字段)进行构造不同的
            if (formattype == "m")
                return std::make_shared<MsgFormatItem>(submit);
            if (formattype == "p")
                return std::make_shared<LevelFormatItem>(submit);
            if (formattype == "c")
                return std::make_shared<NameFormatItem>(submit);
            if (formattype == "t")
                return std::make_shared<ThreadFormatItem>(submit);
            if (formattype == "d")
                return std::make_shared<TimeFormatItem>(submit);
            if (formattype == "f")
                return std::make_shared<CFileFormatItem>(submit);
            if (formattype == "l")
                return std::make_shared<CLineFormatItem>(submit);
            if (formattype == "T")
                return std::make_shared<TabFormatItem>(submit);
            if (formattype == "n")
                return std::make_shared<NLineFormatItem>(submit);
            return FormatItem::Ptr(); // 都不符合直接放回空对象指针
            // if()  return std::make_shared<OtherFormatItem>();
        }

        enum ItemType
        {
            RawText,
            FormatSpec
        }; // 这里枚举不用 class 即可充当全局变量来使用  否则需要有一个对应类型的对象参数来实现
        //_pattern = [%d{%H:%M:%S}][%t][%p][%c][%f:%l] %m%n
        bool parsePattern() // 对输出串进行格式化的核心函数
        {
            _items.clear();
            bool sub_format_error = false;
            std::vector<std::tuple<std::string, std::string, int>> arr; // 用来存储每一段格式化的 格式字符 子字符 类型
            std::string _key, _val, _row;                               // 用来存储格式化字符、子格式、普通字符
            int size = _pattern.size();
            int pos = 0;
            while (pos < size)
            {
                if (_pattern[pos] != '%')
                {
                    _row.push_back(_pattern[pos++]);
                    continue;
                }
                if (pos + 1 < _pattern.size() && _pattern[pos + 1] == '%')
                {
                    _row.push_back(_pattern[pos]);
                    pos += 2;
                    continue;
                }
                // 到这里来了 就说明已经完成了普通字符串的解析
                if (!_row.empty())
                {
                    arr.push_back(std::make_tuple(_row, "", ItemType::RawText)); // 普通字符串和它的子串
                    _row.clear();
                }
                // 当前位置是%字符位置
                pos += 1; // pos指向格式化字符位置
                if (pos < _pattern.size() && isalpha(_pattern[pos]))
                {
                    _key = _pattern[pos]; // 保存格式化字符
                }
                else
                {
                    std::cout << &_pattern[pos - 1] << "位置附近格式错误！\n"; // 意味着%后面乱跟了一个数字或其他字符
                    return false;
                }
                pos += 1; // 判断完是否有效%后的字符后 开始检测有没有子串+1有的话就跑到了{上
                if (pos < _pattern.size() && _pattern[pos] == '{')
                {
                    sub_format_error = true;
                    pos += 1; // pos指向花括号下一个字符处
                    while (pos < _pattern.size())
                    {
                        if (_pattern[pos] == '}')
                        {
                            sub_format_error = false;
                            pos += 1; // 让pos指向}的下一个字符处
                            break;
                        }
                        _val.push_back(_pattern[pos++]); // 把格式化的字段都拿到
                    }
                }
                arr.push_back(std::make_tuple(_key, _val, ItemType::FormatSpec)); // 格式化字符和它的子串
                _key.clear();
                _val.clear();
            }
            if (sub_format_error)
            {
                std::cout << "{}出现对应问题\n";
                return false;
            }

            //从下面开始来进行对该类的成员的填充  以便配合前面两个关于流的两个接口函数
            if (_row.empty() == false)
                arr.push_back(std::make_tuple(_row, "", ItemType::RawText));
            if (_key.empty() == false)
                arr.push_back(std::make_tuple(_key, _val, ItemType::FormatSpec));
            for (auto &it : arr)//对每一个元组进行成员提取
            {
                if (std::get<2>(it) == ItemType::RawText)//判断是什么字符类型 普通还是格式化的 --是普通的
                {
                    FormatItem::Ptr fi(new OtherFormatItem(std::get<0>(it)));
                    _items.push_back(fi);//这个函数的前面的所有工作都是为了这一步 服务的
                }
                else//是格式化的
                {
                    FormatItem::Ptr fi = CreateItem(std::get<0>(it), std::get<1>(it));//关键字+(可能存在的)子串
                    if (fi.get() == nullptr)//创建失败的判断
                    {
                        std::cout << "没有对应的格式化字符: %" << std::get<0>(it) << std::endl;
                        return false;
                    }
                    _items.push_back(fi);//这个函数的前面的所有工作都是为了这一步 服务的
                }
            }
            return true;
        }

    private:
        std::string _pattern;
        std::vector<FormatItem::Ptr> _items; // 这个vector的目的是把 各个派生类对象存起来
    };
}
