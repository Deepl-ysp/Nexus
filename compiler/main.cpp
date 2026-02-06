/**
 * @file main.cpp
 * @brief Nexus 编译器的主入口文件
 * @author Nexus 编译器团队
 * @version 1.0
 */

#include "frontend/parser.h"
#include "frontend/semantic_analyzer.h"
#include "middleend/codegen.h"
#include "middleend/optimizer.h"
#include "backend/codegen.h"

#include <iostream>
#include <fstream>
#include <string>

namespace nexus {

/**
 * @brief 从文件读取内容
 * 
 * @param filePath 文件路径
 * @return std::string 文件内容
 */
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

/**
 * @brief 向文件写入内容
 * 
 * @param filePath 文件路径
 * @param content 要写入的内容
 */
void writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        exit(1);
    }
    
    file << content;
    file.close();
}

/**
 * @brief 编译 Nexus 源文件到目标代码
 * 
 * @param inputPath 输入 Nexus 源文件路径
 * @param outputPath 输出目标代码文件路径
 */
void compile(const std::string& inputPath, const std::string& outputPath) {
    std::cout << "=== Nexus Compiler ===" << std::endl;
    std::cout << "Compiling: " << inputPath << " -> " << outputPath << std::endl;
    
    // Step 1: Read input file
    std::cout << "1. Reading input file..." << std::endl;
    std::string source = readFile(inputPath);
    
    // Step 2: Parse code
    std::cout << "2. Parsing code..." << std::endl;
    frontend::Parser parser(source);
    std::vector<std::unique_ptr<frontend::Stmt>> statements = parser.parse();
    
    // Step 3: Perform semantic analysis
    std::cout << "3. Performing semantic analysis..." << std::endl;
    frontend::SemanticAnalyzer analyzer;
    analyzer.analyze(statements);
    
    if (analyzer.hadError()) {
        std::cerr << "Semantic analysis failed. Aborting compilation." << std::endl;
        exit(1);
    }
    
    // Step 4: Generate intermediate code
    std::cout << "4. Generating intermediate code..." << std::endl;
    middleend::CodeGenerator middleendCodegen;
    std::unique_ptr<middleend::Module> module = middleendCodegen.generate(statements);
    
    // Step 5: Optimize intermediate code
    std::cout << "5. Optimizing intermediate code..." << std::endl;
    middleend::Optimizer optimizer;
    std::unique_ptr<middleend::Module> optimizedModule = optimizer.optimize(std::move(module));
    
    // Step 6: Generate target code
    std::cout << "6. Generating target code..." << std::endl;
    backend::CodeGenerator backendCodegen;
    std::string code = backendCodegen.generate(*optimizedModule);
    
    // Step 7: Write output file
    std::cout << "7. Writing output file..." << std::endl;
    writeFile(outputPath, code);
    
    std::cout << "Compilation completed successfully!" << std::endl;
}

} // namespace nexus

/**
 * @brief Nexus 编译器的主函数
 * 
 * @param argc 命令行参数数量
 * @param argv 命令行参数
 * @return int 编译成功返回 0，失败返回 1
 */
int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: nexus-compiler <input_file> <output_file>" << std::endl;
            return 1;
        }
        
        std::string inputPath = argv[1];
        std::string outputPath = argv[2];
        
        nexus::compile(inputPath, outputPath);
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception occurred." << std::endl;
        return 1;
    }
    
    return 0;
}
