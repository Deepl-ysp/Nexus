#include "./codegen.h"
#include "../frontend/parser.h"
#include "../frontend/semantic_analyzer.h"
#include "../middleend/codegen.h"

#include <iostream>
#include <fstream>
#include <string>

namespace nexus {
namespace backend {

// 从文件读取内容
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        exit(1);
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    return content;
}

// 测试后端代码生成
void testCodeGeneration(const std::string& source) {
    std::cout << "=== Testing Backend Code Generation ===" << std::endl;
    
    // 解析代码
    frontend::Parser parser(source);
    std::vector<std::unique_ptr<frontend::Stmt>> statements = parser.parse();
    
    // 语义分析
    frontend::SemanticAnalyzer analyzer;
    analyzer.analyze(statements);
    
    if (analyzer.hadError()) {
        std::cerr << "Semantic analysis failed." << std::endl;
        return;
    }
    
    // 生成中间代码
    middleend::CodeGenerator middleendCodegen;
    std::unique_ptr<middleend::Module> module = middleendCodegen.generate(statements);
    
    // 生成目标代码
    CodeGenerator backendCodegen;
    std::string code = backendCodegen.generate(*module);
    
    // 打印生成的目标代码
    std::cout << "Generated x86_64 Assembly Code:" << std::endl;
    std::cout << code << std::endl;
    
    std::cout << "=== Test Completed ===" << std::endl;
}

} // namespace backend
} // namespace nexus

int main(int argc, char* argv[]) {
    try {
        std::string source;
        
        if (argc > 1) {
            // 从命令行参数读取文件路径
            std::string filePath = argv[1];
            std::cout << "Testing with file: " << filePath << "\n";
            source = nexus::backend::readFile(filePath);
        } else {
            // 测试代码
            std::string testCode = R"(
                let x: int = 10;
                const PI: float = 3.14;
                
                fn add(a: int, b: int): int {
                    return a + b;
                }
                
                let result = add(1, 2);
                println(result);
                
                if (result > 0) {
                    println("Result is positive");
                } else {
                    println("Result is non-positive");
                }
                
                while (x > 0) {
                    println(x);
                    x = x - 1;
                }
            )";
            
            std::cout << "Testing with sample code...\n";
            source = testCode;
        }
        
        // 运行测试
        nexus::backend::testCodeGeneration(source);
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
    }
    
    return 0;
}
