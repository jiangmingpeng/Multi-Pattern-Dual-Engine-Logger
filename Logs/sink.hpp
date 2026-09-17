#pragma once
#include "formatter.hpp"
#include "message.hpp"
#include <fstream>


/*
    日志可落地的方向
    1.标准输出:StdoutSink
    2.固定文件:FileSink
    3.滚动文件:RollSink
*/

namespace MPLog
{
    class LogSink        //落地基类
    {
    public:
        using Ptr =  std::shared_ptr<LogSink>;
        LogSink (){}
        virtual ~LogSink() = default;//这个析构进行多态的重写 实质上是让派生类都使用默认析构
        virtual void log(const char* data,size_t len) = 0;//重写这个日志主内容的写入
    };

    class StdoutSink : public LogSink
    {   
    public:
        using Ptr = std::shared_ptr<StdoutSink>;
        StdoutSink() = default;
        void log(const char* data,size_t len) override
        {
            std::cout.write(data,len);//以原始字符串的方式 输出到用户态缓冲区
        }
    };

    class FileSink : public LogSink
    {
    public://文件落地 首先得有文件
        using Ptr = std::shared_ptr<FileSink>;
        FileSink(const std::string& filename)//这里给的按道理来讲是自己传 往哪进行日志落地
            :_filename(filename)
        {
            util::file::CreateDir(util::file::GetFilePath(filename));//检查是否有 没有则进行创建
            _ofs.open(filename,std::ios::binary|std::ios::app);//然后把要写入的文件给打开
            assert(_ofs.is_open());
        }
        void log (const char* data,size_t len) override
        {   
            _ofs.write(data,len);//把内容写进了fstrean自己维护的用户态缓冲区了 也可以手动直接刷到内核缓冲区(即近似看成刷入磁盘 关机重启后还在)
            if(!_ofs.good())//检查上一次的流是否出错
            {
                std::cout<<"内容写入失败"<<std::endl;
            }
        }
    private:
        std::string _filename;
        std::ofstream _ofs;
        
    };

    class RollSink : public LogSink
    {
    public:
        using Ptr = std::shared_ptr<RollSink>;
        RollSink(const std::string& basename,size_t max_size)
        :_basename(basename)
        ,_max_fsize(max_size)
        ,_cur_fsize(0)
        {
            util::file::CreateDir(util::file::GetFilePath(basename));
        }
        void log(const char* data,size_t len) override
        {
            InitLogFile();
            _ofs.write(data,len);
            if(!_ofs.good())
            {
                std::cout<<"内容写入文件失败!!!"<<std::endl;
            }
            _cur_fsize+=len;

        }
    private:
        void InitLogFile()
        {
            if(!_ofs.is_open()||_cur_fsize>=_max_fsize)
            {
                _ofs.close();
                std::string newfile = CreateFilename();
                _ofs.open(newfile,std::ios::binary|std::ios::app);
                assert(!_ofs.is_open());
                _cur_fsize = 0;//每次创建新文件的时候要把当前的容量大小清零
            }
        }
        std::string CreateFilename()
        {
            time_t tm = util::Date::GetNowTime()/1000;
            struct tm t;
            std::stringstream ss;
            localtime_r(&tm,&t);
            ss<<_basename;
            ss<<t.tm_year+1900;
            ss<<t.tm_mon+1;
            ss<<t.tm_mday;
            ss<<t.tm_hour;
            ss<<t.tm_min;
            ss<<t.tm_sec;
            ss<<".log.txt";
            return ss.str();
        }
    private:

        std::string _basename;
        std::ofstream _ofs;
        size_t _max_fsize;
        size_t _cur_fsize;
    };

    class SinkFactory//简单工厂的设计模式
    {
    public:
        template<typename SinkType,typename ...Args>
        static LogSink::Ptr create(Args&& ... args) //创建基类指针 来指向派生类 参数用万能引用
        {
            return  std::make_shared<SinkType>(std::forward<Args>(args)...);
        }
    };
};



