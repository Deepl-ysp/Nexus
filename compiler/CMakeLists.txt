cmake_minimum_required(VERSION 3.10)

project(nexus_compiler)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend
    ${CMAKE_CURRENT_SOURCE_DIR}/middleend
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
)

# 前端源文件
set(FRONTEND_SOURCE_FILES
    frontend/token.cpp
    frontend/lexer.cpp
    frontend/ast.cpp
    frontend/parser.cpp
    frontend/semantic_analyzer.cpp
)

# 中间端源文件
set(MIDDLEEND_SOURCE_FILES
    middleend/ir.cpp
    middleend/codegen.cpp
    middleend/optimizer.cpp
)

# 后端源文件
set(BACKEND_SOURCE_FILES
    backend/codegen.cpp
)

# 编译器源文件
set(COMPILER_SOURCE_FILES
    main.cpp
    ${FRONTEND_SOURCE_FILES}
    ${MIDDLEEND_SOURCE_FILES}
    ${BACKEND_SOURCE_FILES}
)

# 创建编译器可执行文件
add_executable(nexus-compiler ${COMPILER_SOURCE_FILES})

# 前端测试程序
add_executable(lexer_test frontend/lexer_test.cpp ${FRONTEND_SOURCE_FILES})
add_executable(parser_test frontend/parser_test.cpp ${FRONTEND_SOURCE_FILES})
add_executable(semantic_test frontend/semantic_test.cpp ${FRONTEND_SOURCE_FILES})

# 中间端测试程序
add_executable(middleend_test middleend/middleend_test.cpp ${FRONTEND_SOURCE_FILES} ${MIDDLEEND_SOURCE_FILES})

# 后端测试程序
add_executable(backend_test backend/backend_test.cpp ${FRONTEND_SOURCE_FILES} ${MIDDLEEND_SOURCE_FILES} ${BACKEND_SOURCE_FILES})
