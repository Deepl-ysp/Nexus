#include "./token.h"

#include <sstream>

namespace nexus {
namespace frontend {

/**
 * @brief 将TokenType枚举转换为字符串表示
 * 
 * 此函数接收一个TokenType枚举值，并返回其对应的字符串表示形式，
 * 用于调试和错误报告。
 * 
 * @param type TokenType枚举值
 * @return 对应的字符串表示
 */
std::string tokenTypeToString(TokenType type) {
  switch (type) {
    // 关键字
    case TokenType::LET:
      return "LET";
    case TokenType::CONST:
      return "CONST";
    case TokenType::FN:
      return "FN";
    case TokenType::ASYNC:
      return "ASYNC";
    case TokenType::AWAIT:
      return "AWAIT";
    case TokenType::COROUTINE:
      return "COROUTINE";
    case TokenType::STRUCT:
      return "STRUCT";
    case TokenType::PROCESS:
      return "PROCESS";
    case TokenType::CLASS:
      return "CLASS";
    case TokenType::INTERFACE:
      return "INTERFACE";
    case TokenType::IF:
      return "IF";
    case TokenType::ELSE:
      return "ELSE";
    case TokenType::FOR:
      return "FOR";
    case TokenType::WHILE:
      return "WHILE";
    case TokenType::RETURN:
      return "RETURN";
    case TokenType::BREAK:
      return "BREAK";
    case TokenType::CONTINUE:
      return "CONTINUE";
    case TokenType::IMPORT:
      return "IMPORT";
    case TokenType::EXPORT:
      return "EXPORT";
    case TokenType::USE:
      return "USE";
    case TokenType::NAMESPACE:
      return "NAMESPACE";
    case TokenType::CONSTRUCTOR:
      return "CONSTRUCTOR";
    case TokenType::THIS:
      return "THIS";
    case TokenType::NEW:
      return "NEW";
    case TokenType::YIELD:
      return "YIELD";
    case TokenType::TRY:
      return "TRY";
    case TokenType::CATCH:
      return "CATCH";
    case TokenType::FINALLY:
      return "FINALLY";
    case TokenType::NULL_:
      return "NULL";
    case TokenType::TRUE_:
      return "TRUE";
    case TokenType::FALSE_:
      return "FALSE";
    case TokenType::TYPEOF:
      return "TYPEOF";
    case TokenType::INSTANCEOF:
      return "INSTANCEOF";
    case TokenType::AS:
      return "AS";

    // 操作符
    case TokenType::PLUS:
      return "PLUS";
    case TokenType::MINUS:
      return "MINUS";
    case TokenType::MULTIPLY:
      return "MULTIPLY";
    case TokenType::DIVIDE:
      return "DIVIDE";
    case TokenType::MODULO:
      return "MODULO";
    case TokenType::PLUS_PLUS:
      return "PLUS_PLUS";
    case TokenType::MINUS_MINUS:
      return "MINUS_MINUS";
    case TokenType::ASSIGN:
      return "ASSIGN";
    case TokenType::PLUS_ASSIGN:
      return "PLUS_ASSIGN";
    case TokenType::MINUS_ASSIGN:
      return "MINUS_ASSIGN";
    case TokenType::MULTIPLY_ASSIGN:
      return "MULTIPLY_ASSIGN";
    case TokenType::DIVIDE_ASSIGN:
      return "DIVIDE_ASSIGN";
    case TokenType::MODULO_ASSIGN:
      return "MODULO_ASSIGN";
    case TokenType::EQUAL: return "EQUAL";
    case TokenType::NOT_EQUAL: return "NOT_EQUAL";
    case TokenType::LESS: return "LESS";
    case TokenType::LESS_EQUAL: return "LESS_EQUAL";
    case TokenType::GREATER: return "GREATER";
    case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
    case TokenType::AND: return "AND";
    case TokenType::OR: return "OR";
    case TokenType::NOT: return "NOT";
    case TokenType::BIT_AND: return "BIT_AND";
    case TokenType::BIT_OR: return "BIT_OR";
    case TokenType::BIT_XOR: return "BIT_XOR";
    case TokenType::BIT_NOT: return "BIT_NOT";
    case TokenType::LEFT_SHIFT: return "LEFT_SHIFT";
    case TokenType::RIGHT_SHIFT: return "RIGHT_SHIFT";
    case TokenType::UNSIGNED_RIGHT_SHIFT: return "UNSIGNED_RIGHT_SHIFT";
    case TokenType::LEFT_SHIFT_ASSIGN: return "LEFT_SHIFT_ASSIGN";
    case TokenType::RIGHT_SHIFT_ASSIGN: return "RIGHT_SHIFT_ASSIGN";
    case TokenType::UNSIGNED_RIGHT_SHIFT_ASSIGN: return "UNSIGNED_RIGHT_SHIFT_ASSIGN";
    case TokenType::BIT_AND_ASSIGN: return "BIT_AND_ASSIGN";
    case TokenType::BIT_OR_ASSIGN: return "BIT_OR_ASSIGN";
    case TokenType::BIT_XOR_ASSIGN: return "BIT_XOR_ASSIGN";
        
    // 标点符号
    case TokenType::SEMICOLON: return "SEMICOLON";
    case TokenType::COLON: return "COLON";
    case TokenType::COMMA: return "COMMA";
    case TokenType::DOT: return "DOT";
    case TokenType::LEFT_PAREN: return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE: return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
    case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
    case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
    case TokenType::ARROW: return "ARROW";
    case TokenType::DOUBLE_COLON: return "DOUBLE_COLON";
        
    // 字面量
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::INTEGER: return "INTEGER";
    case TokenType::FLOAT: return "FLOAT";
    case TokenType::STRING: return "STRING";
    case TokenType::CHARACTER: return "CHARACTER";
        
    // 特殊标记
    case TokenType::END_OF_FILE: return "END_OF_FILE";
    case TokenType::ERROR: return "ERROR";
        
    default: return "UNKNOWN";
  }
}

/**
 * @brief 将Token对象转换为字符串表示
 * 
 * 此方法生成Token对象的详细字符串表示，包括其类型、词素、行号和列号，
 * 用于调试和错误报告。
 * 
 * @return Token对象的字符串表示
 */
std::string Token::toString() const {
  std::stringstream ss;
  ss << "Token{type=" << tokenTypeToString(type) 
     << ", lexeme=\"" << lexeme << "\""
     << ", line=" << line 
     << ", column=" << column << "}";
  return ss.str();
}

} // namespace frontend
} // namespace nexus
