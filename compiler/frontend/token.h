/*
 * Nexus Compiler
 * Token definitions
 *
 * This file defines the token types and token class used by the lexer
 * to represent tokens in the Nexus source code.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

namespace nexus {
    namespace frontend {
        
        /**
         * Token type enumeration
         *
         * This enum defines all possible token types recognized by the lexer.
         */
        enum class TokenType {
            // Keywords
            LET,            // let keyword
            CONST,          // const keyword
            FN,             // fn keyword
            ASYNC,          // async keyword
            AWAIT,          // await keyword
            COROUTINE,      // coroutine keyword
            STRUCT,         // struct keyword
            PROCESS,        // process keyword
            CLASS,          // class keyword
            INTERFACE,      // interface keyword
            IF,             // if keyword
            ELSE,           // else keyword
            FOR,            // for keyword
            WHILE,          // while keyword
            RETURN,         // return keyword
            BREAK,          // break keyword
            CONTINUE,       // continue keyword
            IMPORT,         // import keyword
            EXPORT,         // export keyword
            USE,            // use keyword
            NAMESPACE,      // namespace keyword
            CONSTRUCTOR,    // constructor keyword
            THIS,           // this keyword
            NEW,            // new keyword
            YIELD,          // yield keyword
            TRY,            // try keyword
            CATCH,          // catch keyword
            FINALLY,        // finally keyword
            NULL_,          // null keyword
            TRUE_,          // true keyword
            FALSE_,         // false keyword
            TYPEOF,         // typeof keyword
            INSTANCEOF,     // instanceof keyword
            AS,             // as keyword
            
            // Operators
            PLUS,           // + operator
            MINUS,          // - operator
            MULTIPLY,       // * operator
            DIVIDE,         // / operator
            MODULO,         // % operator
            PLUS_PLUS,      // ++ operator
            MINUS_MINUS,    // -- operator
            ASSIGN,         // = operator
            PLUS_ASSIGN,    // += operator
            MINUS_ASSIGN,   // -= operator
            MULTIPLY_ASSIGN, // *= operator
            DIVIDE_ASSIGN,  // /= operator
            MODULO_ASSIGN,  // %= operator
            EQUAL,          // == operator
            NOT_EQUAL,      // != operator
            LESS,           // < operator
            LESS_EQUAL,     // <= operator
            GREATER,        // > operator
            GREATER_EQUAL,  // >= operator
            AND,            // && operator
            OR,             // || operator
            NOT,            // ! operator
            BIT_AND,        // & operator
            BIT_OR,         // | operator
            BIT_XOR,        // ^ operator
            BIT_NOT,        // ~ operator
            LEFT_SHIFT,     // << operator
            RIGHT_SHIFT,    // >> operator
            UNSIGNED_RIGHT_SHIFT, // >>> operator
            LEFT_SHIFT_ASSIGN, // <<= operator
            RIGHT_SHIFT_ASSIGN, // >>= operator
            UNSIGNED_RIGHT_SHIFT_ASSIGN, // >>>= operator
            BIT_AND_ASSIGN, // &= operator
            BIT_OR_ASSIGN,  // |= operator
            BIT_XOR_ASSIGN, // ^= operator
            
            // Punctuation
            SEMICOLON,      // ; punctuation
            COLON,          // : punctuation
            COMMA,          // , punctuation
            DOT,            // . punctuation
            LEFT_PAREN,     // ( punctuation
            RIGHT_PAREN,    // ) punctuation
            LEFT_BRACE,     // { punctuation
            RIGHT_BRACE,    // } punctuation
            LEFT_BRACKET,   // [ punctuation
            RIGHT_BRACKET,  // ] punctuation
            ARROW,          // => punctuation
            DOUBLE_COLON,   // :: punctuation
            
            // Literals
            IDENTIFIER,     // Identifier literal
            INTEGER,        // Integer literal
            FLOAT,          // Float literal
            STRING,         // String literal
            CHARACTER,      // Character literal
            
            // Special tokens
            END_OF_FILE,    // End of file token
            ERROR           // Error token
        };
        
        /**
         * Token class
         *
         * This class represents a token in the Nexus source code.
         */
        class Token {
            public:
            TokenType type;   // Token type
            std::string lexeme; // Token lexeme
            int line;        // Line number
            int column;      // Column number
            
            /**
             * Constructs a new token
             *
             * @param type Token type
             * @param lexeme Token lexeme
             * @param line Line number
             * @param column Column number
             */
            Token(TokenType type, const std::string& lexeme, int line, int column)
            : type(type), lexeme(lexeme), line(line), column(column) {}
            
            /**
             * Converts the token to a string
             *
             * @return String representation of the token
             */
            std::string toString() const;
        };
        
    } // namespace frontend
} // namespace nexus

#endif