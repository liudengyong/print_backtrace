/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-12 15:50:12
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-12 17:01:37
 * @ Description: Print backtrace log
 */

#include "Log.h"

#include <stdio.h>
#include <stdarg.h>

namespace dy::logger {

    // 日志标签
    const char* TAG = "[PrintBacktrace]";

    // 日志级别
    static Level logLevel = Level::Info;

    // 打印日志
    void log(const Level level, const char* format, ...) {
        if (level >= logLevel) {
            // 打印标签
            printf("%s ", TAG);

            va_list args;
            va_start(args, format);

            // 打印日志
            vprintf(format, args);

            va_end(args);

            // 换行    
            printf("\n");
        }
    }

    // 获取日志标签
    const char* get_tag() {
        return TAG;
    }

    // 设置日志级别
    void set_level(const Level level) {
        logLevel = level;
    }

    // 获取日志级别
    Level get_level() {
        return logLevel;
    }
}