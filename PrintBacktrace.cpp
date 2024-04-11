/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-11 13:45:53
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-11 16:52:04
 * @ Description: Print backtrace info for debug
 */

#include "PrintBacktrace.h"

#include <execinfo.h>
#include <cxxabi.h>
#include <fstream>
#include <regex>
#include <typeinfo>
#include <unistd.h>

namespace dy {

    static std::string getProcessName() {
        std::string processName;
        std::ifstream file("/proc/self/cmdline");
        if (file.is_open()) {
            std::getline(file, processName, '\0'); // 使用'\0'作为分隔符，因为cmdline文件中进程名是以null字符分隔的
            file.close();
        }
        return processName;
    }

    static std::string getThreadName() {
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
    static std::string demangle(const char* str) {
        size_t size = 0;
        int status = 0;

        char* tmp = abi::__cxa_demangle(str, nullptr, &size, &status);
        if (tmp != nullptr) {
            std::string result(tmp);
            free(tmp);
            return result;
        }

        return str;
    }

    // 提取括号中的+前面的符号
    static std::string subSym(std::string input) {
        // /map/bev/yhlib/libyh_bev_cam_perception.so(_ZN14HrzJ5DNNEngine6DeInitEv+0x14) [0xffffa92b1a64]
        // match[0]=(_ZN14HrzJ5DNNEngine6DeInitEv+0x14), match[1]=_ZN14HrzJ5DNNEngine6DeInitEv+0x14
        // 匹配括号，捕获括号内的字符串
        std::regex rgx("\\((.*?)\\)");
        std::smatch match;

        if (std::regex_search(input, match, rgx)) {
            // 第一个是匹配的内容，第二个是捕获的内容
            std::string symbol = match[1].str();
            size_t pos = symbol.find("+");
            // 提取+号前的符号
            if (pos != std::string::npos) {
                return symbol.substr(0, pos);
            }
        }

        return "";
    }

    std::string get_type_name(const std::type_info& info) {
        std::string name = info.name();
        return demangle(name.c_str());
    }

    // 打印堆栈到文件
    void print_backtrace(std::ostream& of, const int max_frame, const char* time, const char* date) {
        // TODO 待开放参数
        const char* prifix = "[print_backtrace]";
        const int skip_frame = 1;

        // assert(skip_frame < max_frame);

        void *buf[max_frame];
        int size = backtrace(buf, sizeof(buf));
        char **strs = backtrace_symbols(buf, size);

        if (strs != nullptr) {
            of << prifix << " <" << getProcessName() << "/" << getThreadName() << "> compile time " << time << " " << date << std::endl;

            for (int i = skip_frame; i < size; i++) {
                std::string frame = strs[i];
                // /map/bev/yhlib/libyh_bev_cam_perception.so(_ZN14HrzJ5DNNEngine6DeInitEv+0x14) [0xffffa92b1a64]
                // /map/bev/yhlib/libyh_bev_cam_perception.so(HrzJ5DNNEngine::DeInit()+0x14) [0xffffa92b1a64]

                std::string sym = subSym(frame);
                if (sym.size() > 0) {
                    std::string readableSym = demangle(sym.c_str());
                    size_t startPos = frame.find(sym);
                    size_t endPos = frame.find("+", startPos);
                    frame = frame.replace(startPos, endPos - startPos, readableSym);
                }

                of << prifix << " " << frame << std::endl;
            }

            free(strs);
        } else {
            perror("No backtrace symbols");
        }
    }

}