#ifndef NEXUS_SEMANTIC_ANALYZER_H
#define NEXUS_SEMANTIC_ANALYZER_H

#include "./ast.h"
#include <vector>
#include <map>
#include <string>

namespace nexus {
namespace frontend {

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    
    // 分析程序
    void analyze(const std::vector<std::unique_ptr<Stmt>>& statements);
    
    // 检查是否有错误
    bool hadError() const;
    
private:
    bool _hadError;
    
    // 符号表
    struct Scope {
        std::map<std::string, std::string> variables; // 变量名 -> 类型
        std::map<std::string, std::string> functions; // 函数名 -> 返回类型
        std::map<std::string, std::vector<std::pair<std::string, std::string>>> structs; // 结构体名 -> 字段列表
    };
    
    std::vector<Scope> scopes;
    
    // 错误处理
    void error(const std::string& message);
    
    // 进入和退出作用域
    void enterScope();
    void exitScope();
    
    // 变量和函数操作
    bool isVariableDefined(const std::string& name);
    bool isFunctionDefined(const std::string& name);
    bool isStructDefined(const std::string& name);
    void defineVariable(const std::string& name, const std::string& type);
    void defineFunction(const std::string& name, const std::string& returnType);
    void defineStruct(const std::string& name, const std::vector<std::pair<std::string, std::string>>& fields);
    
    // 分析语句
    void analyzeStatement(const Stmt* stmt);
    void analyzeBlockStatement(const BlockStmt* stmt);
    void analyzeVarStatement(const VarStmt* stmt);
    void analyzeConstStatement(const ConstStmt* stmt);
    void analyzeFunctionStatement(const FunctionStmt* stmt);
    void analyzeClassStatement(const ClassStmt* stmt);
    void analyzeStructStatement(const StructStmt* stmt);
    void analyzeIfStatement(const IfStmt* stmt);
    void analyzeWhileStatement(const WhileStmt* stmt);
    void analyzeForStatement(const ForStmt* stmt);
    void analyzeReturnStatement(const ReturnStmt* stmt);
    void analyzeExpressionStatement(const ExpressionStmt* stmt);
    void analyzePrintStatement(const PrintStmt* stmt);
    void analyzeTryStatement(const TryStmt* stmt);
    void analyzeCatchStatement(const CatchStmt* stmt);
    void analyzeThrowStatement(const ThrowStmt* stmt);
    void analyzeProcessStatement(const ProcessStmt* stmt);
    
    // 分析表达式
    std::string analyzeExpression(const Expr* expr);
    std::string analyzeBinaryExpr(const BinaryExpr* expr);
    std::string analyzeUnaryExpr(const UnaryExpr* expr);
    std::string analyzeLiteralExpr(const LiteralExpr* expr);
    std::string analyzeIdentifierExpr(const IdentifierExpr* expr);
    std::string analyzeAssignExpr(const AssignExpr* expr);
    std::string analyzeCallExpr(const CallExpr* expr);
    std::string analyzeMemberExpr(const MemberExpr* expr);
    std::string analyzeThisExpr(const ThisExpr* expr);
    std::string analyzeSuperExpr(const SuperExpr* expr);
    std::string analyzeGroupingExpr(const GroupingExpr* expr);
};

} // namespace frontend
} // namespace nexus

#endif // NEXUS_SEMANTIC_ANALYZER_H
