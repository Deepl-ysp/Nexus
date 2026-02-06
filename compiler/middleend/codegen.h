#ifndef NEXUS_CODEGEN_H
#define NEXUS_CODEGEN_H

#include "ir.h"
#include "../frontend/ast.h"
#include "../frontend/semantic_analyzer.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace nexus {
namespace middleend {

class CodeGenerator {
public:
    CodeGenerator();
    
    // 生成中间代码
    std::unique_ptr<Module> generate(const std::vector<std::unique_ptr<frontend::Stmt>>& statements);
    
    // 获取生成的模块
    const std::unique_ptr<Module>& getModule() const;
    
private:
    // 模块
    std::unique_ptr<Module> module;
    
    // 当前函数
    std::unique_ptr<Function> currentFunction;
    
    // 当前基本块
    BasicBlock* currentBlock;
    
    // 指令计数器
    int instrCount;
    
    // 基本块计数器
    int blockCount;
    
    // 变量映射
    std::map<std::string, std::string> variableMap;
    
    // 生成指令名称
    std::string generateInstrName();
    
    // 生成基本块名称
    std::string generateBlockName();
    
    // 访问表达式
    std::string visitExpression(const frontend::Expr* expr);
    
    // 访问语句
    void visitStatement(const frontend::Stmt* stmt);
    
    // 访问二元表达式
    std::string visitBinaryExpr(const frontend::BinaryExpr* expr);
    
    // 访问一元表达式
    std::string visitUnaryExpr(const frontend::UnaryExpr* expr);
    
    // 访问字面量表达式
    std::string visitLiteralExpr(const frontend::LiteralExpr* expr);
    
    // 访问标识符表达式
    std::string visitIdentifierExpr(const frontend::IdentifierExpr* expr);
    
    // 访问赋值表达式
    std::string visitAssignExpr(const frontend::AssignExpr* expr);
    
    // 访问函数调用表达式
    std::string visitCallExpr(const frontend::CallExpr* expr);
    
    // 访问成员访问表达式
    std::string visitMemberExpr(const frontend::MemberExpr* expr);
    
    // 访问分组表达式
    std::string visitGroupingExpr(const frontend::GroupingExpr* expr);
    
    // 访问变量声明语句
    void visitVarStmt(const frontend::VarStmt* stmt);
    
    // 访问常量声明语句
    void visitConstStmt(const frontend::ConstStmt* stmt);
    
    // 访问函数声明语句
    void visitFunctionStmt(const frontend::FunctionStmt* stmt);
    
    // 访问表达式语句
    void visitExpressionStmt(const frontend::ExpressionStmt* stmt);
    
    // 访问打印语句
    void visitPrintStmt(const frontend::PrintStmt* stmt);
    
    // 访问返回语句
    void visitReturnStmt(const frontend::ReturnStmt* stmt);
    
    // 访问代码块语句
    void visitBlockStmt(const frontend::BlockStmt* stmt);
    
    // 访问条件语句
    void visitIfStmt(const frontend::IfStmt* stmt);
    
    // 访问while语句
    void visitWhileStmt(const frontend::WhileStmt* stmt);
    
    // 访问for语句
    void visitForStmt(const frontend::ForStmt* stmt);
    
    // 访问结构体声明语句
    void visitStructStmt(const frontend::StructStmt* stmt);
    
    // 访问类声明语句
    void visitClassStmt(const frontend::ClassStmt* stmt);
    
    // 辅助函数：创建常量指令
    std::string createConstant(Type type, const std::string& value);
    
    // 辅助函数：创建二元操作指令
    std::string createBinaryOp(OpCode opcode, Type type, const std::string& left, const std::string& right);
    
    // 辅助函数：创建一元操作指令
    std::string createUnaryOp(OpCode opcode, Type type, const std::string& operand);
    
    // 辅助函数：创建函数调用指令
    std::string createCall(Type returnType, const std::string& funcName, const std::vector<std::string>& arguments);
    
    // 辅助函数：创建栈分配指令
    std::string createAlloca(Type type);
    
    // 辅助函数：创建加载指令
    std::string createLoad(Type type, const std::string& pointer);
    
    // 辅助函数：创建存储指令
    void createStore(Type type, const std::string& value, const std::string& pointer);
    
    // 辅助函数：创建条件分支指令
    void createCondBr(const std::string& condition, const std::string& trueBlock, const std::string& falseBlock);
    
    // 辅助函数：创建无条件分支指令
    void createBr(const std::string& targetBlock);
    
    // 辅助函数：创建返回指令
    void createRet(Type returnType, const std::string& value = "");
    
    // 辅助函数：获取类型
    Type getTypeFromASTType(const std::string& astType);
};

} // namespace middleend
} // namespace nexus

#endif // NEXUS_CODEGEN_H
