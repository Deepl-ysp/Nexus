#include "./ast.h"

namespace nexus {
namespace frontend {

/**
 * @brief 二元表达式类的构造函数
 * 
 * 创建一个二元表达式，包含左操作数、操作符和右操作数。
 * 
 * @param left 左操作数表达式
 * @param op 操作符字符串
 * @param right 右操作数表达式
 */
BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, const std::string& op, std::unique_ptr<Expr> right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::BINARY
 */
ExprType BinaryExpr::getType() const {
    return ExprType::BINARY;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string BinaryExpr::toString() const {
    return "(" + op + " " + left->toString() + " " + right->toString() + ")";
}

/**
 * @brief 一元表达式类的构造函数
 * 
 * 创建一个一元表达式，包含操作符和操作数。
 * 
 * @param op 操作符字符串
 * @param right 操作数表达式
 */
UnaryExpr::UnaryExpr(const std::string& op, std::unique_ptr<Expr> right)
    : op(op), right(std::move(right)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::UNARY
 */
ExprType UnaryExpr::getType() const {
    return ExprType::UNARY;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string UnaryExpr::toString() const {
    return "(" + op + " " + right->toString() + ")";
}

/**
 * @brief 字面量表达式类的构造函数
 * 
 * 创建一个字面量表达式，包含值和类型。
 * 
 * @param value 字面量值
 * @param type 字面量类型
 */
LiteralExpr::LiteralExpr(const std::string& value, const std::string& type)
    : value(value), type(type) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::LITERAL
 */
ExprType LiteralExpr::getType() const {
    return ExprType::LITERAL;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string LiteralExpr::toString() const {
    return value;
}

/**
 * @brief 标识符表达式类的构造函数
 * 
 * 创建一个标识符表达式，包含标识符名称。
 * 
 * @param name 标识符名称
 */
IdentifierExpr::IdentifierExpr(const std::string& name)
    : name(name) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::IDENTIFIER
 */
ExprType IdentifierExpr::getType() const {
    return ExprType::IDENTIFIER;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string IdentifierExpr::toString() const {
    return name;
}

/**
 * @brief 赋值表达式类的构造函数
 * 
 * 创建一个赋值表达式，包含变量名和赋值表达式。
 * 
 * @param name 变量名
 * @param value 赋值表达式
 */
AssignExpr::AssignExpr(const std::string& name, std::unique_ptr<Expr> value)
    : name(name), value(std::move(value)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::ASSIGN
 */
ExprType AssignExpr::getType() const {
    return ExprType::ASSIGN;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string AssignExpr::toString() const {
    return "(= " + name + " " + value->toString() + ")";
}

/**
 * @brief 函数调用表达式类的构造函数
 * 
 * 创建一个函数调用表达式，包含被调用的表达式和参数列表。
 * 
 * @param callee 被调用的表达式
 * @param arguments 参数列表
 */
CallExpr::CallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> arguments)
    : callee(std::move(callee)), arguments(std::move(arguments)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::CALL
 */
ExprType CallExpr::getType() const {
    return ExprType::CALL;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string CallExpr::toString() const {
    std::string result = "(call " + callee->toString();
    for (const auto& arg : arguments) {
        result += " " + arg->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief 成员访问表达式类的构造函数
 * 
 * 创建一个成员访问表达式，包含对象表达式和成员名称。
 * 
 * @param object 对象表达式
 * @param name 成员名称
 */
MemberExpr::MemberExpr(std::unique_ptr<Expr> object, const std::string& name)
    : object(std::move(object)), name(name) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::MEMBER
 */
ExprType MemberExpr::getType() const {
    return ExprType::MEMBER;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string MemberExpr::toString() const {
    return "(. " + object->toString() + " " + name + ")";
}

/**
 * @brief this 表达式类的构造函数
 * 
 * 创建一个 this 表达式。
 */
ThisExpr::ThisExpr() {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::THIS
 */
ExprType ThisExpr::getType() const {
    return ExprType::THIS;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string ThisExpr::toString() const {
    return "this";
}

/**
 * @brief super 表达式类的构造函数
 * 
 * 创建一个 super 表达式，包含方法名称。
 * 
 * @param method 方法名称
 */
SuperExpr::SuperExpr(const std::string& method)
    : method(method) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::SUPER
 */
ExprType SuperExpr::getType() const {
    return ExprType::SUPER;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string SuperExpr::toString() const {
    return "(super " + method + ")";
}

/**
 * @brief 分组表达式类的构造函数
 * 
 * 创建一个分组表达式，包含被分组的表达式。
 * 
 * @param expression 被分组的表达式
 */
GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::GROUPING
 */
ExprType GroupingExpr::getType() const {
    return ExprType::GROUPING;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string GroupingExpr::toString() const {
    return "(group " + expression->toString() + ")";
}

/**
 * @brief 数组表达式类的构造函数
 * 
 * 创建一个数组表达式，包含元素列表。
 * 
 * @param elements 元素列表
 */
ArrayExpr::ArrayExpr(std::vector<std::unique_ptr<Expr>> elements)
    : elements(std::move(elements)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::ARRAY
 */
ExprType ArrayExpr::getType() const {
    return ExprType::ARRAY;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string ArrayExpr::toString() const {
    std::string result = "(array";
    for (const auto& element : elements) {
        result += " " + element->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief 对象表达式类的构造函数
 * 
 * 创建一个对象表达式，包含属性列表。
 * 
 * @param properties 属性列表，每个属性是名称和表达式的对
 */
ObjectExpr::ObjectExpr(std::vector<std::pair<std::string, std::unique_ptr<Expr>>> properties)
    : properties(std::move(properties)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::OBJECT
 */
ExprType ObjectExpr::getType() const {
    return ExprType::OBJECT;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string ObjectExpr::toString() const {
    std::string result = "(object";
    for (const auto& property : properties) {
        result += " (" + property.first + " " + property.second->toString() + ")";
    }
    result += ")";
    return result;
}

/**
 * @brief 索引访问表达式类的构造函数
 * 
 * 创建一个索引访问表达式，包含对象表达式和索引表达式。
 * 
 * @param object 对象表达式
 * @param index 索引表达式
 */
IndexExpr::IndexExpr(std::unique_ptr<Expr> object, std::unique_ptr<Expr> index)
    : object(std::move(object)), index(std::move(index)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::INDEX
 */
ExprType IndexExpr::getType() const {
    return ExprType::INDEX;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string IndexExpr::toString() const {
    return "(index " + object->toString() + " " + index->toString() + ")";
}

/**
 * @brief Lambda 表达式类的构造函数
 * 
 * 创建一个 Lambda 表达式，包含参数列表和函数体。
 * 
 * @param parameters 参数列表，每个参数是名称和类型的对
 * @param body 函数体表达式
 */
LambdaExpr::LambdaExpr(std::vector<std::pair<std::string, std::string>> parameters, std::unique_ptr<Expr> body)
    : parameters(std::move(parameters)), body(std::move(body)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::LAMBDA
 */
ExprType LambdaExpr::getType() const {
    return ExprType::LAMBDA;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string LambdaExpr::toString() const {
    std::string result = "(lambda (";
    for (size_t i = 0; i < parameters.size(); i++) {
        if (i > 0) {
            result += " ";
        }
        result += parameters[i].first;
        if (!parameters[i].second.empty()) {
            result += ": " + parameters[i].second;
        }
    }
    result += ") " + body->toString() + ")";
    return result;
}

/**
 * @brief Await 表达式类的构造函数
 * 
 * 创建一个 Await 表达式，包含要等待的表达式。
 * 
 * @param expression 要等待的表达式
 */
AwaitExpr::AwaitExpr(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::AWAIT
 */
ExprType AwaitExpr::getType() const {
    return ExprType::AWAIT;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string AwaitExpr::toString() const {
    return "(await " + expression->toString() + ")";
}

/**
 * @brief Yield 表达式类的构造函数
 * 
 * 创建一个 Yield 表达式，包含要产生的值的表达式。
 * 
 * @param expression 要产生的值的表达式
 */
YieldExpr::YieldExpr(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

/**
 * @brief 获取表达式类型
 * 
 * @return 表达式类型，返回 ExprType::YIELD
 */
ExprType YieldExpr::getType() const {
    return ExprType::YIELD;
}

/**
 * @brief 将表达式转换为字符串表示
 * 
 * @return 表达式的字符串表示
 */
std::string YieldExpr::toString() const {
    std::string result = "(yield";
    if (expression) {
        result += " " + expression->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief 表达式语句类的构造函数
 * 
 * 创建一个表达式语句，包含要执行的表达式。
 * 
 * @param expression 要执行的表达式
 */
ExpressionStmt::ExpressionStmt(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::EXPRESSION
 */
StmtType ExpressionStmt::getType() const {
    return StmtType::EXPRESSION;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string ExpressionStmt::toString() const {
    return expression->toString() + ";";
}

/**
 * @brief 打印语句类的构造函数
 * 
 * 创建一个打印语句，包含要打印的表达式。
 * 
 * @param expression 要打印的表达式
 */
PrintStmt::PrintStmt(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::PRINT
 */
StmtType PrintStmt::getType() const {
    return StmtType::PRINT;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string PrintStmt::toString() const {
    return "(print " + expression->toString() + ")";
}

/**
 * @brief 变量声明语句类的构造函数
 * 
 * 创建一个变量声明语句，包含变量名、类型和初始化表达式。
 * 
 * @param name 变量名
 * @param type 变量类型
 * @param initializer 初始化表达式
 */
VarStmt::VarStmt(const std::string& name, const std::string& type, std::unique_ptr<Expr> initializer)
    : name(name), type(type), initializer(std::move(initializer)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::VAR
 */
StmtType VarStmt::getType() const {
    return StmtType::VAR;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string VarStmt::toString() const {
    std::string result = "(var " + name;
    if (!type.empty()) {
        result += ": " + type;
    }
    if (initializer) {
        result += " = " + initializer->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief 常量声明语句类的构造函数
 * 
 * 创建一个常量声明语句，包含常量名、类型和初始化表达式。
 * 
 * @param name 常量名
 * @param type 常量类型
 * @param initializer 初始化表达式
 */
ConstStmt::ConstStmt(const std::string& name, const std::string& type, std::unique_ptr<Expr> initializer)
    : name(name), type(type), initializer(std::move(initializer)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::CONST
 */
StmtType ConstStmt::getType() const {
    return StmtType::CONST;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string ConstStmt::toString() const {
    std::string result = "(const " + name;
    if (!type.empty()) {
        result += ": " + type;
    }
    if (initializer) {
        result += " = " + initializer->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief 代码块语句类的构造函数
 * 
 * 创建一个代码块语句，包含语句列表。
 * 
 * @param statements 语句列表
 */
BlockStmt::BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
    : statements(std::move(statements)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::BLOCK
 */
StmtType BlockStmt::getType() const {
    return StmtType::BLOCK;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string BlockStmt::toString() const {
    std::string result = "(block";
    for (const auto& stmt : statements) {
        result += " " + stmt->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief if 语句类的构造函数
 * 
 * 创建一个 if 语句，包含条件表达式、then 分支和 else 分支。
 * 
 * @param condition 条件表达式
 * @param thenBranch then 分支语句
 * @param elseBranch else 分支语句
 */
IfStmt::IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch)
    : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::IF
 */
StmtType IfStmt::getType() const {
    return StmtType::IF;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string IfStmt::toString() const {
    std::string result = "(if " + condition->toString() + " " + thenBranch->toString();
    if (elseBranch) {
        result += " " + elseBranch->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief while 语句类的构造函数
 * 
 * 创建一个 while 语句，包含条件表达式和循环体。
 * 
 * @param condition 条件表达式
 * @param body 循环体语句
 */
WhileStmt::WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
    : condition(std::move(condition)), body(std::move(body)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::WHILE
 */
StmtType WhileStmt::getType() const {
    return StmtType::WHILE;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string WhileStmt::toString() const {
    return "(while " + condition->toString() + " " + body->toString() + ")";
}

/**
 * @brief for 语句类的构造函数
 * 
 * 创建一个 for 语句，包含初始化语句、条件表达式、增量表达式和循环体。
 * 
 * @param initializer 初始化语句
 * @param condition 条件表达式
 * @param increment 增量表达式
 * @param body 循环体语句
 */
ForStmt::ForStmt(std::unique_ptr<Stmt> initializer, std::unique_ptr<Expr> condition, std::unique_ptr<Expr> increment, std::unique_ptr<Stmt> body)
    : initializer(std::move(initializer)), condition(std::move(condition)), increment(std::move(increment)), body(std::move(body)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::FOR
 */
StmtType ForStmt::getType() const {
    return StmtType::FOR;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string ForStmt::toString() const {
    std::string result = "(for ";
    if (initializer) {
        result += initializer->toString();
    } else {
        result += "nil";
    }
    result += " ";
    if (condition) {
        result += condition->toString();
    } else {
        result += "nil";
    }
    result += " ";
    if (increment) {
        result += increment->toString();
    } else {
        result += "nil";
    }
    result += " " + body->toString() + ")";
    return result;
}

/**
 * @brief return 语句类的构造函数
 * 
 * 创建一个 return 语句，包含返回值表达式。
 * 
 * @param value 返回值表达式
 */
ReturnStmt::ReturnStmt(std::unique_ptr<Expr> value)
    : value(std::move(value)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::RETURN
 */
StmtType ReturnStmt::getType() const {
    return StmtType::RETURN;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string ReturnStmt::toString() const {
    std::string result = "(return";
    if (value) {
        result += " " + value->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief 函数声明语句类的构造函数
 * 
 * 创建一个函数声明语句，包含函数名、参数列表、返回类型、函数体和异步标志。
 * 
 * @param name 函数名
 * @param parameters 参数列表，每个参数是名称和类型的对
 * @param returnType 返回类型
 * @param body 函数体语句
 * @param isAsync 是否为异步函数
 * @param isCoroutine 是否为协程函数
 */
FunctionStmt::FunctionStmt(const std::string& name, std::vector<std::pair<std::string, std::string>> parameters, const std::string& returnType, std::unique_ptr<Stmt> body, bool isAsync, bool isCoroutine)
    : name(name), parameters(std::move(parameters)), returnType(returnType), body(std::move(body)), isAsync(isAsync), isCoroutine(isCoroutine) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::FUNCTION
 */
StmtType FunctionStmt::getType() const {
    return StmtType::FUNCTION;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string FunctionStmt::toString() const {
    std::string result = "(fn " + name + " (";
    for (size_t i = 0; i < parameters.size(); i++) {
        if (i > 0) {
            result += " ";
        }
        result += parameters[i].first;
        if (!parameters[i].second.empty()) {
            result += ": " + parameters[i].second;
        }
    }
    result += ")";
    if (!returnType.empty()) {
        result += ": " + returnType;
    }
    result += " " + body->toString() + ")";
    return result;
}

/**
 * @brief 类声明语句类的构造函数
 * 
 * 创建一个类声明语句，包含类名、父类名和方法列表。
 * 
 * @param name 类名
 * @param superclass 父类名
 * @param methods 方法列表
 */
ClassStmt::ClassStmt(const std::string& name, const std::string& superclass, std::vector<std::unique_ptr<Stmt>> methods)
    : name(name), superclass(superclass), methods(std::move(methods)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::CLASS
 */
StmtType ClassStmt::getType() const {
    return StmtType::CLASS;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string ClassStmt::toString() const {
    std::string result = "(class " + name;
    if (!superclass.empty()) {
        result += " < " + superclass;
    }
    result += " ";
    for (const auto& method : methods) {
        result += method->toString() + " ";
    }
    result += ")";
    return result;
}

/**
 * @brief 结构体声明语句类的构造函数
 * 
 * 创建一个结构体声明语句，包含结构体名和字段列表。
 * 
 * @param name 结构体名
 * @param fields 字段列表，每个字段是名称和类型的对
 */
StructStmt::StructStmt(const std::string& name, std::vector<std::pair<std::string, std::string>> fields)
    : name(name), fields(std::move(fields)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::STRUCT
 */
StmtType StructStmt::getType() const {
    return StmtType::STRUCT;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string StructStmt::toString() const {
    std::string result = "(struct " + name + " (";
    for (size_t i = 0; i < fields.size(); i++) {
        if (i > 0) {
            result += " ";
        }
        result += fields[i].first + ": " + fields[i].second;
    }
    result += "))";
    return result;
}

/**
 * @brief 异常处理 try 语句类的构造函数
 * 
 * 创建一个 try 语句，包含 try 块、catch 块和 finally 块。
 * 
 * @param body try 块语句
 * @param catches catch 块语句列表
 * @param finally finally 块语句
 */
TryStmt::TryStmt(std::unique_ptr<Stmt> body, std::vector<std::unique_ptr<Stmt>> catches, std::unique_ptr<Stmt> finally)
    : body(std::move(body)), catches(std::move(catches)), finally(std::move(finally)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::TRY
 */
StmtType TryStmt::getType() const {
    return StmtType::TRY;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string TryStmt::toString() const {
    std::string result = "(try " + body->toString();
    for (const auto& catchStmt : catches) {
        result += " " + catchStmt->toString();
    }
    if (finally) {
        result += " " + finally->toString();
    }
    result += ")";
    return result;
}

/**
 * @brief catch 语句类的构造函数
 * 
 * 创建一个 catch 语句，包含异常变量名、异常类型和 catch 块。
 * 
 * @param name 异常变量名
 * @param type 异常类型
 * @param body catch 块语句
 */
CatchStmt::CatchStmt(const std::string& name, const std::string& type, std::unique_ptr<Stmt> body)
    : name(name), type(type), body(std::move(body)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::CATCH
 */
StmtType CatchStmt::getType() const {
    return StmtType::CATCH;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string CatchStmt::toString() const {
    std::string result = "(catch (" + name;
    if (!type.empty()) {
        result += ": " + type;
    }
    result += ") " + body->toString() + ")";
    return result;
}

/**
 * @brief throw 语句类的构造函数
 * 
 * 创建一个 throw 语句，包含要抛出的表达式。
 * 
 * @param expression 要抛出的表达式
 */
ThrowStmt::ThrowStmt(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::THROW
 */
StmtType ThrowStmt::getType() const {
    return StmtType::THROW;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string ThrowStmt::toString() const {
    return "(throw " + expression->toString() + ")";
}

/**
 * @brief 进程声明语句类的构造函数
 * 
 * 创建一个进程声明语句，包含进程 ID 和进程体。
 * 
 * @param id 进程 ID
 * @param body 进程体表达式
 */
ProcessStmt::ProcessStmt(const std::string& id, std::unique_ptr<Expr> body)
    : id(id), body(std::move(body)) {}

/**
 * @brief 获取语句类型
 * 
 * @return 语句类型，返回 StmtType::PROCESS
 */
StmtType ProcessStmt::getType() const {
    return StmtType::PROCESS;
}

/**
 * @brief 将语句转换为字符串表示
 * 
 * @return 语句的字符串表示
 */
std::string ProcessStmt::toString() const {
    return "(process " + id + " " + body->toString() + ")";
}

} // namespace frontend
} // namespace nexus
