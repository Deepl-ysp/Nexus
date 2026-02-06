#include "lexer.h"

#include <cctype>
#include <stdexcept>

namespace nexus {
namespace frontend {

/**
 * @brief Lexer类的构造函数
 * 
 * 初始化词法分析器，设置源代码、位置指针和行号列号，并初始化关键字映射。
 * 
 * @param source 源代码字符串
 */
Lexer::Lexer(const std::string& source)
    : source(source), position(0), line(1), column(1), ungotToken(nullptr) {
  initKeywords();
}

/**
 * @brief 初始化关键字映射
 * 
 * 填充关键字到TokenType的映射表，用于识别语言关键字。
 */
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

/**
 * @brief 获取下一个Token
 * 
 * 从源代码中获取下一个Token，如果有回退的Token则返回它，否则扫描新的Token。
 * 
 * @return 下一个Token对象
 */
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

/**
 * @brief 回退一个Token
 * 
 * 将之前获取的Token回退，以便下次调用getNextToken时重新返回它。
 * 
 * @param token 要回退的Token
 */
void Lexer::ungetToken(const Token& token) {
  if (ungotToken != nullptr) {
    delete ungotToken;
  }
  ungotToken = new Token(token);
}

/**
 * @brief 查看当前字符
 * 
 * 查看当前位置的字符，但不移动位置指针。
 * 
 * @return 当前字符，如果到达文件末尾则返回 '\0'
 */
char Lexer::peek() const {
  if (isAtEnd()) {
    return '\0';
  }
  return source[position];
}

/**
 * @brief 前进一个字符
 * 
 * 移动位置指针到下一个字符，并更新行号和列号。
 * 
 * @return 移动前的字符
 */
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

/**
 * @brief 匹配预期字符
 * 
 * 检查当前字符是否与预期字符匹配，如果匹配则前进位置指针。
 * 
 * @param expected 预期的字符
 * @return 如果匹配则返回true，否则返回false
 */
bool Lexer::match(char expected) {
  if (isAtEnd() || source[position] != expected) {
    return false;
  }
  position++;
  column++;
  return true;
}

/**
 * @brief 检查是否到达文件末尾
 * 
 * 检查当前位置是否已经到达源代码的末尾。
 * 
 * @return 如果到达文件末尾则返回true，否则返回false
 */
bool Lexer::isAtEnd() const {
  return position >= source.length();
}

/**
 * @brief 跳过空白和注释
 * 
 * 跳过源代码中的空白字符、单行注释和多行注释。
 */
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

/**
 * @brief 跳过单行注释
 * 
 * 跳过从当前位置到行尾的所有字符。
 */
void Lexer::skipComment() {
  while (!isAtEnd() && peek() != '\n') {
    advance();
  }
}

/**
 * @brief 扫描Token
 * 
 * 根据当前字符扫描并生成相应的Token。
 * 
 * @return 生成的Token对象
 */
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

/**
 * @brief 处理标识符
 * 
 * 扫描并处理标识符，检查是否是关键字。
 * 
 * @return 标识符或关键字对应的Token
 */
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

/**
 * @brief 处理数字
 * 
 * 扫描并处理数字字面量，支持整数和浮点数。
 * 
 * @return 数字对应的Token
 */
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

/**
 * @brief 处理字符串
 * 
 * 扫描并处理字符串字面量，支持转义字符。
 * 
 * @return 字符串对应的Token
 */
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

/**
 * @brief 处理字符
 * 
 * 扫描并处理字符字面量，支持转义字符。
 * 
 * @return 字符对应的Token
 */
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

/**
 * @brief 创建Token
 * 
 * 根据类型和词素创建Token对象，设置正确的行号和列号。
 * 
 * @param type Token类型
 * @param lexeme Token词素
 * @return 创建的Token对象
 */
Token Lexer::makeToken(TokenType type, const std::string& lexeme) {
  int tokenLine = line;
  int tokenColumn = column - lexeme.length();
  return Token(type, lexeme, tokenLine, tokenColumn);
}

/**
 * @brief 创建错误Token
 * 
 * 创建表示错误的Token对象。
 * 
 * @param message 错误消息
 * @return 错误Token对象
 */
Token Lexer::errorToken(const std::string& message) {
  return Token(TokenType::ERROR, message, line, column);
}

} // namespace frontend
} // namespace nexus
