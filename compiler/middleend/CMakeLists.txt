cmake_minimum_required(VERSION 3.10)

project(nexus_middleend)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件
set(SOURCE_FILES
    ir.cpp
    codegen.cpp
    optimizer.cpp
)

# 头文件
set(HEADER_FILES
    ir.h
    codegen.h
    optimizer.h
)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../frontend
)

# 创建库
add_library(nexus_middleend ${SOURCE_FILES} ${HEADER_FILES})

# 测试程序
add_executable(middleend_test middleend_test.cpp)
target_link_libraries(middleend_test nexus_middleend nexus_frontend)

# 包含目录
target_include_directories(nexus_middleend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(middleend_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
