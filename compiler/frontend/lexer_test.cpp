#include "./lexer.h"

#include <fstream>
#include <iostream>

namespace nexus {
namespace frontend {

// 测试词法分析器
void testLexer(const std::string& source) {
  Lexer lexer(source);
  Token token;
    
    std::cout << "Token stream:\n";
    std::cout << "----------------------------------------\n";
    
    do {
        token = lexer.getNextToken();
        std::cout << token.toString() << std::endl;
        if (token.type == TokenType::ERROR) {
            std::cerr << "Error token found, stopping..." << std::endl;
            break;
        }
    } while (token.type != TokenType::END_OF_FILE);
    
    std::cout << "----------------------------------------\n";
    std::cout << "Lexical analysis completed." << std::endl;
}

} // namespace frontend
} // namespace nexus

int main() {
    // 测试代码
    std::string testCode = R"(
        // 测试注释
        let x: int = 10;
        const PI: float = 3.14;
        
        async fn add(a: int, b: int): int {
            return a + b;
        }
        
        coroutine fn count() {
            for (let i: int = 0; i < 5; i++) {
                println(i);
                yield;
            }
        }
        
        struct Point {
            x: int;
            y: int;
        }
        
        process::spawn(() => {
            println("Hello from another process!");
        });
        
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
        
        let person = new Person("Alice", 30);
        println(person.greet());
        
        // 测试表达式
        let result = (10 + 5) * 2 - 8 / 4;
        println(`Result: ${result}`);
        
        // 测试条件语句
        if (result > 0) {
            println("Result is positive");
        } else if (result < 0) {
            println("Result is negative");
        } else {
            println("Result is zero");
        }
        
        // 测试循环
        let count: int = 0;
        while (count < 3) {
            println(`Count: ${count}`);
            count++;
        }
        
        // 测试异常处理
        try {
            let division = 10 / 0;
        } catch (e: Error) {
            println(`Error: ${e.message}`);
        } finally {
            println("Cleanup code");
        }
    )";
    
    // 运行测试
    std::cout << "Testing Lexer with sample code...\n";
    nexus::frontend::testLexer(testCode);
    
    return 0;
}
