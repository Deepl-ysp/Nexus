#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>

#include "./token.h"

namespace nexus {
namespace frontend {

// 词法分析器类
class Lexer {
 public:
  Lexer(const std::string& source);
    
    // 获取下一个Token
    Token getNextToken();
    
    // 回退一个Token
    void ungetToken(const Token& token);
    
private:
    // 源代码
    std::string source;
    // 当前位置
    size_t position;
    // 当前行号
    int line;
    // 当前列号
    int column;
    // 回退的Token
    Token* ungotToken;
    // 关键字映射
    std::unordered_map<std::string, TokenType> keywords;
    
    // 初始化关键字映射
    void initKeywords();
    
    // 辅助方法
    char peek() const;
    char advance();
    bool match(char expected);
    bool isAtEnd() const;
    
    // 词法分析方法
    Token scanToken();
    Token identifier();
    Token number();
    Token string();
    Token character();
    
    // 跳过空白和注释
    void skipWhitespace();
    void skipComment();
    
    // 创建Token
    Token makeToken(TokenType type, const std::string& lexeme);
    Token errorToken(const std::string& message);
};

} // namespace frontend
} // namespace nexus

#endif // LEXER_H
