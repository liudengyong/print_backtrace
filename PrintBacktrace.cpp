/**
 * @ Author: liudengyong
 * @ Create Time: 2024-04-11 13:45:53
 * @ Modified by: liudengyong
 * @ Modified time: 2024-04-12 11:45:18
 * @ Description: Print backtrace info for debug
 */

#include "PrintBacktrace.h"

#include <execinfo.h>
#include <fstream>
#include <regex>

namespace dy {

    // 日志标签
    const char* TAG = "[PrintBacktrace] ";

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

    // 打印堆栈到文件
    void print_backtrace(std::ostream& of, const int max_frame, const char* time, const char* date) {
        // TODO 待开放参数
        const int skip_frame = 1;

        // assert(skip_frame < max_frame);

        void *buf[max_frame];
        int size = backtrace(buf, sizeof(buf));
        char **strs = backtrace_symbols(buf, size);

        if (strs != nullptr) {
            of << TAG << "<" << get_process_name() << "/" << get_thread_name() << "> compile time " << time << " " << date << std::endl;

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

                of << TAG << frame << std::endl;
            }

            free(strs);
        } else {
            perror("No backtrace symbols");
        }
    }

    // 日志目录
    static std::string logDir;

    // SIGSEGV/SIGABRT 信号处理程序
    static void sigHandler(int sig) {
        std::string fileName = logDir + "/" + gen_filename_with_cur_ts();
        std::ofstream file(fileName);
        std::cout << TAG << "Print backtrace to file: " << fileName << std::endl;
        if (file.is_open()) {
            dy::print_backtrace(file);
            file.close();
        }

        exit(sig);
    }

    // 注册 SIGSEGV/SIGABRT 信号处理程序
    bool register_sig_handler(std::string log_dir, std::vector<int> signals) {
        std::cout << TAG << "Register signal handler, C++ version " << __cplusplus << std::endl;

        if (signals.size() == 0) {
            std::cerr << TAG << "Register signal handler failed : no signal is specified !" << std::endl;
            return false;
        }

        if (!mkdir(log_dir)) {
            std::cerr << TAG << "Register signal handler failed : cannt create log dir : " << log_dir << " !" << std::endl;
            return false;
        }

        logDir = log_dir;

        struct sigaction action{};
        action.sa_handler = sigHandler;
        for (auto sig : signals) {
            sigaction(sig, &action, NULL);
        }

        return true;
    }

}