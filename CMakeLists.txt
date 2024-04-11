cmake_minimum_required(VERSION 3.15)

# 设置编译目标名称
project(print_backtrace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Werror)

# add_compile_definitions(MSGPACK_NO_BOOST)

# 警告即错误
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
# add_definitions(-g)
# add_definitions(-rdynamic)

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
