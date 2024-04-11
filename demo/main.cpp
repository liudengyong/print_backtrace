/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-11 13:45:42
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-11 17:46:47
 * @ Description: PrinitBacktrace demo
 */

#include "PrintBacktrace.h"

#include <fstream>
#include <signal.h>
#include <ctime>

// 测试打印堆栈信息
void testPrintBacktrace() {
    // 主动调用
    dy::print_backtrace(std::cout);

    // 制造SegmentFault，打印crash时的堆栈
    char* tmp = nullptr;
    std::cout << tmp[0] << std::endl;
}

// SIGSEGV/SIGABRT 信号处理程序
static void sigHandler(int sig) {
    time_t t = std::time(nullptr);
	struct tm* now = std::localtime(&t);
	char formatedTime[50];
	strftime(formatedTime, sizeof(formatedTime), "%Y-%m-%d_%H-%M-%S.txt", now);

    std::string fileName = "backtrace_" + std::to_string(sig) + "_" + formatedTime;

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