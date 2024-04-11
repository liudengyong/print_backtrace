# print_backtrace

- C++版本的PrintStacktrace；
- 可以在C++代码中打印堆栈信息；
- 可以用于无调试器调试；
- 可打印输出到控制台；
- 可打印输出到文件；

demo代码：

```C++
/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-11 13:45:42
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-11 18:18:23
 * @ Description: PrinitBacktrace demo
 */

#include "PrintBacktrace.h"

#include <fstream>
#include <signal.h>
#include <ctime>

namespace ns {
    class A {
        public:
        void test() {}
    };
}

// 测试打印堆栈信息
void testPrintBacktrace() {
    // 获取可读类型名称
    ns::A a;
    std::string name = dy::get_type_name(typeid(a));

    std::cout << "Type name " << name << std::endl;

    // 主动调用
    dy::print_backtrace(std::cout);

    // 制造SegmentFault，打印crash时的堆栈
    char* tmp = nullptr;
    std::cout << tmp[0] << std::endl;
}

// SIGSEGV/SIGABRT 信号处理程序
static void sigHandler(int sig) {
    std::string fileName = dy::gen_filename_with_cur_ts();
    std::ofstream file(fileName);
    std::cout << "Print backtrace to file: " << fileName << std::endl;
    if (file.is_open()) {
        dy::print_backtrace(file);
        file.close();
    }

    exit(sig);
}

// 注册 SIGSEGV/SIGABRT 信号处理程序
static void registerSigHandler() {
    struct sigaction action{};
    action.sa_handler = sigHandler;
    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGABRT, &action, NULL);
}

int main(int32_t argc, char const *argv[]) {
    // 注册 SIGSEGV/SIGABRT 信号处理程序
    registerSigHandler();

    // 测试打印堆栈信息
    testPrintBacktrace();

    return 0;
}
```
