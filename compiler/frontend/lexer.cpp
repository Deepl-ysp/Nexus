#include "lexer.h"

#include <cctype>
#include <stdexcept>

namespace nexus {
namespace frontend {

// 构造函数
Lexer::Lexer(const std::string& source)
    : source(source), position(0), line(1), column(1), ungotToken(nullptr) {
  initKeywords();
}

// 初始化关键字映射
void Lexer::initKeywords() {
  keywords["let"] = TokenType::LET;
  keywords["const"] = TokenType::CONST;
  keywords["fn"] = TokenType::FN;
  keywords["async"] = TokenType::ASYNC;
  keywords["await"] = TokenType::AWAIT;
  keywords["coroutine"] = TokenType::COROUTINE;
  keywords["struct"] = TokenType::STRUCT;
  keywords["process"] = TokenType::PROCESS;
  keywords["class"] = TokenType::CLASS;
  keywords["interface"] = TokenType::INTERFACE;
  keywords["if"] = TokenType::IF;
  keywords["else"] = TokenType::ELSE;
  keywords["for"] = TokenType::FOR;
  keywords["while"] = TokenType::WHILE;
  keywords["return"] = TokenType::RETURN;
  keywords["break"] = TokenType::BREAK;
    keywords["continue"] = TokenType::CONTINUE;
    keywords["import"] = TokenType::IMPORT;
    keywords["export"] = TokenType::EXPORT;
    keywords["use"] = TokenType::USE;
    keywords["namespace"] = TokenType::NAMESPACE;
    keywords["constructor"] = TokenType::CONSTRUCTOR;
    keywords["this"] = TokenType::THIS;
    keywords["new"] = TokenType::NEW;
    keywords["yield"] = TokenType::YIELD;
    keywords["try"] = TokenType::TRY;
    keywords["catch"] = TokenType::CATCH;
    keywords["finally"] = TokenType::FINALLY;
    keywords["null"] = TokenType::NULL_;
    keywords["true"] = TokenType::TRUE_;
    keywords["false"] = TokenType::FALSE_;
    keywords["typeof"] = TokenType::TYPEOF;
    keywords["instanceof"] = TokenType::INSTANCEOF;
    keywords["as"] = TokenType::AS;
}

// 获取下一个Token
Token Lexer::getNextToken() {
    // 如果有回退的Token，返回它
    if (ungotToken != nullptr) {
        Token token = *ungotToken;
        delete ungotToken;
        ungotToken = nullptr;
        return token;
    }
    
    // 跳过空白和注释
    skipWhitespace();
    
    // 检查是否到达文件末尾
    if (isAtEnd()) {
        return makeToken(TokenType::END_OF_FILE, "");
    }
    
    // 扫描Token
    return scanToken();
}

// 回退一个Token
void Lexer::ungetToken(const Token& token) {
    if (ungotToken != nullptr) {
        delete ungotToken;
    }
    ungotToken = new Token(token);
}

// 辅助方法：查看当前字符
char Lexer::peek() const {
    if (isAtEnd()) {
        return '\0';
    }
    return source[position];
}

// 辅助方法：前进一个字符
char Lexer::advance() {
    char c = source[position];
    position++;
    column++;
    if (c == '\n') {
        line++;
        column = 1;
    }
    return c;
}

// 辅助方法：匹配预期字符
bool Lexer::match(char expected) {
    if (isAtEnd() || source[position] != expected) {
        return false;
    }
    position++;
    column++;
    return true;
}

// 辅助方法：检查是否到达文件末尾
bool Lexer::isAtEnd() const {
    return position >= source.length();
}

// 跳过空白和注释
void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ': case '\t': case '\r': case '\n':
                advance();
                break;
            case '/':
                if (peek() == '/') {
                    // 单行注释
                    skipComment();
                } else if (peek() == '*') {
                    // 多行注释
                    advance();
                    advance();
                    while (!isAtEnd() && !(peek() == '*' && source[position + 1] == '/')) {
                        if (peek() == '\n') {
                            line++;
                            column = 1;
                        }
                        advance();
                    }
                    if (!isAtEnd()) {
                        advance(); // 跳过 '*'
                        advance(); // 跳过 '/'
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

// 跳过单行注释
void Lexer::skipComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

// 扫描Token
Token Lexer::scanToken() {
    char c = advance();
    
    // 操作符和标点符号
    switch (c) {
        // 单字符操作符
        case '+':
            if (match('+')) {
                return makeToken(TokenType::PLUS_PLUS, "++");
            } else if (match('=')) {
                return makeToken(TokenType::PLUS_ASSIGN, "+=");
            } else {
                return makeToken(TokenType::PLUS, "+");
            }
        case '-':
            if (match('-')) {
                return makeToken(TokenType::MINUS_MINUS, "--");
            } else if (match('=')) {
                return makeToken(TokenType::MINUS_ASSIGN, "-=");
            } else if (match('>')) {
                return makeToken(TokenType::ARROW, "->");
            } else {
                return makeToken(TokenType::MINUS, "-");
            }
        case '*':
            if (match('=')) {
                return makeToken(TokenType::MULTIPLY_ASSIGN, "*=");
            } else {
                return makeToken(TokenType::MULTIPLY, "*");
            }
        case '/':
            if (match('=')) {
                return makeToken(TokenType::DIVIDE_ASSIGN, "/=");
            } else {
                return makeToken(TokenType::DIVIDE, "/");
            }
        case '%':
            if (match('=')) {
                return makeToken(TokenType::MODULO_ASSIGN, "%=");
            } else {
                return makeToken(TokenType::MODULO, "%");
            }
        case '=':
            if (match('=')) {
                return makeToken(TokenType::EQUAL, "==");
            } else {
                return makeToken(TokenType::ASSIGN, "=");
            }
        case '!':
            if (match('=')) {
                return makeToken(TokenType::NOT_EQUAL, "!=");
            } else {
                return makeToken(TokenType::NOT, "!");
            }
        case '<':
            if (match('<')) {
                if (match('=')) {
                    return makeToken(TokenType::LEFT_SHIFT_ASSIGN, "<<=");
                } else {
                    return makeToken(TokenType::LEFT_SHIFT, "<<");
                }
            } else if (match('=')) {
                return makeToken(TokenType::LESS_EQUAL, "<=");
            } else {
                return makeToken(TokenType::LESS, "<");
            }
        case '>':
            if (match('>')) {
                if (match('>')) {
                    if (match('=')) {
                        return makeToken(TokenType::UNSIGNED_RIGHT_SHIFT_ASSIGN, ">>>=");
                    } else {
                        return makeToken(TokenType::UNSIGNED_RIGHT_SHIFT, ">>>");
                    }
                } else if (match('=')) {
                    return makeToken(TokenType::RIGHT_SHIFT_ASSIGN, ">>=");
                } else {
                    return makeToken(TokenType::RIGHT_SHIFT, ">>");
                }
            } else if (match('=')) {
                return makeToken(TokenType::GREATER_EQUAL, ">=");
            } else {
                return makeToken(TokenType::GREATER, ">");
            }
        case '&':
            if (match('&')) {
                return makeToken(TokenType::AND, "&&");
            } else if (match('=')) {
                return makeToken(TokenType::BIT_AND_ASSIGN, "&=");
            } else {
                return makeToken(TokenType::BIT_AND, "&");
            }
        case '|':
            if (match('|')) {
                return makeToken(TokenType::OR, "||");
            } else if (match('=')) {
                return makeToken(TokenType::BIT_OR_ASSIGN, "|=");
            } else {
                return makeToken(TokenType::BIT_OR, "|");
            }
        case '^':
            if (match('=')) {
                return makeToken(TokenType::BIT_XOR_ASSIGN, "^=");
            } else {
                return makeToken(TokenType::BIT_XOR, "^");
            }
        case '~':
            return makeToken(TokenType::BIT_NOT, "~");
        
        // 标点符号
        case ';':
            return makeToken(TokenType::SEMICOLON, ";");
        case ':':
            if (match(':')) {
                return makeToken(TokenType::DOUBLE_COLON, "::");
            } else {
                return makeToken(TokenType::COLON, ":");
            }
        case ',':
            return makeToken(TokenType::COMMA, ",");
        case '.':
            if (isdigit(peek())) {
                return number();
            } else {
                return makeToken(TokenType::DOT, ".");
            }
        case '(':
            return makeToken(TokenType::LEFT_PAREN, "(");
        case ')':
            return makeToken(TokenType::RIGHT_PAREN, ")");
        case '{':
            return makeToken(TokenType::LEFT_BRACE, "{");
        case '}':
            return makeToken(TokenType::RIGHT_BRACE, "}");
        case '[':
            return makeToken(TokenType::LEFT_BRACKET, "[");
        case ']':
            return makeToken(TokenType::RIGHT_BRACKET, "]");
        
        // 字符串和字符
        case '"':
            return string();
        case '\'':
            return character();
        
        // 数字
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return number();
        
        // 标识符
        case 'a': case 'b': case 'c': case 'd': case 'e':
        case 'f': case 'g': case 'h': case 'i': case 'j':
        case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':
        case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E':
        case 'F': case 'G': case 'H': case 'I': case 'J':
        case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':
        case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_':
            return identifier();
        
        // 错误
        default:
            return errorToken("Unexpected character");
    }
}

// 处理标识符
Token Lexer::identifier() {
    size_t start = position - 1;
    while (!isAtEnd() && (isalnum(peek()) || peek() == '_')) {
        advance();
    }
    std::string lexeme = source.substr(start, position - start);
    
    // 检查是否是关键字
    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        return makeToken(it->second, lexeme);
    }
    
    // 否则是标识符
    return makeToken(TokenType::IDENTIFIER, lexeme);
}

// 处理数字
Token Lexer::number() {
    size_t start = position - 1;
    bool isFloat = false;
    
    // 处理整数部分
    while (!isAtEnd() && isdigit(peek())) {
        advance();
    }
    
    // 处理小数部分
    if (peek() == '.' && isdigit(source[position + 1])) {
        isFloat = true;
        advance();
        while (!isAtEnd() && isdigit(peek())) {
            advance();
        }
    }
    
    // 处理指数部分
    if ((peek() == 'e' || peek() == 'E') && (isdigit(source[position + 1]) || (source[position + 1] == '+' || source[position + 1] == '-') && isdigit(source[position + 2]))) {
        isFloat = true;
        advance();
        if (peek() == '+' || peek() == '-') {
            advance();
        }
        while (!isAtEnd() && isdigit(peek())) {
            advance();
        }
    }
    
    std::string lexeme = source.substr(start, position - start);
    if (isFloat) {
        return makeToken(TokenType::FLOAT, lexeme);
    } else {
        return makeToken(TokenType::INTEGER, lexeme);
    }
}

// 处理字符串
Token Lexer::string() {
    size_t start = position;
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\\') {
            advance(); // 跳过转义字符
        }
        advance();
    }
    
    if (isAtEnd()) {
        return errorToken("Unterminated string");
    }
    
    advance(); // 跳过结束的 '"'
    std::string lexeme = source.substr(start, position - start - 1);
    return makeToken(TokenType::STRING, lexeme);
}

// 处理字符
Token Lexer::character() {
    size_t start = position;
    if (peek() == '\\') {
        advance(); // 跳过转义字符
    }
    advance(); // 跳过字符
    
    if (isAtEnd() || peek() != '\'') {
        return errorToken("Unterminated character");
    }
    
    advance(); // 跳过结束的 '\''
    std::string lexeme = source.substr(start, position - start - 1);
    return makeToken(TokenType::CHARACTER, lexeme);
}

// 创建Token
Token Lexer::makeToken(TokenType type, const std::string& lexeme) {
    int tokenLine = line;
    int tokenColumn = column - lexeme.length();
    return Token(type, lexeme, tokenLine, tokenColumn);
}

// 创建错误Token
Token Lexer::errorToken(const std::string& message) {
    return Token(TokenType::ERROR, message, line, column);
}

} // namespace frontend
} // namespace nexus
