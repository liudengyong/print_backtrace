/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-12 15:50:12
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-12 17:01:28
 * @ Description: Print backtrace log header
 */

namespace dy::logger {

    /// @brief 日志级别定义
    typedef enum {Debug = -1, Info = 0, Warn, Error} Level;

    /// @brief 设置日志级别
    void set_level(const Level level);

    /// @brief 获取日志级别
    Level get_level();

    /// @brief 获取日志标签
    const char* get_tag();

    /// @brief 打印日志
    void log(const Level level, const char* format, ...);

    /// @brief 打印日志debug
    #define log_d(format, ...) log(Level::Debug, format, ##__VA_ARGS__)

    /// @brief 打印日志info
    #define log_i(format, ...) log(Level::Info, format, ##__VA_ARGS__)

    /// @brief 打印日志warn
    #define log_w(format, ...) log(Level::Warn, format, ##__VA_ARGS__)

    /// @brief 打印日志error
    #define log_e(format, ...) log(Level::Error, format, ##__VA_ARGS__)
}
