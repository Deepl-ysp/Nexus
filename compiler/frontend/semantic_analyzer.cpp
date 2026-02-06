#include "./semantic_analyzer.h"
#include <iostream>

namespace nexus {
namespace frontend {

/**
 * @brief SemanticAnalyzer类的构造函数
 * 
 * 初始化语义分析器，创建全局作用域并定义内置类型和函数。
 */
SemanticAnalyzer::SemanticAnalyzer()
    : _hadError(false) {
    // 初始化全局作用域
    enterScope();
    
    // 定义内置类型和函数
    defineVariable("print", "function");
    defineVariable("println", "function");
    defineVariable("error", "function");
    defineVariable("assert", "function");
    defineVariable("len", "function");
    defineVariable("toString", "function");
    defineVariable("parseInt", "function");
    defineVariable("parseFloat", "function");
    defineVariable("isNaN", "function");
    defineVariable("isFinite", "function");
    defineVariable("Math", "object");
    defineVariable("Date", "object");
    defineVariable("Array", "object");
    defineVariable("Object", "object");
    defineVariable("String", "object");
    defineVariable("Number", "object");
    defineVariable("Boolean", "object");
    defineVariable("Error", "object");
}

/**
 * @brief 检查是否有错误
 * 
 * @return 如果有错误则返回true，否则返回false
 */
bool SemanticAnalyzer::hadError() const {
    return _hadError;
}

/**
 * @brief 报告语义错误
 * 
 * @param message 错误消息
 */
void SemanticAnalyzer::error(const std::string& message) {
    std::cerr << "Semantic error: " << message << std::endl;
    _hadError = true;
}

/**
 * @brief 进入新的作用域
 * 
 * 创建并进入一个新的作用域。
 */
void SemanticAnalyzer::enterScope() {
    scopes.push_back(Scope());
}

/**
 * @brief 退出当前作用域
 * 
 * 退出并销毁当前作用域。
 */
void SemanticAnalyzer::exitScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

/**
 * @brief 检查变量是否已定义
 * 
 * @param name 变量名
 * @return 如果变量已定义则返回true，否则返回false
 */
bool SemanticAnalyzer::isVariableDefined(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->variables.find(name) != it->variables.end()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief 检查函数是否已定义
 * 
 * @param name 函数名
 * @return 如果函数已定义则返回true，否则返回false
 */
bool SemanticAnalyzer::isFunctionDefined(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->functions.find(name) != it->functions.end()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief 检查结构体是否已定义
 * 
 * @param name 结构体名
 * @return 如果结构体已定义则返回true，否则返回false
 */
bool SemanticAnalyzer::isStructDefined(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->structs.find(name) != it->structs.end()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief 定义变量
 * 
 * 在当前作用域中定义一个变量。
 * 
 * @param name 变量名
 * @param type 变量类型
 */
void SemanticAnalyzer::defineVariable(const std::string& name, const std::string& type) {
    if (!scopes.empty()) {
        scopes.back().variables[name] = type;
    }
}

/**
 * @brief 定义函数
 * 
 * 在当前作用域中定义一个函数。
 * 
 * @param name 函数名
 * @param returnType 返回类型
 */
void SemanticAnalyzer::defineFunction(const std::string& name, const std::string& returnType) {
    if (!scopes.empty()) {
        scopes.back().functions[name] = returnType;
    }
}

/**
 * @brief 定义结构体
 * 
 * 在当前作用域中定义一个结构体。
 * 
 * @param name 结构体名
 * @param fields 字段列表
 */
void SemanticAnalyzer::defineStruct(const std::string& name, const std::vector<std::pair<std::string, std::string>>& fields) {
    if (!scopes.empty()) {
        scopes.back().structs[name] = fields;
    }
}

/**
 * @brief 分析语句列表
 * 
 * 分析一系列语句的语义。
 * 
 * @param statements 语句列表
 */
void SemanticAnalyzer::analyze(const std::vector<std::unique_ptr<Stmt>>& statements) {
    for (const auto& stmt : statements) {
        analyzeStatement(stmt.get());
    }
}

/**
 * @brief 分析语句
 * 
 * 根据语句类型分析语句的语义。
 * 
 * @param stmt 语句指针
 */
void SemanticAnalyzer::analyzeStatement(const Stmt* stmt) {
    if (!stmt) return;
    
    switch (stmt->getType()) {
        case StmtType::BLOCK:
            analyzeBlockStatement(static_cast<const BlockStmt*>(stmt));
            break;
        case StmtType::VAR:
            analyzeVarStatement(static_cast<const VarStmt*>(stmt));
            break;
        case StmtType::CONST:
            analyzeConstStatement(static_cast<const ConstStmt*>(stmt));
            break;
        case StmtType::FUNCTION:
            analyzeFunctionStatement(static_cast<const FunctionStmt*>(stmt));
            break;
        case StmtType::CLASS:
            analyzeClassStatement(static_cast<const ClassStmt*>(stmt));
            break;
        case StmtType::STRUCT:
            analyzeStructStatement(static_cast<const StructStmt*>(stmt));
            break;
        case StmtType::IF:
            analyzeIfStatement(static_cast<const IfStmt*>(stmt));
            break;
        case StmtType::WHILE:
            analyzeWhileStatement(static_cast<const WhileStmt*>(stmt));
            break;
        case StmtType::FOR:
            analyzeForStatement(static_cast<const ForStmt*>(stmt));
            break;
        case StmtType::RETURN:
            analyzeReturnStatement(static_cast<const ReturnStmt*>(stmt));
            break;
        case StmtType::EXPRESSION:
            analyzeExpressionStatement(static_cast<const ExpressionStmt*>(stmt));
            break;
        case StmtType::PRINT:
            analyzePrintStatement(static_cast<const PrintStmt*>(stmt));
            break;
        case StmtType::TRY:
            analyzeTryStatement(static_cast<const TryStmt*>(stmt));
            break;
        case StmtType::CATCH:
            analyzeCatchStatement(static_cast<const CatchStmt*>(stmt));
            break;
        case StmtType::THROW:
            analyzeThrowStatement(static_cast<const ThrowStmt*>(stmt));
            break;
        case StmtType::PROCESS:
            analyzeProcessStatement(static_cast<const ProcessStmt*>(stmt));
            break;
        default:
            break;
    }
}

/**
 * @brief 分析代码块语句
 * 
 * 分析代码块语句的语义，包括进入和退出作用域。
 * 
 * @param stmt 代码块语句指针
 */
void SemanticAnalyzer::analyzeBlockStatement(const BlockStmt* stmt) {
    enterScope();
    for (const auto& statement : stmt->statements) {
        analyzeStatement(statement.get());
    }
    exitScope();
}

/**
 * @brief 分析变量声明语句
 * 
 * 分析变量声明语句的语义，包括变量名检查、类型推断和定义。
 * 
 * @param stmt 变量声明语句指针
 */
void SemanticAnalyzer::analyzeVarStatement(const VarStmt* stmt) {
    if (isVariableDefined(stmt->name)) {
        error("Variable '" + stmt->name + "' is already defined.");
    }
    
    std::string type = stmt->type;
    if (type.empty()) {
        if (stmt->initializer) {
            type = analyzeExpression(stmt->initializer.get());
        } else {
            type = "any";
        }
    }
    
    // 验证类型是否有效
    if (type != "any" && type != "int" && type != "float" && type != "bool" && type != "string" && !isStructDefined(type)) {
        error("Unknown type '" + type + "'.");
    }
    
    defineVariable(stmt->name, type);
    
    if (stmt->initializer) {
        std::string initType = analyzeExpression(stmt->initializer.get());
        if (type != "any" && initType != type) {
            error("Type mismatch: expected '" + type + "', got '" + initType + "'.");
        }
    }
}

/**
 * @brief 分析常量声明语句
 * 
 * 分析常量声明语句的语义，包括常量名检查、类型推断和定义。
 * 
 * @param stmt 常量声明语句指针
 */
void SemanticAnalyzer::analyzeConstStatement(const ConstStmt* stmt) {
    if (isVariableDefined(stmt->name)) {
        error("Constant '" + stmt->name + "' is already defined.");
    }
    
    std::string type = stmt->type;
    if (type.empty()) {
        if (stmt->initializer) {
            type = analyzeExpression(stmt->initializer.get());
        } else {
            type = "any";
        }
    }
    
    // 验证类型是否有效
    if (type != "any" && type != "int" && type != "float" && type != "bool" && type != "string" && !isStructDefined(type)) {
        error("Unknown type '" + type + "'.");
    }
    
    defineVariable(stmt->name, type);
    
    if (stmt->initializer) {
        std::string initType = analyzeExpression(stmt->initializer.get());
        if (type != "any" && initType != type) {
            error("Type mismatch: expected '" + type + "', got '" + initType + "'.");
        }
    } else {
        error("Constant '" + stmt->name + "' must be initialized.");
    }
}

/**
 * @brief 分析函数声明语句
 * 
 * 分析函数声明语句的语义，包括函数名检查、参数定义和函数体分析。
 * 
 * @param stmt 函数声明语句指针
 */
void SemanticAnalyzer::analyzeFunctionStatement(const FunctionStmt* stmt) {
    if (isFunctionDefined(stmt->name)) {
        error("Function '" + stmt->name + "' is already defined.");
    }
    
    defineFunction(stmt->name, stmt->returnType);
    
    enterScope();
    
    // 定义函数参数
    for (const auto& param : stmt->parameters) {
        defineVariable(param.first, param.second.empty() ? "any" : param.second);
    }
    
    // 分析函数体
    analyzeStatement(stmt->body.get());
    
    exitScope();
}

/**
 * @brief 分析类声明语句
 * 
 * 分析类声明语句的语义，包括类名检查、父类检查和方法分析。
 * 
 * @param stmt 类声明语句指针
 */
void SemanticAnalyzer::analyzeClassStatement(const ClassStmt* stmt) {
    // 简化处理，暂时只检查类名是否已定义
    if (isStructDefined(stmt->name)) {
        error("Class '" + stmt->name + "' is already defined.");
    }
    
    // 检查父类是否存在
    if (!stmt->superclass.empty() && !isStructDefined(stmt->superclass)) {
        error("Superclass '" + stmt->superclass + "' is not defined.");
    }
    
    enterScope();
    
    // 定义类的方法
    for (const auto& method : stmt->methods) {
        analyzeStatement(method.get());
    }
    
    exitScope();
}

/**
 * @brief 分析结构体声明语句
 * 
 * 分析结构体声明语句的语义，包括结构体名检查和定义。
 * 
 * @param stmt 结构体声明语句指针
 */
void SemanticAnalyzer::analyzeStructStatement(const StructStmt* stmt) {
    if (isStructDefined(stmt->name)) {
        error("Struct '" + stmt->name + "' is already defined.");
    }
    
    defineStruct(stmt->name, stmt->fields);
}

/**
 * @brief 分析if语句
 * 
 * 分析if语句的语义，包括条件表达式分析和分支分析。
 * 
 * @param stmt if语句指针
 */
void SemanticAnalyzer::analyzeIfStatement(const IfStmt* stmt) {
    // 分析条件表达式
    std::string conditionType = analyzeExpression(stmt->condition.get());
    if (conditionType != "bool" && conditionType != "any") {
        error("If condition must be a boolean, got '" + conditionType + "'.");
    }
    
    // 分析 then 分支
    analyzeStatement(stmt->thenBranch.get());
    
    // 分析 else 分支
    if (stmt->elseBranch) {
        analyzeStatement(stmt->elseBranch.get());
    }
}

/**
 * @brief 分析while语句
 * 
 * 分析while语句的语义，包括条件表达式分析和循环体分析。
 * 
 * @param stmt while语句指针
 */
void SemanticAnalyzer::analyzeWhileStatement(const WhileStmt* stmt) {
    // 分析条件表达式
    std::string conditionType = analyzeExpression(stmt->condition.get());
    if (conditionType != "bool" && conditionType != "any") {
        error("While condition must be a boolean, got '" + conditionType + "'.");
    }
    
    // 分析循环体
    analyzeStatement(stmt->body.get());
}

/**
 * @brief 分析for语句
 * 
 * 分析for语句的语义，包括初始化、条件、增量和循环体分析。
 * 
 * @param stmt for语句指针
 */
void SemanticAnalyzer::analyzeForStatement(const ForStmt* stmt) {
    // 分析初始化语句
    if (stmt->initializer) {
        analyzeStatement(stmt->initializer.get());
    }
    
    // 分析条件表达式
    if (stmt->condition) {
        std::string conditionType = analyzeExpression(stmt->condition.get());
        if (conditionType != "bool" && conditionType != "any") {
            error("For condition must be a boolean, got '" + conditionType + "'.");
        }
    }
    
    // 分析递增表达式
    if (stmt->increment) {
        analyzeExpression(stmt->increment.get());
    }
    
    // 分析循环体
    analyzeStatement(stmt->body.get());
}

/**
 * @brief 分析return语句
 * 
 * 分析return语句的语义，包括返回值表达式分析。
 * 
 * @param stmt return语句指针
 */
void SemanticAnalyzer::analyzeReturnStatement(const ReturnStmt* stmt) {
    // 分析返回值表达式
    if (stmt->value) {
        analyzeExpression(stmt->value.get());
    }
}

/**
 * @brief 分析表达式语句
 * 
 * 分析表达式语句的语义，包括表达式分析。
 * 
 * @param stmt 表达式语句指针
 */
void SemanticAnalyzer::analyzeExpressionStatement(const ExpressionStmt* stmt) {
    analyzeExpression(stmt->expression.get());
}

/**
 * @brief 分析打印语句
 * 
 * 分析打印语句的语义，包括表达式分析。
 * 
 * @param stmt 打印语句指针
 */
void SemanticAnalyzer::analyzePrintStatement(const PrintStmt* stmt) {
    analyzeExpression(stmt->expression.get());
}

/**
 * @brief 分析try语句
 * 
 * 分析try语句的语义，包括try块、catch块和finally块分析。
 * 
 * @param stmt try语句指针
 */
void SemanticAnalyzer::analyzeTryStatement(const TryStmt* stmt) {
    // 分析 try 体
    analyzeStatement(stmt->body.get());
    
    // 分析 catch 块
    for (const auto& catchStmt : stmt->catches) {
        analyzeStatement(catchStmt.get());
    }
    
    // 分析 finally 块
    if (stmt->finally) {
        analyzeStatement(stmt->finally.get());
    }
}

/**
 * @brief 分析catch语句
 * 
 * 分析catch语句的语义，包括异常变量定义和catch块分析。
 * 
 * @param stmt catch语句指针
 */
void SemanticAnalyzer::analyzeCatchStatement(const CatchStmt* stmt) {
    enterScope();
    
    // 定义异常变量
    defineVariable(stmt->name, stmt->type.empty() ? "Error" : stmt->type);
    
    // 分析 catch 体
    analyzeStatement(stmt->body.get());
    
    exitScope();
}

/**
 * @brief 分析throw语句
 * 
 * 分析throw语句的语义，包括表达式分析。
 * 
 * @param stmt throw语句指针
 */
void SemanticAnalyzer::analyzeThrowStatement(const ThrowStmt* stmt) {
    analyzeExpression(stmt->expression.get());
}

/**
 * @brief 分析进程声明语句
 * 
 * 分析进程声明语句的语义，包括表达式分析。
 * 
 * @param stmt 进程声明语句指针
 */
void SemanticAnalyzer::analyzeProcessStatement(const ProcessStmt* stmt) {
    analyzeExpression(stmt->body.get());
}

/**
 * @brief 分析表达式
 * 
 * 根据表达式类型分析表达式的语义并返回其类型。
 * 
 * @param expr 表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeExpression(const Expr* expr) {
    if (!expr) return "any";
    
    switch (expr->getType()) {
        case ExprType::BINARY:
            return analyzeBinaryExpr(static_cast<const BinaryExpr*>(expr));
        case ExprType::UNARY:
            return analyzeUnaryExpr(static_cast<const UnaryExpr*>(expr));
        case ExprType::LITERAL:
            return analyzeLiteralExpr(static_cast<const LiteralExpr*>(expr));
        case ExprType::IDENTIFIER:
            return analyzeIdentifierExpr(static_cast<const IdentifierExpr*>(expr));
        case ExprType::ASSIGN:
            return analyzeAssignExpr(static_cast<const AssignExpr*>(expr));
        case ExprType::CALL:
            return analyzeCallExpr(static_cast<const CallExpr*>(expr));
        case ExprType::MEMBER:
            return analyzeMemberExpr(static_cast<const MemberExpr*>(expr));
        case ExprType::THIS:
            return analyzeThisExpr(static_cast<const ThisExpr*>(expr));
        case ExprType::SUPER:
            return analyzeSuperExpr(static_cast<const SuperExpr*>(expr));
        case ExprType::GROUPING:
            return analyzeGroupingExpr(static_cast<const GroupingExpr*>(expr));
        default:
            return "any";
    }
}

/**
 * @brief 分析二元表达式
 * 
 * 分析二元表达式的语义，包括类型兼容性检查和类型推断。
 * 
 * @param expr 二元表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeBinaryExpr(const BinaryExpr* expr) {
    std::string leftType = analyzeExpression(expr->left.get());
    std::string rightType = analyzeExpression(expr->right.get());
    
    // 检查类型兼容性
    if (leftType != rightType && leftType != "any" && rightType != "any") {
        error("Type mismatch in binary expression: expected '" + leftType + "', got '" + rightType + "'.");
    }
    
    // 简单的类型推断
    if (expr->op == "+") {
        if (leftType == "string" || rightType == "string") {
            return "string";
        }
        if (leftType == "number" || rightType == "number") {
            return "number";
        }
    } else if (expr->op == "-" || expr->op == "*" || expr->op == "/" || expr->op == "%") {
        if (leftType == "number" || rightType == "number") {
            return "number";
        }
    } else if (expr->op == "==" || expr->op == "!=" || expr->op == "<" || expr->op == "<=" || expr->op == ">" || expr->op == ">") {
        return "bool";
    } else if (expr->op == "&&" || expr->op == "||") {
        if (leftType != "bool" && leftType != "any") {
            error("Logical operator '" + expr->op + "' expects boolean operands, got '" + leftType + "'.");
        }
        if (rightType != "bool" && rightType != "any") {
            error("Logical operator '" + expr->op + "' expects boolean operands, got '" + rightType + "'.");
        }
        return "bool";
    }
    
    return leftType;
}

/**
 * @brief 分析一元表达式
 * 
 * 分析一元表达式的语义，包括类型检查和类型推断。
 * 
 * @param expr 一元表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeUnaryExpr(const UnaryExpr* expr) {
    std::string rightType = analyzeExpression(expr->right.get());
    
    if (expr->op == "!" || expr->op == "!") {
        if (rightType != "bool" && rightType != "any") {
            error("Logical operator '" + expr->op + "' expects boolean operand, got '" + rightType + "'.");
        }
        return "bool";
    } else if (expr->op == "-") {
        if (rightType != "number" && rightType != "any") {
            error("Unary operator '-' expects number operand, got '" + rightType + "'.");
        }
        return "number";
    }
    
    return rightType;
}

/**
 * @brief 分析字面量表达式
 * 
 * 分析字面量表达式的语义，返回其类型。
 * 
 * @param expr 字面量表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeLiteralExpr(const LiteralExpr* expr) {
    return expr->type;
}

/**
 * @brief 分析标识符表达式
 * 
 * 分析标识符表达式的语义，包括变量或函数存在性检查和类型获取。
 * 
 * @param expr 标识符表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeIdentifierExpr(const IdentifierExpr* expr) {
    if (!isVariableDefined(expr->name) && !isFunctionDefined(expr->name)) {
        error("Undefined identifier '" + expr->name + "'.");
        return "any";
    }
    
    if (isVariableDefined(expr->name)) {
        // 从符号表中获取变量类型
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto varIt = it->variables.find(expr->name);
            if (varIt != it->variables.end()) {
                return varIt->second;
            }
        }
    }
    
    if (isFunctionDefined(expr->name)) {
        // 从符号表中获取函数返回类型
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto funcIt = it->functions.find(expr->name);
            if (funcIt != it->functions.end()) {
                return funcIt->second;
            }
        }
    }
    
    return "any";
}

/**
 * @brief 分析赋值表达式
 * 
 * 分析赋值表达式的语义，包括变量存在性检查和类型兼容性检查。
 * 
 * @param expr 赋值表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeAssignExpr(const AssignExpr* expr) {
    std::string valueType = analyzeExpression(expr->value.get());
    
    if (!isVariableDefined(expr->name)) {
        error("Undefined variable '" + expr->name + "'.");
        return "any";
    }
    
    // 检查类型兼容性
    std::string varType = "any";
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto varIt = it->variables.find(expr->name);
        if (varIt != it->variables.end()) {
            varType = varIt->second;
            break;
        }
    }
    
    if (varType != "any" && valueType != varType) {
        error("Type mismatch in assignment: expected '" + varType + "', got '" + valueType + "'.");
    }
    
    return valueType;
}

/**
 * @brief 分析函数调用表达式
 * 
 * 分析函数调用表达式的语义，包括函数存在性检查和参数分析。
 * 
 * @param expr 函数调用表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeCallExpr(const CallExpr* expr) {
    // 分析调用表达式
    std::string calleeType = analyzeExpression(expr->callee.get());
    
    // 检查函数是否存在
    if (auto* identifierExpr = dynamic_cast<const IdentifierExpr*>(expr->callee.get())) {
        std::string funcName = identifierExpr->name;
        if (!isFunctionDefined(funcName) && funcName != "println" && funcName != "print" && funcName != "error") {
            // 允许调用内置函数
            error("Undefined function '" + funcName + "'.");
        }
    }
    
    // 分析参数
    for (const auto& arg : expr->arguments) {
        analyzeExpression(arg.get());
    }
    
    // 暂时返回 any 类型，后续可以根据函数定义返回具体类型
    return "any";
}

/**
 * @brief 分析成员访问表达式
 * 
 * 分析成员访问表达式的语义，包括对象表达式分析。
 * 
 * @param expr 成员访问表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeMemberExpr(const MemberExpr* expr) {
    // 分析对象表达式
    analyzeExpression(expr->object.get());
    
    // 暂时返回 any 类型
    return "any";
}

/**
 * @brief 分析this表达式
 * 
 * 分析this表达式的语义。
 * 
 * @param expr this表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeThisExpr(const ThisExpr* expr) {
    // 暂时返回 any 类型
    return "any";
}

/**
 * @brief 分析super表达式
 * 
 * 分析super表达式的语义。
 * 
 * @param expr super表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeSuperExpr(const SuperExpr* expr) {
    // 暂时返回 any 类型
    return "any";
}

/**
 * @brief 分析分组表达式
 * 
 * 分析分组表达式的语义，返回内部表达式的类型。
 * 
 * @param expr 分组表达式指针
 * @return 表达式的类型
 */
std::string SemanticAnalyzer::analyzeGroupingExpr(const GroupingExpr* expr) {
    return analyzeExpression(expr->expression.get());
}

} // namespace frontend
} // namespace nexus
