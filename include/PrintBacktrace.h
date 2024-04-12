/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-11 13:49:56
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-12 11:39:25
 * @ Description: Print backtrace info for debug
 */

#ifndef __DY_PRINT_BACKTRACE__
#define __DY_PRINT_BACKTRACE__

#include "PrintBacktraceUtils.h"

#include <iostream>
#include <signal.h>
#include <vector>

namespace dy {
    /// @brief 打印当前调用的堆栈符号信息
    void print_backtrace(std::ostream& of, const int max_frame = 128, const char* build_time = __TIME__, const char* build_date = __DATE__);

    /// @brief 注册 SIGSEGV/SIGABRT 信号处理程序
    bool register_sig_handler(std::string log_dir, std::vector<int> signals = {SIGSEGV, SIGABRT});
}

#endif
