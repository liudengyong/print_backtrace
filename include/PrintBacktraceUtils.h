/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-12 11:32:44
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-12 11:42:10
 * @ Description: Print backtrace utils header
 */

#ifndef __DY_PRINT_BACKTRACE_UTILS__
#define __DY_PRINT_BACKTRACE_UTILS__

#include <string>

namespace dy {
    /// @brief 获取可读性类型名称
    std::string get_type_name(const std::type_info& info);

    /// @brief 生成当前时间戳文件名（如20240411_182710_11.backtrace，下划线分割开日期，时间和信号）
    std::string gen_filename_with_cur_ts(int sig = 0);

    /// @brief 创建文件夹，创建失败返回错误（如果存在则不创建）
    bool mkdir(const std::string& dir);

    /// @brief 转可读符号名
    std::string demangle(const char* str);

    /// @brief 获取进程名称
    std::string get_process_name();

    /// @brief 获取线程名称
    std::string get_thread_name();
}

#endif
