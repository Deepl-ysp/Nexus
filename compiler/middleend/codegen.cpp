/**
 * @file codegen.cpp
 * @brief 代码生成器实现，将 AST 转换为 IR
 * @author Nexus 编译器团队
 * @version 1.0
 */

#include "./codegen.h"

namespace nexus {
namespace middleend {

/**
 * @brief CodeGenerator 构造函数
 * 
 * 初始化代码生成器的成员变量
 */
CodeGenerator::CodeGenerator()
    : module(nullptr), currentFunction(nullptr), currentBlock(nullptr),
      instrCount(0), blockCount(0) {
}

/**
 * @brief 生成 IR 模块
 * 
 * @param statements AST 语句列表
 * @return std::unique_ptr<Module> 生成的 IR 模块
 */
std::unique_ptr<Module> CodeGenerator::generate(const std::vector<std::unique_ptr<frontend::Stmt>>& statements) {
    // 创建模块
    module = std::make_unique<Module>("main");
    
    // 创建主函数
    currentFunction = std::make_unique<Function>("main", Type::INT32);
    
    // 创建入口基本块
    auto entryBlock = std::make_unique<BasicBlock>(generateBlockName());
    currentBlock = entryBlock.get();
    currentFunction->addBasicBlock(std::move(entryBlock));
    
    // 访问所有语句
    for (const auto& stmt : statements) {
        visitStatement(stmt.get());
    }
    
    // 添加返回语句
    createRet(Type::INT32, createConstant(Type::INT32, "0"));
    
    // 将主函数添加到模块
    module->addFunction(std::move(currentFunction));
    
    return std::move(module);
}

/**
 * @brief 获取生成的 IR 模块
 * 
 * @return const std::unique_ptr<Module>& 生成的 IR 模块
 */
const std::unique_ptr<Module>& CodeGenerator::getModule() const {
    return module;
}

/**
 * @brief 生成指令名称
 * 
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::generateInstrName() {
    return "%instr" + std::to_string(instrCount++);
}

/**
 * @brief 生成基本块名称
 * 
 * @return std::string 生成的基本块名称
 */
std::string CodeGenerator::generateBlockName() {
    return "block" + std::to_string(blockCount++);
}

/**
 * @brief 访问表达式并生成对应的 IR
 * 
 * @param expr 表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitExpression(const frontend::Expr* expr) {
    switch (expr->getType()) {
        case frontend::ExprType::BINARY:
            return visitBinaryExpr(static_cast<const frontend::BinaryExpr*>(expr));
        case frontend::ExprType::UNARY:
            return visitUnaryExpr(static_cast<const frontend::UnaryExpr*>(expr));
        case frontend::ExprType::LITERAL:
            return visitLiteralExpr(static_cast<const frontend::LiteralExpr*>(expr));
        case frontend::ExprType::IDENTIFIER:
            return visitIdentifierExpr(static_cast<const frontend::IdentifierExpr*>(expr));
        case frontend::ExprType::ASSIGN:
            return visitAssignExpr(static_cast<const frontend::AssignExpr*>(expr));
        case frontend::ExprType::CALL:
            return visitCallExpr(static_cast<const frontend::CallExpr*>(expr));
        case frontend::ExprType::MEMBER:
            return visitMemberExpr(static_cast<const frontend::MemberExpr*>(expr));
        case frontend::ExprType::GROUPING:
            return visitGroupingExpr(static_cast<const frontend::GroupingExpr*>(expr));
        default:
            return createConstant(Type::INT32, "0");
    }
}

/**
 * @brief 访问语句并生成对应的 IR
 * 
 * @param stmt 语句指针
 */
void CodeGenerator::visitStatement(const frontend::Stmt* stmt) {
    switch (stmt->getType()) {
        case frontend::StmtType::EXPRESSION:
            visitExpressionStmt(static_cast<const frontend::ExpressionStmt*>(stmt));
            break;
        case frontend::StmtType::PRINT:
            visitPrintStmt(static_cast<const frontend::PrintStmt*>(stmt));
            break;
        case frontend::StmtType::VAR:
            visitVarStmt(static_cast<const frontend::VarStmt*>(stmt));
            break;
        case frontend::StmtType::CONST:
            visitConstStmt(static_cast<const frontend::ConstStmt*>(stmt));
            break;
        case frontend::StmtType::FN:
            visitFunctionStmt(static_cast<const frontend::FunctionStmt*>(stmt));
            break;
        case frontend::StmtType::RETURN:
            visitReturnStmt(static_cast<const frontend::ReturnStmt*>(stmt));
            break;
        case frontend::StmtType::BLOCK:
            visitBlockStmt(static_cast<const frontend::BlockStmt*>(stmt));
            break;
        case frontend::StmtType::IF:
            visitIfStmt(static_cast<const frontend::IfStmt*>(stmt));
            break;
        case frontend::StmtType::WHILE:
            visitWhileStmt(static_cast<const frontend::WhileStmt*>(stmt));
            break;
        case frontend::StmtType::FOR:
            visitForStmt(static_cast<const frontend::ForStmt*>(stmt));
            break;
        case frontend::StmtType::STRUCT:
            visitStructStmt(static_cast<const frontend::StructStmt*>(stmt));
            break;
        case frontend::StmtType::CLASS:
            visitClassStmt(static_cast<const frontend::ClassStmt*>(stmt));
            break;
        default:
            break;
    }
}

/**
 * @brief 访问二元表达式并生成对应的 IR
 * 
 * @param expr 二元表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitBinaryExpr(const frontend::BinaryExpr* expr) {
    std::string left = visitExpression(expr->left.get());
    std::string right = visitExpression(expr->right.get());
    
    OpCode opcode;
    if (expr->op == "+") {
        opcode = OpCode::ADD;
    } else if (expr->op == "-") {
        opcode = OpCode::SUB;
    } else if (expr->op == "*") {
        opcode = OpCode::MUL;
    } else if (expr->op == "/") {
        opcode = OpCode::DIV;
    } else if (expr->op == "%") {
        opcode = OpCode::MOD;
    } else if (expr->op == "==") {
        opcode = OpCode::EQ;
    } else if (expr->op == "!=") {
        opcode = OpCode::NE;
    } else if (expr->op == "<") {
        opcode = OpCode::LT;
    } else if (expr->op == "<=") {
        opcode = OpCode::LE;
    } else if (expr->op == ">") {
        opcode = OpCode::GT;
    } else if (expr->op == ">=") {
        opcode = OpCode::GE;
    } else if (expr->op == "&&") {
        opcode = OpCode::AND;
    } else if (expr->op == "||") {
        opcode = OpCode::OR;
    } else {
        opcode = OpCode::ADD;
    }
    
    return createBinaryOp(opcode, Type::INT32, left, right);
}

/**
 * @brief 访问一元表达式并生成对应的 IR
 * 
 * @param expr 一元表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitUnaryExpr(const frontend::UnaryExpr* expr) {
    std::string operand = visitExpression(expr->right.get());
    
    OpCode opcode;
    if (expr->op == "!") {
        opcode = OpCode::NOT;
    } else if (expr->op == "-") {
        opcode = OpCode::SUB;
    } else {
        opcode = OpCode::NOT;
    }
    
    return createUnaryOp(opcode, Type::INT32, operand);
}

/**
 * @brief 访问字面量表达式并生成对应的 IR
 * 
 * @param expr 字面量表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitLiteralExpr(const frontend::LiteralExpr* expr) {
    Type type;
    if (expr->type == "number") {
        type = Type::INT32;
    } else if (expr->type == "string") {
        type = Type::POINTER;
    } else if (expr->type == "bool") {
        type = Type::BOOL;
    } else {
        type = Type::INT32;
    }
    
    return createConstant(type, expr->value);
}

/**
 * @brief 访问标识符表达式并生成对应的 IR
 * 
 * @param expr 标识符表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitIdentifierExpr(const frontend::IdentifierExpr* expr) {
    // 查找变量映射
    auto it = variableMap.find(expr->name);
    if (it != variableMap.end()) {
        // 加载变量值
        return createLoad(Type::INT32, it->second);
    }
    
    // 如果变量不存在，返回常量 0
    return createConstant(Type::INT32, "0");
}

/**
 * @brief 访问赋值表达式并生成对应的 IR
 * 
 * @param expr 赋值表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitAssignExpr(const frontend::AssignExpr* expr) {
    // 生成右值
    std::string value = visitExpression(expr->value.get());
    
    // 查找变量映射
    auto it = variableMap.find(expr->name);
    if (it != variableMap.end()) {
        // 存储值到变量
        createStore(Type::INT32, value, it->second);
    } else {
        // 创建新变量
        std::string alloca = createAlloca(Type::INT32);
        createStore(Type::INT32, value, alloca);
        variableMap[expr->name] = alloca;
    }
    
    return value;
}

/**
 * @brief 访问函数调用表达式并生成对应的 IR
 * 
 * @param expr 函数调用表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitCallExpr(const frontend::CallExpr* expr) {
    // 生成函数名
    std::string funcName = "printf";
    
    // 生成参数
    std::vector<std::string> arguments;
    
    // 处理函数调用
    if (auto* ident = dynamic_cast<const frontend::IdentifierExpr*>(expr->callee.get())) {
        if (ident->name == "println") {
            // 检查参数类型
            if (!expr->arguments.empty()) {
                const auto& arg = expr->arguments[0];
                // 对于字符串类型，使用 %s 格式
                if (auto* literalExpr = dynamic_cast<const frontend::LiteralExpr*>(arg.get())) {
                    if (literalExpr->type == "string") {
                        arguments.push_back(createConstant(Type::POINTER, "%s\n"));
                        arguments.push_back(visitExpression(arg.get()));
                    } else {
                        // 对于其他类型，使用 %d 格式
                        arguments.push_back(createConstant(Type::POINTER, "%d\n"));
                        arguments.push_back(visitExpression(arg.get()));
                    }
                } else {
                    // 对于表达式，使用 %d 格式
                    arguments.push_back(createConstant(Type::POINTER, "%d\n"));
                    arguments.push_back(visitExpression(arg.get()));
                }
            } else {
                // 无参数的 println，输出空行
                arguments.push_back(createConstant(Type::POINTER, "\n"));
            }
        } else {
            // 其他函数调用，暂时使用 printf 作为默认实现
            funcName = ident->name;
            // 生成参数
            for (const auto& arg : expr->arguments) {
                arguments.push_back(visitExpression(arg.get()));
            }
        }
    }
    
    return createCall(Type::INT32, funcName, arguments);
}

/**
 * @brief 访问成员表达式并生成对应的 IR
 * 
 * @param expr 成员表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitMemberExpr(const frontend::MemberExpr* expr) {
    // 生成对象表达式
    std::string object = visitExpression(expr->object.get());
    
    // 对于成员访问，暂时返回对象本身
    // 后续需要实现更复杂的成员访问逻辑
    return object;
}

/**
 * @brief 访问分组表达式并生成对应的 IR
 * 
 * @param expr 分组表达式指针
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::visitGroupingExpr(const frontend::GroupingExpr* expr) {
    return visitExpression(expr->expression.get());
}

/**
 * @brief 访问变量声明语句并生成对应的 IR
 * 
 * @param stmt 变量声明语句指针
 */
void CodeGenerator::visitVarStmt(const frontend::VarStmt* stmt) {
    // 生成初始值
    std::string initializer;
    if (stmt->initializer) {
        initializer = visitExpression(stmt->initializer.get());
    } else {
        initializer = createConstant(Type::INT32, "0");
    }
    
    // 创建栈分配
    std::string alloca = createAlloca(Type::INT32);
    
    // 存储初始值
    createStore(Type::INT32, initializer, alloca);
    
    // 添加到变量映射
    variableMap[stmt->name] = alloca;
}

/**
 * @brief 访问常量声明语句并生成对应的 IR
 * 
 * @param stmt 常量声明语句指针
 */
void CodeGenerator::visitConstStmt(const frontend::ConstStmt* stmt) {
    // 生成初始值
    std::string initializer;
    if (stmt->initializer) {
        initializer = visitExpression(stmt->initializer.get());
    } else {
        initializer = createConstant(Type::INT32, "0");
    }
    
    // 创建栈分配
    std::string alloca = createAlloca(Type::INT32);
    
    // 存储初始值
    createStore(Type::INT32, initializer, alloca);
    
    // 添加到变量映射
    variableMap[stmt->name] = alloca;
}

/**
 * @brief 访问函数声明语句并生成对应的 IR
 * 
 * @param stmt 函数声明语句指针
 */
void CodeGenerator::visitFunctionStmt(const frontend::FunctionStmt* stmt) {
    // 创建函数
    auto func = std::make_unique<Function>(stmt->name, Type::INT32);
    
    // 创建入口基本块
    auto entryBlock = std::make_unique<BasicBlock>(generateBlockName());
    BasicBlock* funcEntryBlock = entryBlock.get();
    func->addBasicBlock(std::move(entryBlock));
    
    // 保存当前状态
    auto oldFunction = std::move(currentFunction);
    BasicBlock* oldBlock = currentBlock;
    auto oldVariableMap = variableMap;
    
    // 设置当前函数和基本块
    currentFunction = std::move(func);
    currentBlock = funcEntryBlock;
    variableMap.clear();
    
    // 访问函数体
    visitStatement(stmt->body.get());
    
    // 添加返回语句
    createRet(Type::INT32, createConstant(Type::INT32, "0"));
    
    // 将函数添加到模块
    module->addFunction(std::move(currentFunction));
    
    // 恢复状态
    currentFunction = std::move(oldFunction);
    currentBlock = oldBlock;
    variableMap = oldVariableMap;
}

/**
 * @brief 访问表达式语句并生成对应的 IR
 * 
 * @param stmt 表达式语句指针
 */
void CodeGenerator::visitExpressionStmt(const frontend::ExpressionStmt* stmt) {
    visitExpression(stmt->expression.get());
}

/**
 * @brief 访问打印语句并生成对应的 IR
 * 
 * @param stmt 打印语句指针
 */
void CodeGenerator::visitPrintStmt(const frontend::PrintStmt* stmt) {
    // 生成表达式
    std::string expr = visitExpression(stmt->expression.get());
    
    // 创建打印调用
    std::vector<std::string> args;
    args.push_back(createConstant(Type::POINTER, "%d\n"));
    args.push_back(expr);
    createCall(Type::INT32, "printf", args);
}

/**
 * @brief 访问返回语句并生成对应的 IR
 * 
 * @param stmt 返回语句指针
 */
void CodeGenerator::visitReturnStmt(const frontend::ReturnStmt* stmt) {
    if (stmt->value) {
        std::string value = visitExpression(stmt->value.get());
        createRet(Type::INT32, value);
    } else {
        createRet(Type::INT32, createConstant(Type::INT32, "0"));
    }
}

/**
 * @brief 访问块语句并生成对应的 IR
 * 
 * @param stmt 块语句指针
 */
void CodeGenerator::visitBlockStmt(const frontend::BlockStmt* stmt) {
    // 访问所有语句
    for (const auto& stmt : stmt->statements) {
        visitStatement(stmt.get());
    }
}

/**
 * @brief 访问条件语句并生成对应的 IR
 * 
 * @param stmt 条件语句指针
 */
void CodeGenerator::visitIfStmt(const frontend::IfStmt* stmt) {
    // 生成条件表达式
    std::string condition = visitExpression(stmt->condition.get());
    
    // 创建基本块
    auto thenBlock = std::make_unique<BasicBlock>(generateBlockName());
    auto elseBlock = std::make_unique<BasicBlock>(generateBlockName());
    auto mergeBlock = std::make_unique<BasicBlock>(generateBlockName());
    
    BasicBlock* thenBlockPtr = thenBlock.get();
    BasicBlock* elseBlockPtr = elseBlock.get();
    BasicBlock* mergeBlockPtr = mergeBlock.get();
    
    // 添加基本块到函数
    currentFunction->addBasicBlock(std::move(thenBlock));
    currentFunction->addBasicBlock(std::move(elseBlock));
    currentFunction->addBasicBlock(std::move(mergeBlock));
    
    // 创建条件分支
    createCondBr(condition, thenBlockPtr->name, elseBlockPtr->name);
    
    // 访问 then 分支
    currentBlock = thenBlockPtr;
    visitStatement(stmt->thenBranch.get());
    createBr(mergeBlockPtr->name);
    
    // 访问 else 分支
    currentBlock = elseBlockPtr;
    if (stmt->elseBranch) {
        visitStatement(stmt->elseBranch.get());
    }
    createBr(mergeBlockPtr->name);
    
    // 设置当前块为合并块
    currentBlock = mergeBlockPtr;
}

/**
 * @brief 访问 while 循环语句并生成对应的 IR
 * 
 * @param stmt while 循环语句指针
 */
void CodeGenerator::visitWhileStmt(const frontend::WhileStmt* stmt) {
    // 创建基本块
    auto conditionBlock = std::make_unique<BasicBlock>(generateBlockName());
    auto bodyBlock = std::make_unique<BasicBlock>(generateBlockName());
    auto mergeBlock = std::make_unique<BasicBlock>(generateBlockName());
    
    BasicBlock* conditionBlockPtr = conditionBlock.get();
    BasicBlock* bodyBlockPtr = bodyBlock.get();
    BasicBlock* mergeBlockPtr = mergeBlock.get();
    
    // 添加基本块到函数
    currentFunction->addBasicBlock(std::move(conditionBlock));
    currentFunction->addBasicBlock(std::move(bodyBlock));
    currentFunction->addBasicBlock(std::move(mergeBlock));
    
    // 跳转到条件块
    createBr(conditionBlockPtr->name);
    
    // 访问条件块
    currentBlock = conditionBlockPtr;
    std::string condition = visitExpression(stmt->condition.get());
    createCondBr(condition, bodyBlockPtr->name, mergeBlockPtr->name);
    
    // 访问 body 块
    currentBlock = bodyBlockPtr;
    visitStatement(stmt->body.get());
    createBr(conditionBlockPtr->name);
    
    // 设置当前块为合并块
    currentBlock = mergeBlockPtr;
}

/**
 * @brief 访问 for 循环语句并生成对应的 IR
 * 
 * @param stmt for 循环语句指针
 */
void CodeGenerator::visitForStmt(const frontend::ForStmt* stmt) {
    // 访问初始化语句
    if (stmt->initializer) {
        visitStatement(stmt->initializer.get());
    }
    
    // 创建基本块
    auto conditionBlock = std::make_unique<BasicBlock>(generateBlockName());
    auto bodyBlock = std::make_unique<BasicBlock>(generateBlockName());
    auto incrementBlock = std::make_unique<BasicBlock>(generateBlockName());
    auto mergeBlock = std::make_unique<BasicBlock>(generateBlockName());
    
    BasicBlock* conditionBlockPtr = conditionBlock.get();
    BasicBlock* bodyBlockPtr = bodyBlock.get();
    BasicBlock* incrementBlockPtr = incrementBlock.get();
    BasicBlock* mergeBlockPtr = mergeBlock.get();
    
    // 添加基本块到函数
    currentFunction->addBasicBlock(std::move(conditionBlock));
    currentFunction->addBasicBlock(std::move(bodyBlock));
    currentFunction->addBasicBlock(std::move(incrementBlock));
    currentFunction->addBasicBlock(std::move(mergeBlock));
    
    // 跳转到条件块
    createBr(conditionBlockPtr->name);
    
    // 访问条件块
    currentBlock = conditionBlockPtr;
    std::string condition = visitExpression(stmt->condition.get());
    createCondBr(condition, bodyBlockPtr->name, mergeBlockPtr->name);
    
    // 访问 body 块
    currentBlock = bodyBlockPtr;
    visitStatement(stmt->body.get());
    createBr(incrementBlockPtr->name);
    
    // 访问 increment 块
    currentBlock = incrementBlockPtr;
    visitExpression(stmt->increment.get());
    createBr(conditionBlockPtr->name);
    
    // 设置当前块为合并块
    currentBlock = mergeBlockPtr;
}

/**
 * @brief 访问结构体声明语句并生成对应的 IR
 * 
 * @param stmt 结构体声明语句指针
 */
void CodeGenerator::visitStructStmt(const frontend::StructStmt* stmt) {
    // 结构体声明暂时不生成代码
    // 后续需要实现结构体的代码生成
}

/**
 * @brief 访问类声明语句并生成对应的 IR
 * 
 * @param stmt 类声明语句指针
 */
void CodeGenerator::visitClassStmt(const frontend::ClassStmt* stmt) {
    // 类声明暂时不生成代码
    // 后续需要实现类的代码生成
}

/**
 * @brief 创建常量指令
 * 
 * @param type 常量类型
 * @param value 常量值
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::createConstant(Type type, const std::string& value) {
    auto instr = std::make_unique<ConstInst>(type, value);
    instr->name = generateInstrName();
    currentBlock->addInstruction(std::move(instr));
    return instr->name;
}

/**
 * @brief 创建二元操作指令
 * 
 * @param opcode 操作码
 * @param type 操作类型
 * @param left 左操作数
 * @param right 右操作数
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::createBinaryOp(OpCode opcode, Type type, const std::string& left, const std::string& right) {
    auto instr = std::make_unique<BinaryInst>(opcode, type, left, right);
    instr->name = generateInstrName();
    currentBlock->addInstruction(std::move(instr));
    return instr->name;
}

/**
 * @brief 创建一元操作指令
 * 
 * @param opcode 操作码
 * @param type 操作类型
 * @param operand 操作数
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::createUnaryOp(OpCode opcode, Type type, const std::string& operand) {
    auto instr = std::make_unique<UnaryInst>(opcode, type, operand);
    instr->name = generateInstrName();
    currentBlock->addInstruction(std::move(instr));
    return instr->name;
}

/**
 * @brief 创建函数调用指令
 * 
 * @param returnType 返回类型
 * @param funcName 函数名称
 * @param arguments 参数列表
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::createCall(Type returnType, const std::string& funcName, const std::vector<std::string>& arguments) {
    auto instr = std::make_unique<CallInst>(returnType, funcName, arguments);
    instr->name = generateInstrName();
    currentBlock->addInstruction(std::move(instr));
    return instr->name;
}

/**
 * @brief 创建栈分配指令
 * 
 * @param type 分配类型
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::createAlloca(Type type) {
    auto instr = std::make_unique<AllocaInst>(type);
    instr->name = generateInstrName();
    currentBlock->addInstruction(std::move(instr));
    return instr->name;
}

/**
 * @brief 创建加载指令
 * 
 * @param type 加载类型
 * @param pointer 指针地址
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::createLoad(Type type, const std::string& pointer) {
    auto instr = std::make_unique<LoadInst>(type, pointer);
    instr->name = generateInstrName();
    currentBlock->addInstruction(std::move(instr));
    return instr->name;
}

/**
 * @brief 创建存储指令
 * 
 * @param type 存储类型
 * @param value 存储值
 * @param pointer 指针地址
 */
void CodeGenerator::createStore(Type type, const std::string& value, const std::string& pointer) {
    auto instr = std::make_unique<StoreInst>(type, value, pointer);
    currentBlock->addInstruction(std::move(instr));
}

/**
 * @brief 创建条件分支指令
 * 
 * @param condition 条件表达式
 * @param trueBlock 条件为真时的目标块
 * @param falseBlock 条件为假时的目标块
 */
void CodeGenerator::createCondBr(const std::string& condition, const std::string& trueBlock, const std::string& falseBlock) {
    auto instr = std::make_unique<CondBrInst>(condition, trueBlock, falseBlock);
    currentBlock->addInstruction(std::move(instr));
}

/**
 * @brief 创建无条件分支指令
 * 
 * @param targetBlock 目标块
 */
void CodeGenerator::createBr(const std::string& targetBlock) {
    auto instr = std::make_unique<BrInst>(targetBlock);
    currentBlock->addInstruction(std::move(instr));
}

/**
 * @brief 创建返回指令
 * 
 * @param returnType 返回类型
 * @param value 返回值
 */
void CodeGenerator::createRet(Type returnType, const std::string& value) {
    auto instr = std::make_unique<RetInst>(returnType, value);
    currentBlock->addInstruction(std::move(instr));
}

/**
 * @brief 从 AST 类型获取 IR 类型
 * 
 * @param astType AST 类型字符串
 * @return Type 对应的 IR 类型
 */
Type CodeGenerator::getTypeFromASTType(const std::string& astType) {
    if (astType == "int" || astType == "number") {
        return Type::INT32;
    } else if (astType == "float" || astType == "double") {
        return Type::FLOAT;
    } else if (astType == "bool") {
        return Type::BOOL;
    } else if (astType == "string") {
        return Type::POINTER;
    } else {
        return Type::INT32;
    }
}

} // namespace middleend
} // namespace nexus
