#include "./parser.h"
#include "./semantic_analyzer.h"

#include <fstream>
#include <iostream>

namespace nexus {
namespace frontend {

// 测试语义分析器
void testSemanticAnalyzer(const std::string& source) {
    std::cout << "Testing Semantic Analyzer...\n";
    std::cout << "----------------------------------------\n";
    
    // 解析代码
    Parser parser(source);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
    
    // 进行语义分析
    SemanticAnalyzer analyzer;
    analyzer.analyze(statements);
    
    // 输出结果
    if (analyzer.hadError()) {
        std::cout << "\nSemantic analysis completed with errors.\n";
    } else {
        std::cout << "\nSemantic analysis completed successfully.\n";
    }
    
    std::cout << "----------------------------------------\n";
}

} // namespace frontend
} // namespace nexus

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

int main(int argc, char* argv[]) {
    std::string source;
    
    if (argc > 1) {
        // 从命令行参数读取文件路径
        std::string filePath = argv[1];
        std::cout << "Testing with file: " << filePath << "\n";
        source = readFile(filePath);
    } else {
        // 测试代码 - 包含一些语义错误
        std::string testCode = R"(
            // 测试变量作用域
            let x: int = 10;
            {
                let y: int = 20;
                let x: int = 30; // 应该允许，因为是在不同的作用域
                println(x);
            }
            println(y); // 应该报错，因为 y 不在作用域内
            
            // 测试类型错误
            let a: int = "hello"; // 应该报错，类型不匹配
            
            // 测试未定义变量
            let b: int = c; // 应该报错，c 未定义
            
            // 测试函数定义
            fn add(a: int, b: int): int {
                return a + b;
            }
            
            // 测试结构体定义
            struct Point {
                x: int;
                y: int;
            }
            
            // 测试类定义
            class Person {
                name: string;
                age: int;
                
                constructor(name: string, age: int) {
                    this.name = name;
                    this.age = age;
                }
                
                greet(): string {
                    return `Hello, my name is ${this.name}!`;
                }
            }
            
            // 测试函数调用
            let result = add(1, 2);
            println(result);
        )";
        
        std::cout << "Testing with sample code containing semantic errors...\n";
        source = testCode;
    }
    
    // 运行测试
    nexus::frontend::testSemanticAnalyzer(source);
    
    return 0;
}
