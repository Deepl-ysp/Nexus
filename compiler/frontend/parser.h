#ifndef NEXUS_PARSER_H
#define NEXUS_PARSER_H

#include "./lexer.h"
#include "./ast.h"
#include <vector>
#include <memory>

namespace nexus {
namespace frontend {

class Parser {
public:
    Parser(const std::string& source);
    
    // 解析整个程序
    std::vector<std::unique_ptr<Stmt>> parse();
    
private:
    Lexer lexer;
    Token currentToken;
    Token previousToken;
    bool hadError;
    
    // 错误处理
    void error(const std::string& message);
    void synchronize();
    
    // 词法分析器操作
    void advance();
    bool match(const TokenType& type);
    bool check(const TokenType& type);
    Token consume(const TokenType& type, const std::string& message);
    
    // 解析表达式
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> orExpr();
    std::unique_ptr<Expr> andExpr();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> member();
    
    // 解析语句
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Stmt> blockStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> forStatement();
    std::unique_ptr<Stmt> returnStatement();
    std::unique_ptr<Stmt> varStatement();
    std::unique_ptr<Stmt> constStatement();
    std::unique_ptr<Stmt> functionStatement();
    std::unique_ptr<Stmt> classStatement();
    std::unique_ptr<Stmt> structStatement();
    std::unique_ptr<Stmt> tryStatement();
    std::unique_ptr<Stmt> catchStatement();
    std::unique_ptr<Stmt> finallyStatement();
    std::unique_ptr<Stmt> throwStatement();
    std::unique_ptr<Stmt> processStatement();
    
    // 解析类型
    std::string parseType();
    
    // 解析参数
    std::vector<std::pair<std::string, std::string>> parseParameters();
};

} // namespace frontend
} // namespace nexus

#endif // NEXUS_PARSER_H
