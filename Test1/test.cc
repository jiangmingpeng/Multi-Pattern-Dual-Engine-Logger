#include <iostream>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

// #define _GNU_SOURCE
// #define LOG(fmt,...) printf("[%s:%d]" fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)//fmt会自动拼接字符串
// #define LOG(fmt,...) printf("[%s:%d]" fmt,__FILE__,__LINE__,__VA_ARGS__)
#define LOG(fmt, ...) printf("[%d] [%s:%d]" fmt "\n", getpid(), __FILE__, __LINE__, ##__VA_ARGS__) // fmt会自动拼接字符串

// C语言风格
//  void PrintNum(int num,...)//第一个参数为后面的参数个数进行摸底
//  {
//      va_list vl;//创建对象
//      va_start(vl,num);//关联对象
//      for(int i = 0;i<num;i++)
//      {
//          int n = va_arg(vl,int);//每次调用读取type个字节 指针自动往下走
//          std::cout<<n<<std::endl;
//      }
//      va_end(vl);//释放对象
//  }
// void my_printf(const char *fmt, ...)
// {
//     char *res;//格式化对象
//     va_list vl;
//     va_start(vl, fmt);
//     int len = vasprintf(&res, fmt, vl);//第一个参数用俩开辟空间 格式化的
//     va_end(vl);
//     std::cout << res << std::endl;
//     free(res);
// }

// C++风格
int i = 1;
void xprintf()//这里要特化一个无参的 这个参数包的传法是每次往下传都少一个(原因其实是因为原参数是有1+n 构成的所以每次往后走都成1+n式 把n->1+n-1(总数依次减一))
{
    std::cout<<"这是打印结束语句..."<<std::endl;
}

template <class T, class... Args> // 右边是一个参数包
void xprintf(const T &value, Args&&... arg)
{
    std::cout<<"这是第"<<i++<<"个参数"<<value<<std::endl;
    if(sizeof...(arg)>0)
    {
        xprintf(std::forward<Args>(arg)...);
    }
    else
    {
        xprintf();
    }
}

int main()
{
    int num = 0;
    LOG("我是一个日志输出 数字");
    LOG("我是一个日志输出 数字%d", num);
    xprintf(5,4,3,2,1);
    // my_printf("%s-%d","我是数字:",666);
    // PrintNum(2,88,98);
    // LOG("我是一个日志输出 数字%"/* 这中间添加了一个逗号,不用#消除就会出现语法报错 无法打印不传参加内容*/);
}
