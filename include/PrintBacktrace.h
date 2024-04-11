/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-11 13:49:56
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-11 16:51:21
 * @ Description: Print backtrace info for debug
 */

#ifndef __DY_PRINT_BACKTRACE__
#define __DY_PRINT_BACKTRACE__

#include <iostream>

namespace dy {
    // 获取当前可读性类型名称
    std::string get_type_name(const std::type_info& info);

    /// 打印当前调用的堆栈符号信息
    void print_backtrace(std::ostream& of, const int max_frame = 128, const char* build_time = __TIME__, const char* build_date = __DATE__);
}

#endif
