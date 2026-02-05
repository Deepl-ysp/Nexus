#ifndef TOKEN_H
#define TOKEN_H

#include <string>
namespace nexus {
    namespace frontend {
        
        // Token类型枚举
        enum class TokenType {
            // 关键字
            LET, CONST, FN, ASYNC, AWAIT, COROUTINE, STRUCT, PROCESS, CLASS, INTERFACE,
            IF, ELSE, FOR, WHILE, RETURN, BREAK, CONTINUE, IMPORT, EXPORT, USE, NAMESPACE,
            CONSTRUCTOR, THIS, NEW, YIELD, TRY, CATCH, FINALLY, NULL_, TRUE_, FALSE_,
            TYPEOF, INSTANCEOF, AS,
            
            // 操作符
            PLUS, MINUS, MULTIPLY, DIVIDE, MODULO, 
            PLUS_PLUS, MINUS_MINUS, 
            ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN, MODULO_ASSIGN,
            EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, 
            AND, OR, NOT, 
            BIT_AND, BIT_OR, BIT_XOR, BIT_NOT, 
            LEFT_SHIFT, RIGHT_SHIFT, UNSIGNED_RIGHT_SHIFT, 
            LEFT_SHIFT_ASSIGN, RIGHT_SHIFT_ASSIGN, UNSIGNED_RIGHT_SHIFT_ASSIGN, 
            BIT_AND_ASSIGN, BIT_OR_ASSIGN, BIT_XOR_ASSIGN,
            
            // 标点符号
            SEMICOLON, COLON, COMMA, DOT, 
            LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,
            ARROW, DOUBLE_COLON, 
            
            // 字面量
            IDENTIFIER, INTEGER, FLOAT, STRING, CHARACTER,
            
            // 特殊标记
            END_OF_FILE, ERROR
        };
        
        // Token类
        class Token {
            public:
            TokenType type;
            std::string lexeme;
            int line;
            int column;
            
            Token(TokenType type, const std::string& lexeme, int line, int column)
            : type(type), lexeme(lexeme), line(line), column(column) {}
            
            // 转换为字符串
            std::string toString() const;
        };
        
    } // namespace frontend
} // namespace nexus

#endif