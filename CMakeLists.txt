cmake_minimum_required(VERSION 3.15)

# 设置编译目标名称
project(print_backtrace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 警告即错误
add_definitions(-Werror)

set(CMAKE_BUILD_TYPE Release)

include_directories(include)

set(SRC_LIST
    PrintBacktrace.cpp
)

# 生成动态库
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

# 安装库
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION lib
)

# 安装对外接口的头文件
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/${PROJECT_NAME}/)
