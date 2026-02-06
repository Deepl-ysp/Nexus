/*
 * Nexus Compiler
 * Lexer definitions
 *
 * This file defines the lexer class used by the Nexus compiler
 * to tokenize source code into a stream of tokens.
 */

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>

#include "./token.h"

namespace nexus {
namespace frontend {

/**
 * Lexer class
 *
 * This class is responsible for tokenizing Nexus source code
 * into a stream of tokens for the parser.
 */
class Lexer {
 public:
  /**
   * Constructs a new lexer
   *
   * @param source The source code to tokenize
   */
  Lexer(const std::string& source);
    
    /**
     * Gets the next token from the source code
     *
     * @return The next token
     */
    Token getNextToken();
    
    /**
     * Puts back a token to be read again
     *
     * @param token The token to put back
     */
    void ungetToken(const Token& token);
    
private:
    // Source code
    std::string source;
    // Current position in the source code
    size_t position;
    // Current line number
    int line;
    // Current column number
    int column;
    // Ungot token
    Token* ungotToken;
    // Keyword mapping
    std::unordered_map<std::string, TokenType> keywords;
    
    /**
     * Initializes the keyword mapping
     */
    void initKeywords();
    
    /**
     * Helper methods
     */
    char peek() const;
    char advance();
    bool match(char expected);
    bool isAtEnd() const;
    
    /**
     * Lexical analysis methods
     */
    Token scanToken();
    Token identifier();
    Token number();
    Token string();
    Token character();
    
    /**
     * Skips whitespace and comments
     */
    void skipWhitespace();
    void skipComment();
    
    /**
     * Creates tokens
     */
    Token makeToken(TokenType type, const std::string& lexeme);
    Token errorToken(const std::string& message);
};

} // namespace frontend
} // namespace nexus

#endif // LEXER_H
