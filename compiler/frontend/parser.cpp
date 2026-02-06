#include "./parser.h"
#include <iostream>

namespace nexus {
namespace frontend {

/**
 * @brief Parser类的构造函数
 * 
 * 初始化解析器，设置词法分析器、错误标志和当前/前一个Token。
 * 
 * @param source 源代码字符串
 */
Parser::Parser(const std::string& source)
    : lexer(source), hadError(false), 
      currentToken(TokenType::END_OF_FILE, "", 0, 0),
      previousToken(TokenType::END_OF_FILE, "", 0, 0) {
    advance();
}

/**
 * @brief 前进到下一个Token
 * 
 * 更新当前Token为下一个有效的Token，跳过错误Token。
 */
void Parser::advance() {
    previousToken = currentToken;
    
    while (true) {
        currentToken = lexer.getNextToken();
        if (currentToken.type != TokenType::ERROR) {
            break;
        }
        error(currentToken.lexeme);
    }
}

/**
 * @brief 匹配指定类型的Token
 * 
 * 检查当前Token是否为指定类型，如果是则前进到下一个Token。
 * 
 * @param type 要匹配的Token类型
 * @return 如果匹配成功则返回true，否则返回false
 */
bool Parser::match(const TokenType& type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

/**
 * @brief 检查当前Token是否为指定类型
 * 
 * @param type 要检查的Token类型
 * @return 如果当前Token类型匹配则返回true，否则返回false
 */
bool Parser::check(const TokenType& type) {
    return currentToken.type == type;
}

/**
 * @brief 消费指定类型的Token
 * 
 * 检查当前Token是否为指定类型，如果是则前进到下一个Token并返回当前Token，否则报错。
 * 
 * @param type 要消费的Token类型
 * @param message 错误消息
 * @return 消费的Token
 */
Token Parser::consume(const TokenType& type, const std::string& message) {
    if (check(type)) {
        Token token = currentToken;
        advance();
        return token;
    }
    error(message);
    return currentToken;
}

/**
 * @brief 报告错误
 * 
 * 输出错误消息，设置错误标志。
 * 
 * @param message 错误消息
 */
void Parser::error(const std::string& message) {
    std::cerr << "Error at line " << currentToken.line << ", column " << currentToken.column << ": " << message << std::endl;
    hadError = true;
}

/**
 * @brief 同步解析器状态
 * 
 * 在遇到错误后，尝试同步解析器状态，跳过当前语句直到找到下一个语句的开始。
 */
void Parser::synchronize() {
    advance();
    
    while (currentToken.type != TokenType::END_OF_FILE) {
        if (previousToken.type == TokenType::SEMICOLON) {
            return;
        }
        
        switch (currentToken.type) {
            case TokenType::CLASS:
            case TokenType::STRUCT:
            case TokenType::FN:
            case TokenType::LET:
            case TokenType::CONST:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::FOR:
            case TokenType::TRY:
            case TokenType::RETURN:
            case TokenType::PROCESS:
                return;
            default:
                break;
        }
        
        advance();
    }
}

/**
 * @brief 解析整个源代码
 * 
 * 解析整个源代码，生成语句列表。
 * 
 * @return 语句列表
 */
std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    while (currentToken.type != TokenType::END_OF_FILE) {
        statements.push_back(statement());
    }
    
    return statements;
}

/**
 * @brief 解析表达式
 * 
 * 从最高优先级的表达式开始解析。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::expression() {
    return assignment();
}

/**
 * @brief 解析赋值表达式
 * 
 * 解析赋值表达式，处理变量赋值。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::assignment() {
    std::unique_ptr<Expr> expr = orExpr();
    
    if (match(TokenType::ASSIGN)) {
        Token equals = previousToken;
        std::unique_ptr<Expr> value = assignment();
        
        if (auto* identifierExpr = dynamic_cast<IdentifierExpr*>(expr.get())) {
            std::string name = identifierExpr->name;
            return std::make_unique<AssignExpr>(name, std::move(value));
        }
        
        error("Invalid assignment target.");
    }
    
    return expr;
}

/**
 * @brief 解析逻辑或表达式
 * 
 * 解析逻辑或表达式，处理 || 操作符。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::orExpr() {
    std::unique_ptr<Expr> expr = andExpr();
    
    while (match(TokenType::OR)) {
        Token op = previousToken;
        std::unique_ptr<Expr> right = andExpr();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op.lexeme, std::move(right));
    }
    
    return expr;
}

/**
 * @brief 解析逻辑与表达式
 * 
 * 解析逻辑与表达式，处理 && 操作符。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::andExpr() {
    std::unique_ptr<Expr> expr = equality();
    
    while (match(TokenType::AND)) {
        Token op = previousToken;
        std::unique_ptr<Expr> right = equality();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op.lexeme, std::move(right));
    }
    
    return expr;
}

/**
 * @brief 解析相等性表达式
 * 
 * 解析相等性表达式，处理 == 和 != 操作符。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();
    
    while (match(TokenType::EQUAL) || match(TokenType::NOT_EQUAL)) {
        Token op = previousToken;
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op.lexeme, std::move(right));
    }
    
    return expr;
}

/**
 * @brief 解析比较表达式
 * 
 * 解析比较表达式，处理 <, <=, >, >= 操作符。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();
    
    while (match(TokenType::LESS) || match(TokenType::LESS_EQUAL) || 
           match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL)) {
        Token op = previousToken;
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op.lexeme, std::move(right));
    }
    
    return expr;
}

/**
 * @brief 解析项表达式
 * 
 * 解析项表达式，处理 + 和 - 操作符。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();
    
    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        Token op = previousToken;
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op.lexeme, std::move(right));
    }
    
    return expr;
}

/**
 * @brief 解析因子表达式
 * 
 * 解析因子表达式，处理 *, /, % 操作符。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = unary();
    
    while (match(TokenType::MULTIPLY) || match(TokenType::DIVIDE) || match(TokenType::MODULO)) {
        Token op = previousToken;
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op.lexeme, std::move(right));
    }
    
    return expr;
}

/**
 * @brief 解析一元表达式
 * 
 * 解析一元表达式，处理 ! 和 - 操作符。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::unary() {
    if (match(TokenType::NOT) || match(TokenType::MINUS)) {
        Token op = previousToken;
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<UnaryExpr>(op.lexeme, std::move(right));
    }
    
    return call();
}

/**
 * @brief 解析成员访问表达式
 * 
 * 解析成员访问表达式，处理 . 和 [] 操作符。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::member() {
    std::unique_ptr<Expr> expr = primary();
    
    while (true) {
        if (match(TokenType::DOT)) {
            Token dot = previousToken;
            Token name = consume(TokenType::IDENTIFIER, "Expect property name after '.'.");
            expr = std::make_unique<MemberExpr>(std::move(expr), name.lexeme);
        } else if (match(TokenType::LEFT_BRACKET)) {
            std::unique_ptr<Expr> index = expression();
            consume(TokenType::RIGHT_BRACKET, "Expect ']' after index expression.");
            expr = std::make_unique<IndexExpr>(std::move(expr), std::move(index));
        } else {
            break;
        }
    }
    
    return expr;
}

/**
 * @brief 解析基本表达式
 * 
 * 解析基本表达式，包括字面量、标识符、this、括号表达式等。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::primary() {
    if (match(TokenType::FALSE_)) {
        return std::make_unique<LiteralExpr>("false", "bool");
    }
    if (match(TokenType::TRUE_)) {
        return std::make_unique<LiteralExpr>("true", "bool");
    }
    if (match(TokenType::NULL_)) {
        return std::make_unique<LiteralExpr>("null", "null");
    }
    
    if (match(TokenType::INTEGER)) {
        return std::make_unique<LiteralExpr>(previousToken.lexeme, "number");
    }
    if (match(TokenType::FLOAT)) {
        return std::make_unique<LiteralExpr>(previousToken.lexeme, "number");
    }
    if (match(TokenType::STRING)) {
        return std::make_unique<LiteralExpr>(previousToken.lexeme, "string");
    }
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<IdentifierExpr>(previousToken.lexeme);
    }
    
    if (match(TokenType::THIS)) {
        return std::make_unique<ThisExpr>();
    }
    
    if (match(TokenType::LEFT_PAREN)) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }
    
    error("Expect expression.");
    return std::make_unique<LiteralExpr>("null", "null");
}

/**
 * @brief 解析函数调用表达式
 * 
 * 解析函数调用表达式，处理函数调用和方法调用。
 * 
 * @return 表达式节点
 */
std::unique_ptr<Expr> Parser::call() {
    std::unique_ptr<Expr> expr = member();
    
    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            std::vector<std::unique_ptr<Expr>> arguments;
            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    arguments.push_back(expression());
                } while (match(TokenType::COMMA));
            }
            consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
            expr = std::make_unique<CallExpr>(std::move(expr), std::move(arguments));
        } else {
            break;
        }
    }
    
    return expr;
}

/**
 * @brief 解析语句
 * 
 * 根据当前Token类型解析不同类型的语句。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::statement() {
    if (match(TokenType::LET)) {
        return varStatement();
    }
    if (match(TokenType::CONST)) {
        return constStatement();
    }
    if (match(TokenType::FN)) {
        return functionStatement();
    }
    if (match(TokenType::CLASS)) {
        return classStatement();
    }
    if (match(TokenType::STRUCT)) {
        return structStatement();
    }
    if (match(TokenType::IF)) {
        return ifStatement();
    }
    if (match(TokenType::WHILE)) {
        return whileStatement();
    }
    if (match(TokenType::FOR)) {
        return forStatement();
    }
    if (match(TokenType::RETURN)) {
        return returnStatement();
    }
    if (match(TokenType::TRY)) {
        return tryStatement();
    }
    if (match(TokenType::LEFT_BRACE)) {
        return blockStatement();
    }
    if (match(TokenType::PROCESS)) {
        return processStatement();
    }
    
    return expressionStatement();
}

/**
 * @brief 解析表达式语句
 * 
 * 解析表达式语句，即单个表达式后跟分号。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

/**
 * @brief 解析代码块语句
 * 
 * 解析代码块语句，即由花括号包围的语句列表。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::blockStatement() {
    std::vector<std::unique_ptr<Stmt>> statements;
    
    while (!check(TokenType::RIGHT_BRACE) && currentToken.type != TokenType::END_OF_FILE) {
        statements.push_back(statement());
    }
    
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return std::make_unique<BlockStmt>(std::move(statements));
}

/**
 * @brief 解析if语句
 * 
 * 解析if语句，包括条件、then分支和else分支。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    
    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    
    if (match(TokenType::ELSE)) {
        elseBranch = statement();
    }
    
    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

/**
 * @brief 解析while语句
 * 
 * 解析while语句，包括条件和循环体。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after while condition.");
    
    std::unique_ptr<Stmt> body = statement();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

/**
 * @brief 解析for语句
 * 
 * 解析for语句，包括初始化、条件、增量和循环体。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::forStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");
    
    std::unique_ptr<Stmt> initializer = nullptr;
    if (!match(TokenType::SEMICOLON)) {
        if (match(TokenType::LET)) {
            initializer = varStatement();
        } else if (match(TokenType::CONST)) {
            initializer = constStatement();
        } else {
            initializer = expressionStatement();
        }
    }
    
    std::unique_ptr<Expr> condition = nullptr;
    if (!match(TokenType::SEMICOLON)) {
        condition = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after for condition.");
    }
    
    std::unique_ptr<Expr> increment = nullptr;
    if (!match(TokenType::RIGHT_PAREN)) {
        increment = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after for increment.");
    }
    
    std::unique_ptr<Stmt> body = statement();
    return std::make_unique<ForStmt>(std::move(initializer), std::move(condition), std::move(increment), std::move(body));
}

/**
 * @brief 解析return语句
 * 
 * 解析return语句，包括返回值。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previousToken;
    std::unique_ptr<Expr> value = nullptr;
    
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<ReturnStmt>(std::move(value));
}

/**
 * @brief 解析类型
 * 
 * 解析类型名称，目前只支持简单标识符类型。
 * 
 * @return 类型名称
 */
std::string Parser::parseType() {
    std::string type;
    if (match(TokenType::IDENTIFIER)) {
        type = previousToken.lexeme;
    }
    return type;
}

/**
 * @brief 解析变量声明语句
 * 
 * 解析变量声明语句，包括变量名、类型和初始化表达式。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::varStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    
    std::string type;
    if (match(TokenType::COLON)) {
        type = parseType();
    }
    
    std::unique_ptr<Expr> initializer = nullptr;
    if (match(TokenType::ASSIGN)) {
        initializer = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<VarStmt>(name.lexeme, type, std::move(initializer));
}

/**
 * @brief 解析常量声明语句
 * 
 * 解析常量声明语句，包括常量名、类型和初始化表达式。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::constStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect constant name.");
    
    std::string type;
    if (match(TokenType::COLON)) {
        type = parseType();
    }
    
    consume(TokenType::ASSIGN, "Expect '=' after constant name.");
    std::unique_ptr<Expr> initializer = expression();
    
    consume(TokenType::SEMICOLON, "Expect ';' after constant declaration.");
    return std::make_unique<ConstStmt>(name.lexeme, type, std::move(initializer));
}

/**
 * @brief 解析函数参数
 * 
 * 解析函数参数列表，包括参数名和类型。
 * 
 * @return 参数列表
 */
std::vector<std::pair<std::string, std::string>> Parser::parseParameters() {
    std::vector<std::pair<std::string, std::string>> parameters;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            Token name = consume(TokenType::IDENTIFIER, "Expect parameter name.");
            std::string type;
            if (match(TokenType::COLON)) {
                type = parseType();
            }
            parameters.emplace_back(name.lexeme, type);
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    return parameters;
}

/**
 * @brief 解析函数声明语句
 * 
 * 解析函数声明语句，包括函数名、参数、返回类型和函数体。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::functionStatement() {
    bool isAsync = false;
    bool isCoroutine = false;
    
    if (match(TokenType::ASYNC)) {
        isAsync = true;
    }
    if (match(TokenType::COROUTINE)) {
        isCoroutine = true;
    }
    
    Token name = consume(TokenType::IDENTIFIER, "Expect function name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after function name.");
    
    std::vector<std::pair<std::string, std::string>> parameters = parseParameters();
    
    std::string returnType;
    if (match(TokenType::COLON)) {
        returnType = parseType();
    }
    
    consume(TokenType::LEFT_BRACE, "Expect '{' before function body.");
    std::unique_ptr<Stmt> body = blockStatement();
    
    return std::make_unique<FunctionStmt>(name.lexeme, std::move(parameters), returnType, std::move(body), isAsync, isCoroutine);
}

/**
 * @brief 解析类声明语句
 * 
 * 解析类声明语句，包括类名、父类和方法列表。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::classStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect class name.");
    
    std::string superclass;
    if (match(TokenType::LESS)) {
        Token superToken = consume(TokenType::IDENTIFIER, "Expect superclass name.");
        superclass = superToken.lexeme;
    }
    
    consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");
    
    std::vector<std::unique_ptr<Stmt>> methods;
    while (!check(TokenType::RIGHT_BRACE) && currentToken.type != TokenType::END_OF_FILE) {
        methods.push_back(functionStatement());
    }
    
    consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");
    return std::make_unique<ClassStmt>(name.lexeme, superclass, std::move(methods));
}

/**
 * @brief 解析结构体声明语句
 * 
 * 解析结构体声明语句，包括结构体名和字段列表。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::structStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expect struct name.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before struct body.");
    
    std::vector<std::pair<std::string, std::string>> fields;
    if (!check(TokenType::RIGHT_BRACE)) {
        do {
            Token fieldName = consume(TokenType::IDENTIFIER, "Expect field name.");
            consume(TokenType::COLON, "Expect ':' after field name.");
            std::string fieldType = parseType();
            fields.emplace_back(fieldName.lexeme, fieldType);
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RIGHT_BRACE, "Expect '}' after struct body.");
    return std::make_unique<StructStmt>(name.lexeme, std::move(fields));
}

/**
 * @brief 解析try语句
 * 
 * 解析try语句，包括try块、catch块和finally块。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::tryStatement() {
    consume(TokenType::LEFT_BRACE, "Expect '{' before try body.");
    std::unique_ptr<Stmt> body = blockStatement();
    
    std::vector<std::unique_ptr<Stmt>> catches;
    while (match(TokenType::CATCH)) {
        catches.push_back(catchStatement());
    }
    
    std::unique_ptr<Stmt> finallyStmt = nullptr;
    if (match(TokenType::FINALLY)) {
        finallyStmt = finallyStatement();
    }
    
    return std::make_unique<TryStmt>(std::move(body), std::move(catches), std::move(finallyStmt));
}

/**
 * @brief 解析catch语句
 * 
 * 解析catch语句，包括异常参数和catch块。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::catchStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'catch'.");
    Token name = consume(TokenType::IDENTIFIER, "Expect catch parameter name.");
    
    std::string type;
    if (match(TokenType::COLON)) {
        type = parseType();
    }
    
    consume(TokenType::RIGHT_PAREN, "Expect ')' after catch parameter.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before catch body.");
    
    std::unique_ptr<Stmt> body = blockStatement();
    return std::make_unique<CatchStmt>(name.lexeme, type, std::move(body));
}

/**
 * @brief 解析finally语句
 * 
 * 解析finally语句，包括finally块。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::finallyStatement() {
    consume(TokenType::LEFT_BRACE, "Expect '{' before finally body.");
    return blockStatement();
}

/**
 * @brief 解析throw语句
 * 
 * 解析throw语句，包括抛出的表达式。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::throwStatement() {
    std::unique_ptr<Expr> expression = this->expression();
    consume(TokenType::SEMICOLON, "Expect ';' after throw expression.");
    return std::make_unique<ThrowStmt>(std::move(expression));
}

/**
 * @brief 解析进程声明语句
 * 
 * 解析进程声明语句，目前只支持process.spawn()形式。
 * 
 * @return 语句节点
 */
std::unique_ptr<Stmt> Parser::processStatement() {
    consume(TokenType::DOT, "Expect '.' after 'process'.");
    Token spawnToken = consume(TokenType::IDENTIFIER, "Expect 'spawn' after 'process.'.");
    if (spawnToken.lexeme != "spawn") {
        error("Expect 'spawn' after 'process.'.");
    }
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'process.spawn'.");
    
    // 简化处理，暂时只支持 lambda 表达式
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'process.spawn'.");
    consume(TokenType::RIGHT_PAREN, "Expect ')' after lambda parameters.");
    consume(TokenType::ARROW, "Expect '=>' after lambda parameters.");
    
    std::unique_ptr<Expr> body;
    if (match(TokenType::LEFT_BRACE)) {
        // 代码块
        blockStatement();
        body = std::make_unique<LiteralExpr>("block", "block");
    } else {
        // 表达式
        body = expression();
    }
    
    consume(TokenType::RIGHT_PAREN, "Expect ')' after process.spawn body.");
    consume(TokenType::SEMICOLON, "Expect ';' after process.spawn statement.");
    
    return std::make_unique<ProcessStmt>("spawn", std::move(body));
}

} // namespace frontend
} // namespace nexus
