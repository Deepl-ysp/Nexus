#include "./parser.h"
#include "./semantic_analyzer.h"

#include <fstream>
#include <iostream>

namespace nexus {
namespace frontend {

// 测试语法分析器
void testParser(const std::string& source) {
    std::cout << "Testing Parser...\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Source code:\n" << source << "\n";
    std::cout << "----------------------------------------\n";
    
    Parser parser(source);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
    
    std::cout << "Parsed statements: " << statements.size() << std::endl;
    
    // 打印语句类型
    for (size_t i = 0; i < statements.size(); i++) {
        const Stmt* stmt = statements[i].get();
        std::cout << "Statement " << i << ": " << stmt->toString() << std::endl;
    }
    
    std::cout << "----------------------------------------\n";
    std::cout << "Parsing completed." << std::endl;
}

// 测试语义分析器
void testSemanticAnalyzer(const std::string& source) {
    std::cout << "\nTesting Semantic Analyzer...\n";
    std::cout << "----------------------------------------\n";
    
    Parser parser(source);
    std::vector<std::unique_ptr<Stmt>> statements = parser.parse();
    
    SemanticAnalyzer analyzer;
    analyzer.analyze(statements);
    
    if (analyzer.hadError()) {
        std::cout << "Semantic analysis completed with errors." << std::endl;
    } else {
        std::cout << "Semantic analysis completed successfully." << std::endl;
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
    try {
        std::string source;
        
        if (argc > 1) {
            // 从命令行参数读取文件路径
            std::string filePath = argv[1];
            std::cout << "Testing with file: " << filePath << "\n";
            source = readFile(filePath);
        } else {
            // 测试代码
            std::string testCode = R"(
                // 测试注释
                let x: int = 10;
                const PI: float = 3.14;
                
                fn add(a: int, b: int): int {
                    return a + b;
                }
                
                struct Point {
                    x: int;
                    y: int;
                }
                
                let result = add(1, 2);
                let c = x + result;
            )";
            
            std::cout << "Testing with sample code...\n";
            source = testCode;
        }
        
        // 运行测试
        std::cout << "Starting parser test...\n";
        nexus::frontend::testParser(source);
        std::cout << "Starting semantic analyzer test...\n";
        nexus::frontend::testSemanticAnalyzer(source);
        std::cout << "All tests completed!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
    }
    
    return 0;
}
