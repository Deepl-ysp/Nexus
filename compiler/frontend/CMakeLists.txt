cmake_minimum_required(VERSION 3.10)

project(nexus_frontend)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件
set(SOURCE_FILES
    token.cpp
    lexer.cpp
    ast.cpp
    parser.cpp
    semantic_analyzer.cpp
)

# 头文件
set(HEADER_FILES
    token.h
    lexer.h
    ast.h
    parser.h
    semantic_analyzer.h
)

# 创建库
add_library(nexus_frontend ${SOURCE_FILES} ${HEADER_FILES})

# 词法分析器测试程序
add_executable(lexer_test lexer_test.cpp)
target_link_libraries(lexer_test nexus_frontend)

# 语法分析器测试程序
add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test nexus_frontend)

# 语义分析器测试程序
add_executable(semantic_test semantic_test.cpp)
target_link_libraries(semantic_test nexus_frontend)

# 包含目录
target_include_directories(nexus_frontend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(lexer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(parser_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(semantic_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
