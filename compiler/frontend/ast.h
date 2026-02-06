/*
 * Nexus Compiler
 * AST definitions
 *
 * This file defines the Abstract Syntax Tree (AST) classes
 * used by the Nexus compiler to represent parsed source code.
 */

#ifndef NEXUS_AST_H
#define NEXUS_AST_H

#include <string>
#include <vector>
#include <memory>

namespace nexus {
namespace frontend {

/**
 * Expression types
 *
 * This enum defines all possible expression types in the AST.
 */
enum class ExprType {
    BINARY,      // Binary expression
    UNARY,       // Unary expression
    LITERAL,     // Literal expression
    IDENTIFIER,  // Identifier expression
    ASSIGN,      // Assignment expression
    CALL,        // Function call expression
    MEMBER,      // Member access expression
    THIS,        // this keyword expression
    SUPER,       // super keyword expression
    GROUPING,    // Grouping expression
    ARRAY,       // Array expression
    OBJECT,      // Object expression
    INDEX,       // Index access expression
    LAMBDA,      // Lambda expression
    AWAIT,       // await expression
    YIELD,       // yield expression
};

/**
 * Statement types
 *
 * This enum defines all possible statement types in the AST.
 */
enum class StmtType {
    EXPRESSION,  // Expression statement
    PRINT,       // Print statement
    VAR,         // Variable declaration
    CONST,       // Constant declaration
    BLOCK,       // Block statement
    IF,          // if statement
    WHILE,       // while statement
    FOR,         // for statement
    FOREACH,     // foreach statement
    RETURN,      // return statement
    BREAK,       // break statement
    CONTINUE,    // continue statement
    FUNCTION,    // Function declaration
    CLASS,       // Class declaration
    STRUCT,      // Struct declaration
    INTERFACE,   // Interface declaration
    ENUM,        // Enum declaration
    TRY,         // try statement
    CATCH,       // catch statement
    FINALLY,     // finally statement
    THROW,       // throw statement
    IMPORT,      // import statement
    EXPORT,      // export statement
    ASYNC,       // async statement
    COROUTINE,   // coroutine statement
    PROCESS,     // process statement
};

/**
 * Expression base class
 *
 * This is the base class for all expression types in the AST.
 */
class Expr {
public:
    virtual ~Expr() = default;
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    virtual ExprType getType() const = 0;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    virtual std::string toString() const = 0;
};

/**
 * Statement base class
 *
 * This is the base class for all statement types in the AST.
 */
class Stmt {
public:
    virtual ~Stmt() = default;
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    virtual StmtType getType() const = 0;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    virtual std::string toString() const = 0;
};

/**
 * Binary expression
 *
 * Represents a binary operation like a + b or a == b.
 */
class BinaryExpr : public Expr {
public:
    /**
     * Constructs a new binary expression
     *
     * @param left Left operand
     * @param op Operator
     * @param right Right operand
     */
    BinaryExpr(std::unique_ptr<Expr> left, const std::string& op, std::unique_ptr<Expr> right);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> left;  // Left operand
    std::string op;             // Operator
    std::unique_ptr<Expr> right; // Right operand
};

/**
 * Unary expression
 *
 * Represents a unary operation like !a or -a.
 */
class UnaryExpr : public Expr {
public:
    /**
     * Constructs a new unary expression
     *
     * @param op Operator
     * @param right Right operand
     */
    UnaryExpr(const std::string& op, std::unique_ptr<Expr> right);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::string op;             // Operator
    std::unique_ptr<Expr> right; // Right operand
};

/**
 * Literal expression
 *
 * Represents a literal value like 42, "hello", or true.
 */
class LiteralExpr : public Expr {
public:
    /**
     * Constructs a new literal expression
     *
     * @param value Literal value
     * @param type Literal type
     */
    LiteralExpr(const std::string& value, const std::string& type);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::string value; // Literal value
    std::string type;  // Literal type
};

/**
 * Identifier expression
 *
 * Represents an identifier like x or foo.
 */
class IdentifierExpr : public Expr {
public:
    /**
     * Constructs a new identifier expression
     *
     * @param name Identifier name
     */
    IdentifierExpr(const std::string& name);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::string name; // Identifier name
};

/**
 * Assignment expression
 *
 * Represents an assignment like x = 5.
 */
class AssignExpr : public Expr {
public:
    /**
     * Constructs a new assignment expression
     *
     * @param name Variable name
     * @param value Value to assign
     */
    AssignExpr(const std::string& name, std::unique_ptr<Expr> value);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::string name;            // Variable name
    std::unique_ptr<Expr> value; // Value to assign
};

/**
 * Function call expression
 *
 * Represents a function call like foo(1, 2).
 */
class CallExpr : public Expr {
public:
    /**
     * Constructs a new function call expression
     *
     * @param callee Function to call
     * @param arguments Function arguments
     */
    CallExpr(std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> arguments);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> callee;               // Function to call
    std::vector<std::unique_ptr<Expr>> arguments; // Function arguments
};

/**
 * Member access expression
 *
 * Represents a member access like obj.foo.
 */
class MemberExpr : public Expr {
public:
    /**
     * Constructs a new member access expression
     *
     * @param object Object to access
     * @param name Member name
     */
    MemberExpr(std::unique_ptr<Expr> object, const std::string& name);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> object; // Object to access
    std::string name;             // Member name
};

/**
 * This expression
 *
 * Represents the this keyword.
 */
class ThisExpr : public Expr {
public:
    /**
     * Constructs a new this expression
     */
    ThisExpr();
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
};

/**
 * Super expression
 *
 * Represents the super keyword.
 */
class SuperExpr : public Expr {
public:
    /**
     * Constructs a new super expression
     *
     * @param method Method name
     */
    SuperExpr(const std::string& method);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::string method; // Method name
};

/**
 * Grouping expression
 *
 * Represents a grouped expression like (a + b).
 */
class GroupingExpr : public Expr {
public:
    /**
     * Constructs a new grouping expression
     *
     * @param expression Expression to group
     */
    GroupingExpr(std::unique_ptr<Expr> expression);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> expression; // Expression to group
};

/**
 * Array expression
 *
 * Represents an array literal like [1, 2, 3].
 */
class ArrayExpr : public Expr {
public:
    /**
     * Constructs a new array expression
     *
     * @param elements Array elements
     */
    ArrayExpr(std::vector<std::unique_ptr<Expr>> elements);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::vector<std::unique_ptr<Expr>> elements; // Array elements
};

/**
 * Object expression
 *
 * Represents an object literal like { foo: 1, bar: 2 }.
 */
class ObjectExpr : public Expr {
public:
    /**
     * Constructs a new object expression
     *
     * @param properties Object properties
     */
    ObjectExpr(std::vector<std::pair<std::string, std::unique_ptr<Expr>>> properties);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::vector<std::pair<std::string, std::unique_ptr<Expr>>> properties; // Object properties
};

/**
 * Index access expression
 *
 * Represents an index access like arr[0] or obj[key].
 */
class IndexExpr : public Expr {
public:
    /**
     * Constructs a new index access expression
     *
     * @param object Object to access
     * @param index Index to access
     */
    IndexExpr(std::unique_ptr<Expr> object, std::unique_ptr<Expr> index);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> object; // Object to access
    std::unique_ptr<Expr> index;   // Index to access
};

/**
 * Lambda expression
 *
 * Represents a lambda function like (a, b) => a + b.
 */
class LambdaExpr : public Expr {
public:
    /**
     * Constructs a new lambda expression
     *
     * @param parameters Lambda parameters
     * @param body Lambda body
     */
    LambdaExpr(std::vector<std::pair<std::string, std::string>> parameters, std::unique_ptr<Expr> body);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::vector<std::pair<std::string, std::string>> parameters; // Lambda parameters
    std::unique_ptr<Expr> body;                               // Lambda body
};

/**
 * Await expression
 *
 * Represents an await expression like await promise.
 */
class AwaitExpr : public Expr {
public:
    /**
     * Constructs a new await expression
     *
     * @param expression Expression to await
     */
    AwaitExpr(std::unique_ptr<Expr> expression);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> expression; // Expression to await
};

/**
 * Yield expression
 *
 * Represents a yield expression like yield value.
 */
class YieldExpr : public Expr {
public:
    /**
     * Constructs a new yield expression
     *
     * @param expression Expression to yield
     */
    YieldExpr(std::unique_ptr<Expr> expression);
    
    /**
     * Gets the type of the expression
     *
     * @return The expression type
     */
    ExprType getType() const override;
    
    /**
     * Converts the expression to a string
     *
     * @return String representation of the expression
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> expression; // Expression to yield
};

/**
 * Expression statement
 *
 * Represents an expression as a statement.
 */
class ExpressionStmt : public Stmt {
public:
    /**
     * Constructs a new expression statement
     *
     * @param expression Expression to execute
     */
    ExpressionStmt(std::unique_ptr<Expr> expression);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> expression; // Expression to execute
};

/**
 * Print statement
 *
 * Represents a print statement like println("hello").
 */
class PrintStmt : public Stmt {
public:
    /**
     * Constructs a new print statement
     *
     * @param expression Expression to print
     */
    PrintStmt(std::unique_ptr<Expr> expression);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> expression; // Expression to print
};

/**
 * Variable declaration statement
 *
 * Represents a variable declaration like let x = 5.
 */
class VarStmt : public Stmt {
public:
    /**
     * Constructs a new variable declaration statement
     *
     * @param name Variable name
     * @param type Variable type
     * @param initializer Initializer expression
     */
    VarStmt(const std::string& name, const std::string& type, std::unique_ptr<Expr> initializer);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::string name;                    // Variable name
    std::string type;                    // Variable type
    std::unique_ptr<Expr> initializer;   // Initializer expression
};

/**
 * Constant declaration statement
 *
 * Represents a constant declaration like const PI = 3.14.
 */
class ConstStmt : public Stmt {
public:
    /**
     * Constructs a new constant declaration statement
     *
     * @param name Constant name
     * @param type Constant type
     * @param initializer Initializer expression
     */
    ConstStmt(const std::string& name, const std::string& type, std::unique_ptr<Expr> initializer);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::string name;                    // Constant name
    std::string type;                    // Constant type
    std::unique_ptr<Expr> initializer;   // Initializer expression
};

/**
 * Block statement
 *
 * Represents a block of statements like { ... }.
 */
class BlockStmt : public Stmt {
public:
    /**
     * Constructs a new block statement
     *
     * @param statements Statements in the block
     */
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::vector<std::unique_ptr<Stmt>> statements; // Statements in the block
};

/**
 * If statement
 *
 * Represents an if statement like if (condition) { ... } else { ... }.
 */
class IfStmt : public Stmt {
public:
    /**
     * Constructs a new if statement
     *
     * @param condition Condition expression
     * @param thenBranch Then branch
     * @param elseBranch Else branch
     */
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> condition;  // Condition expression
    std::unique_ptr<Stmt> thenBranch; // Then branch
    std::unique_ptr<Stmt> elseBranch; // Else branch
};

/**
 * While statement
 *
 * Represents a while statement like while (condition) { ... }.
 */
class WhileStmt : public Stmt {
public:
    /**
     * Constructs a new while statement
     *
     * @param condition Condition expression
     * @param body Loop body
     */
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> condition; // Condition expression
    std::unique_ptr<Stmt> body;      // Loop body
};

/**
 * For statement
 *
 * Represents a for statement like for (let i = 0; i < 10; i++) { ... }.
 */
class ForStmt : public Stmt {
public:
    /**
     * Constructs a new for statement
     *
     * @param initializer Initializer statement
     * @param condition Condition expression
     * @param increment Increment expression
     * @param body Loop body
     */
    ForStmt(std::unique_ptr<Stmt> initializer, std::unique_ptr<Expr> condition, std::unique_ptr<Expr> increment, std::unique_ptr<Stmt> body);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::unique_ptr<Stmt> initializer; // Initializer statement
    std::unique_ptr<Expr> condition;    // Condition expression
    std::unique_ptr<Expr> increment;    // Increment expression
    std::unique_ptr<Stmt> body;         // Loop body
};

/**
 * Return statement
 *
 * Represents a return statement like return 42.
 */
class ReturnStmt : public Stmt {
public:
    /**
     * Constructs a new return statement
     *
     * @param value Return value
     */
    ReturnStmt(std::unique_ptr<Expr> value);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> value; // Return value
};

/**
 * Function declaration statement
 *
 * Represents a function declaration like function foo() { ... }.
 */
class FunctionStmt : public Stmt {
public:
    /**
     * Constructs a new function declaration statement
     *
     * @param name Function name
     * @param parameters Function parameters
     * @param returnType Return type
     * @param body Function body
     * @param isAsync Whether the function is async
     * @param isCoroutine Whether the function is a coroutine
     */
    FunctionStmt(const std::string& name, std::vector<std::pair<std::string, std::string>> parameters, const std::string& returnType, std::unique_ptr<Stmt> body, bool isAsync, bool isCoroutine);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::string name;                                // Function name
    std::vector<std::pair<std::string, std::string>> parameters; // Function parameters
    std::string returnType;                          // Return type
    std::unique_ptr<Stmt> body;                     // Function body
    bool isAsync;                                   // Whether the function is async
    bool isCoroutine;                               // Whether the function is a coroutine
};

/**
 * Class declaration statement
 *
 * Represents a class declaration like class Foo { ... }.
 */
class ClassStmt : public Stmt {
public:
    /**
     * Constructs a new class declaration statement
     *
     * @param name Class name
     * @param superclass Superclass name
     * @param methods Class methods
     */
    ClassStmt(const std::string& name, const std::string& superclass, std::vector<std::unique_ptr<Stmt>> methods);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::string name;                 // Class name
    std::string superclass;           // Superclass name
    std::vector<std::unique_ptr<Stmt>> methods; // Class methods
};

/**
 * Struct declaration statement
 *
 * Represents a struct declaration like struct Point { x: int; y: int; }.
 */
class StructStmt : public Stmt {
public:
    /**
     * Constructs a new struct declaration statement
     *
     * @param name Struct name
     * @param fields Struct fields
     */
    StructStmt(const std::string& name, std::vector<std::pair<std::string, std::string>> fields);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::string name;                                // Struct name
    std::vector<std::pair<std::string, std::string>> fields; // Struct fields
};

/**
 * Try statement
 *
 * Represents a try statement like try { ... } catch (e) { ... } finally { ... }.
 */
class TryStmt : public Stmt {
public:
    /**
     * Constructs a new try statement
     *
     * @param body Try body
     * @param catches Catch clauses
     * @param finally Finally block
     */
    TryStmt(std::unique_ptr<Stmt> body, std::vector<std::unique_ptr<Stmt>> catches, std::unique_ptr<Stmt> finally);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::unique_ptr<Stmt> body;           // Try body
    std::vector<std::unique_ptr<Stmt>> catches; // Catch clauses
    std::unique_ptr<Stmt> finally;        // Finally block
};

/**
 * Catch statement
 *
 * Represents a catch clause like catch (e) { ... }.
 */
class CatchStmt : public Stmt {
public:
    /**
     * Constructs a new catch statement
     *
     * @param name Exception variable name
     * @param type Exception type
     * @param body Catch body
     */
    CatchStmt(const std::string& name, const std::string& type, std::unique_ptr<Stmt> body);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::string name;            // Exception variable name
    std::string type;            // Exception type
    std::unique_ptr<Stmt> body;   // Catch body
};

/**
 * Throw statement
 *
 * Represents a throw statement like throw new Error("message").
 */
class ThrowStmt : public Stmt {
public:
    /**
     * Constructs a new throw statement
     *
     * @param expression Expression to throw
     */
    ThrowStmt(std::unique_ptr<Expr> expression);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::unique_ptr<Expr> expression; // Expression to throw
};

/**
 * Process statement
 *
 * Represents a process declaration like process "worker" { ... }.
 */
class ProcessStmt : public Stmt {
public:
    /**
     * Constructs a new process statement
     *
     * @param id Process ID
     * @param body Process body
     */
    ProcessStmt(const std::string& id, std::unique_ptr<Expr> body);
    
    /**
     * Gets the type of the statement
     *
     * @return The statement type
     */
    StmtType getType() const override;
    
    /**
     * Converts the statement to a string
     *
     * @return String representation of the statement
     */
    std::string toString() const override;
    
    std::string id;              // Process ID
    std::unique_ptr<Expr> body;  // Process body
};

} // namespace frontend
} // namespace nexus

#endif // NEXUS_AST_H
