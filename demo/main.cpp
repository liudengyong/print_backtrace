/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-11 13:45:42
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-12 09:57:41
 * @ Description: PrinitBacktrace demo
 */

#include "PrintBacktrace.h"

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

int main(int32_t argc, char const *argv[]) {
    // 注册 SIGSEGV/SIGABRT 信号处理程序
    dy::register_sig_handler("logs", {SIGSEGV, SIGABRT});

    // 测试打印堆栈信息
    testPrintBacktrace();

    return 0;
}