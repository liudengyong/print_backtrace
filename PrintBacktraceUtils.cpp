/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-12 11:32:44
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-12 16:40:05
 * @ Description: Print backtrace utils
 */

#include "PrintBacktraceUtils.h"
#include "Log.h"

#include <cxxabi.h>
#include <fstream>
#include <sys/stat.h>
#include <typeinfo>
#include <unistd.h>

#if __cplusplus > 201703L
    #include <filesystem>
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif

namespace dy {

    using namespace logger;

    // 文件夹是否存在
    static bool folderExists(const std::string& dir) {
        struct stat info;
        if (stat(dir.c_str(), &info) != 0) {
            return false;
        }
        return (info.st_mode & S_IFDIR);
    }

    // 创建文件夹，创建失败返回错误（如果存在则不创建）
    bool mkdir(const std::string& dir) {
        if (!folderExists(dir)) {
            try {
                fs::create_directory(dir);
            } catch (const std::exception& e) {
                log_e("Failed to create dir %s : %s", dir.c_str(), e.what());
                return false;
            }
        }
        return true;
    }

    // 获取进程名称
    std::string get_process_name() {
        std::string processName;
        std::ifstream file("/proc/self/cmdline");
        if (file.is_open()) {
            std::getline(file, processName, '\0'); // 使用'\0'作为分隔符，因为cmdline文件中进程名是以null字符分隔的
            file.close();
        }
        return processName;
    }

    // 获取线程名称
    std::string get_thread_name() {
        std::string threadName;
        char path[256];
        int pid = getpid();
        snprintf(path, sizeof(path), "/proc/self/task/%d/comm", pid);

        std::ifstream file(path);
        if (file.is_open()) {
            std::getline(file, threadName);
            file.close();
        }

        pid_t threadId = gettid();
        return threadName + "(" + std::to_string(threadId) + ")";
    }

    // 转可读符号名
    std::string demangle(const char* str) {
        char* tmp = abi::__cxa_demangle(str, nullptr, nullptr, nullptr);

        if (tmp != nullptr) {
            std::string result(tmp);
            free(tmp);
            return result;
        }

        return str;
    }

    // 获取可读类型名称
    std::string get_type_name(const std::type_info& info) {
        std::string name = info.name();
        return demangle(name.c_str());
    }

    // 生成当前时间戳文件名（如20240411_182710_11.backtrace，下划线分割开日期，时间和信号）
    std::string gen_filename_with_cur_ts(int sig) {
        time_t t = std::time(nullptr);
        struct tm* now = std::localtime(&t);
        char formatedTime[50];
        strftime(formatedTime, sizeof(formatedTime), "%Y%m%d_%H%M%S", now);

        std::string fileName = formatedTime;
        if (sig != 0) {
            fileName += "_" + std::to_string(sig);
        }

        return fileName + ".backtrace";
    }
}