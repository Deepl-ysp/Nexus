#ifndef NEXUS_IR_H
#define NEXUS_IR_H

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace nexus {
namespace middleend {

// 操作码类型
enum class OpCode {
    // 算术操作
    ADD,    // 加法
    SUB,    // 减法
    MUL,    // 乘法
    DIV,    // 除法
    MOD,    // 取模
    
    // 比较操作
    EQ,     // 等于
    NE,     // 不等于
    LT,     // 小于
    LE,     // 小于等于
    GT,     // 大于
    GE,     // 大于等于
    
    // 逻辑操作
    AND,    // 逻辑与
    OR,     // 逻辑或
    NOT,    // 逻辑非
    
    // 位操作
    BIT_AND,    // 位与
    BIT_OR,     // 位或
    BIT_XOR,    // 位异或
    SHL,        // 左移
    SHR,        // 右移
    USHR,       // 无符号右移
    
    // 内存操作
    LOAD,   // 加载
    STORE,  // 存储
    ALLOC,  // 分配
    FREE,   // 释放
    
    // 控制流操作
    BR,     // 无条件分支
    COND_BR,    // 条件分支
    PHI,    // PHI 节点
    CALL,   // 函数调用
    RET,    // 返回
    
    // 其他操作
    CONST,  // 常量
    GLOBAL, // 全局变量
    ALLOCA, // 栈分配
    GETELEMENTPTR, // 获取元素指针
};

// 类型
enum class Type {
    VOID,
    BOOL,
    INT8,
    INT16,
    INT32,
    INT64,
    FLOAT,
    DOUBLE,
    POINTER,
    ARRAY,
    STRUCT,
};

// 指令基类
class Instruction {
public:
    virtual ~Instruction() = default;
    virtual OpCode getOpcode() const = 0;
    virtual std::string toString() const = 0;
    
    std::string name; // 指令名称（用于引用）
};

// 基本块
class BasicBlock {
public:
    BasicBlock(const std::string& name);
    
    void addInstruction(std::unique_ptr<Instruction> instr);
    const std::vector<std::unique_ptr<Instruction>>& getInstructions() const;
    std::string toString() const;
    
    std::string name;
    std::vector<std::unique_ptr<Instruction>> instructions;
};

// 函数
class Function {
public:
    Function(const std::string& name, Type returnType);
    
    void addBasicBlock(std::unique_ptr<BasicBlock> block);
    const std::vector<std::unique_ptr<BasicBlock>>& getBasicBlocks() const;
    std::string toString() const;
    
    std::string name;
    Type returnType;
    std::vector<std::unique_ptr<BasicBlock>> basicBlocks;
};

// 模块
class Module {
public:
    Module(const std::string& name);
    
    void addFunction(std::unique_ptr<Function> func);
    const std::vector<std::unique_ptr<Function>>& getFunctions() const;
    std::string toString() const;
    
    std::string name;
    std::vector<std::unique_ptr<Function>> functions;
};

// 常量指令
class ConstInst : public Instruction {
public:
    ConstInst(Type type, const std::string& value);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    Type type;
    std::string value;
};

// 二元操作指令
class BinaryInst : public Instruction {
public:
    BinaryInst(OpCode opcode, Type type, const std::string& left, const std::string& right);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    OpCode opcode;
    Type type;
    std::string left;
    std::string right;
};

// 一元操作指令
class UnaryInst : public Instruction {
public:
    UnaryInst(OpCode opcode, Type type, const std::string& operand);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    OpCode opcode;
    Type type;
    std::string operand;
};

// 条件分支指令
class CondBrInst : public Instruction {
public:
    CondBrInst(const std::string& condition, const std::string& trueBlock, const std::string& falseBlock);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    std::string condition;
    std::string trueBlock;
    std::string falseBlock;
};

// 无条件分支指令
class BrInst : public Instruction {
public:
    BrInst(const std::string& targetBlock);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    std::string targetBlock;
};

// 函数调用指令
class CallInst : public Instruction {
public:
    CallInst(Type returnType, const std::string& funcName, const std::vector<std::string>& arguments);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    Type returnType;
    std::string funcName;
    std::vector<std::string> arguments;
};

// 返回指令
class RetInst : public Instruction {
public:
    RetInst(Type returnType, const std::string& value = "");
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    Type returnType;
    std::string value;
};

// 栈分配指令
class AllocaInst : public Instruction {
public:
    AllocaInst(Type type);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    Type type;
};

// 加载指令
class LoadInst : public Instruction {
public:
    LoadInst(Type type, const std::string& pointer);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    Type type;
    std::string pointer;
};

// 存储指令
class StoreInst : public Instruction {
public:
    StoreInst(Type type, const std::string& value, const std::string& pointer);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    Type type;
    std::string value;
    std::string pointer;
};

// PHI 节点指令
class PhiInst : public Instruction {
public:
    PhiInst(Type type, const std::vector<std::pair<std::string, std::string>>& incoming);
    OpCode getOpcode() const override;
    std::string toString() const override;
    
    Type type;
    std::vector<std::pair<std::string, std::string>> incoming;
};

// 类型转换函数
std::string typeToString(Type type);

// 操作码转换函数
std::string opcodeToString(OpCode opcode);

} // namespace middleend
} // namespace nexus

#endif // NEXUS_IR_H
