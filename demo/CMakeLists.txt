cmake_minimum_required(VERSION 3.15)

# 设置编译目标名称
project(print_backtrace_demo)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 警告即错误
add_definitions(-Werror)

# 调试模式才有符号表信息
set(CMAKE_BUILD_TYPE Debug)

include_directories(../include)
link_directories(../build) # 此处是print_backtrace编译目录或者安装目录

# 生成可执行程序
add_executable(${PROJECT_NAME} main.cpp)

# 设置链接目标
target_link_libraries(${PROJECT_NAME} PUBLIC print_backtrace "-rdynamic")
