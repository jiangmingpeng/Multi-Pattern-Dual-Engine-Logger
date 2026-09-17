#include "util.hpp"
#include "formatter.hpp"
#include "sink.hpp"
#include <cstring>

int main()
{
    MPLog::Message msg("stdout", "test.cc", 115, MPLog::Level::value::INFO, "这是一条测试代码");
    MPLog::Formatter ft;
    // ft.parsePattern(); 这个在构造函数判断的时候就已经初始化过一次了 这里重复调用了 导致_items.size()会翻倍增长
    // std::cout << "Before format call" << std::endl;
    std::string message = ft.format(msg);
    std::cout<<message;
    // std::cout << "After format call" << std::endl;


    MPLog::SinkFactory sf;
    // MPLog::LogSink::Ptr lp = sf.create<MPLog::StdoutSink>();
    MPLog::LogSink::Ptr lp = sf.create<MPLog::FileSink>("test_one");//------这里 还有点问题
    const char data[] = "This is a test msg!!!!!";//改成const char* 也可以 后面就改strlen(不包括\0)
    lp->log(data,sizeof(data));
    return 0;
}