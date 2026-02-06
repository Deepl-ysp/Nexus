#include "./ir.h"

namespace nexus {
namespace middleend {

/**
 * @brief BasicBlock类的构造函数
 * 
 * 创建一个基本块，用于存储一系列指令。
 * 
 * @param name 基本块名称
 */
BasicBlock::BasicBlock(const std::string& name)
    : name(name) {
}

/**
 * @brief 向基本块添加指令
 * 
 * @param instr 要添加的指令
 */
void BasicBlock::addInstruction(std::unique_ptr<Instruction> instr) {
    instructions.push_back(std::move(instr));
}

/**
 * @brief 获取基本块中的指令列表
 * 
 * @return 指令列表的常量引用
 */
const std::vector<std::unique_ptr<Instruction>>& BasicBlock::getInstructions() const {
    return instructions;
}

/**
 * @brief 将基本块转换为字符串表示
 * 
 * @return 基本块的字符串表示
 */
std::string BasicBlock::toString() const {
    std::string result = name + ":\n";
    for (const auto& instr : instructions) {
        if (!instr->name.empty()) {
            result += "  " + instr->name + " = ";
        } else {
            result += "  ";
        }
        result += instr->toString() + "\n";
    }
    return result;
}

/**
 * @brief Function类的构造函数
 * 
 * 创建一个函数，包含多个基本块。
 * 
 * @param name 函数名称
 * @param returnType 返回类型
 */
Function::Function(const std::string& name, Type returnType)
    : name(name), returnType(returnType) {
}

/**
 * @brief 向函数添加基本块
 * 
 * @param block 要添加的基本块
 */
void Function::addBasicBlock(std::unique_ptr<BasicBlock> block) {
    basicBlocks.push_back(std::move(block));
}

/**
 * @brief 获取函数中的基本块列表
 * 
 * @return 基本块列表的常量引用
 */
const std::vector<std::unique_ptr<BasicBlock>>& Function::getBasicBlocks() const {
    return basicBlocks;
}

/**
 * @brief 将函数转换为字符串表示
 * 
 * @return 函数的字符串表示
 */
std::string Function::toString() const {
    std::string result = "define " + typeToString(returnType) + " @" + name + "() {\n";
    for (const auto& block : basicBlocks) {
        result += block->toString() + "\n";
    }
    result += "}\n";
    return result;
}

/**
 * @brief Module类的构造函数
 * 
 * 创建一个模块，包含多个函数。
 * 
 * @param name 模块名称
 */
Module::Module(const std::string& name)
    : name(name) {
}

/**
 * @brief 向模块添加函数
 * 
 * @param func 要添加的函数
 */
void Module::addFunction(std::unique_ptr<Function> func) {
    functions.push_back(std::move(func));
}

/**
 * @brief 获取模块中的函数列表
 * 
 * @return 函数列表的常量引用
 */
const std::vector<std::unique_ptr<Function>>& Module::getFunctions() const {
    return functions;
}

/**
 * @brief 将模块转换为字符串表示
 * 
 * @return 模块的字符串表示
 */
std::string Module::toString() const {
    std::string result = "module @" + name + "\n\n";
    for (const auto& func : functions) {
        result += func->toString() + "\n";
    }
    return result;
}

/**
 * @brief ConstInst类的构造函数
 * 
 * 创建一个常量指令，用于表示常量值。
 * 
 * @param type 常量类型
 * @param value 常量值
 */
ConstInst::ConstInst(Type type, const std::string& value)
    : type(type), value(value) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::CONST
 */
OpCode ConstInst::getOpcode() const {
    return OpCode::CONST;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string ConstInst::toString() const {
    return "const " + typeToString(type) + " " + value;
}

/**
 * @brief BinaryInst类的构造函数
 * 
 * 创建一个二元操作指令，用于表示二元运算。
 * 
 * @param opcode 操作码
 * @param type 操作类型
 * @param left 左操作数
 * @param right 右操作数
 */
BinaryInst::BinaryInst(OpCode opcode, Type type, const std::string& left, const std::string& right)
    : opcode(opcode), type(type), left(left), right(right) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码
 */
OpCode BinaryInst::getOpcode() const {
    return opcode;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string BinaryInst::toString() const {
    return opcodeToString(opcode) + " " + typeToString(type) + " " + left + ", " + right;
}

/**
 * @brief UnaryInst类的构造函数
 * 
 * 创建一个一元操作指令，用于表示一元运算。
 * 
 * @param opcode 操作码
 * @param type 操作类型
 * @param operand 操作数
 */
UnaryInst::UnaryInst(OpCode opcode, Type type, const std::string& operand)
    : opcode(opcode), type(type), operand(operand) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码
 */
OpCode UnaryInst::getOpcode() const {
    return opcode;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string UnaryInst::toString() const {
    return opcodeToString(opcode) + " " + typeToString(type) + " " + operand;
}

/**
 * @brief CondBrInst类的构造函数
 * 
 * 创建一个条件分支指令，用于根据条件跳转到不同的基本块。
 * 
 * @param condition 条件表达式
 * @param trueBlock 条件为真时跳转的基本块
 * @param falseBlock 条件为假时跳转的基本块
 */
CondBrInst::CondBrInst(const std::string& condition, const std::string& trueBlock, const std::string& falseBlock)
    : condition(condition), trueBlock(trueBlock), falseBlock(falseBlock) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::COND_BR
 */
OpCode CondBrInst::getOpcode() const {
    return OpCode::COND_BR;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string CondBrInst::toString() const {
    return "cond_br i1 " + condition + ", label %" + trueBlock + ", label %" + falseBlock;
}

/**
 * @brief BrInst类的构造函数
 * 
 * 创建一个无条件分支指令，用于跳转到指定的基本块。
 * 
 * @param targetBlock 目标基本块
 */
BrInst::BrInst(const std::string& targetBlock)
    : targetBlock(targetBlock) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::BR
 */
OpCode BrInst::getOpcode() const {
    return OpCode::BR;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string BrInst::toString() const {
    return "br label %" + targetBlock;
}

/**
 * @brief CallInst类的构造函数
 * 
 * 创建一个函数调用指令，用于调用函数。
 * 
 * @param returnType 返回类型
 * @param funcName 函数名称
 * @param arguments 参数列表
 */
CallInst::CallInst(Type returnType, const std::string& funcName, const std::vector<std::string>& arguments)
    : returnType(returnType), funcName(funcName), arguments(arguments) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::CALL
 */
OpCode CallInst::getOpcode() const {
    return OpCode::CALL;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string CallInst::toString() const {
    std::string result = "call " + typeToString(returnType) + " @" + funcName + "(";
    for (size_t i = 0; i < arguments.size(); i++) {
        if (i > 0) {
            result += ", ";
        }
        result += arguments[i];
    }
    result += ")";
    return result;
}

/**
 * @brief RetInst类的构造函数
 * 
 * 创建一个返回指令，用于从函数返回值。
 * 
 * @param returnType 返回类型
 * @param value 返回值
 */
RetInst::RetInst(Type returnType, const std::string& value)
    : returnType(returnType), value(value) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::RET
 */
OpCode RetInst::getOpcode() const {
    return OpCode::RET;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string RetInst::toString() const {
    if (returnType == Type::VOID) {
        return "ret void";
    } else {
        return "ret " + typeToString(returnType) + " " + value;
    }
}

/**
 * @brief AllocaInst类的构造函数
 * 
 * 创建一个栈分配指令，用于在栈上分配内存。
 * 
 * @param type 分配的类型
 */
AllocaInst::AllocaInst(Type type)
    : type(type) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::ALLOCA
 */
OpCode AllocaInst::getOpcode() const {
    return OpCode::ALLOCA;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string AllocaInst::toString() const {
    return "alloca " + typeToString(type);
}

/**
 * @brief LoadInst类的构造函数
 * 
 * 创建一个加载指令，用于从内存加载值。
 * 
 * @param type 加载的类型
 * @param pointer 内存指针
 */
LoadInst::LoadInst(Type type, const std::string& pointer)
    : type(type), pointer(pointer) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::LOAD
 */
OpCode LoadInst::getOpcode() const {
    return OpCode::LOAD;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string LoadInst::toString() const {
    return "load " + typeToString(type) + ", ptr " + pointer;
}

/**
 * @brief StoreInst类的构造函数
 * 
 * 创建一个存储指令，用于将值存储到内存。
 * 
 * @param type 存储的类型
 * @param value 要存储的值
 * @param pointer 内存指针
 */
StoreInst::StoreInst(Type type, const std::string& value, const std::string& pointer)
    : type(type), value(value), pointer(pointer) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::STORE
 */
OpCode StoreInst::getOpcode() const {
    return OpCode::STORE;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string StoreInst::toString() const {
    return "store " + typeToString(type) + " " + value + ", ptr " + pointer;
}

/**
 * @brief PhiInst类的构造函数
 * 
 * 创建一个PHI节点指令，用于处理基本块合并时的值选择。
 * 
 * @param type PHI节点的类型
 * @param incoming  incoming值和基本块的对列表
 */
PhiInst::PhiInst(Type type, const std::vector<std::pair<std::string, std::string>>& incoming)
    : type(type), incoming(incoming) {
}

/**
 * @brief 获取指令的操作码
 * 
 * @return 操作码，返回 OpCode::PHI
 */
OpCode PhiInst::getOpcode() const {
    return OpCode::PHI;
}

/**
 * @brief 将指令转换为字符串表示
 * 
 * @return 指令的字符串表示
 */
std::string PhiInst::toString() const {
    std::string result = "phi " + typeToString(type) + " [";
    for (size_t i = 0; i < incoming.size(); i++) {
        if (i > 0) {
            result += ", ";
        }
        result += incoming[i].first + ", label %" + incoming[i].second;
    }
    result += "]";
    return result;
}

/**
 * @brief 将Type枚举转换为字符串表示
 * 
 * @param type Type枚举值
 * @return 类型的字符串表示
 */
std::string typeToString(Type type) {
    switch (type) {
        case Type::VOID:
            return "void";
        case Type::BOOL:
            return "i1";
        case Type::INT8:
            return "i8";
        case Type::INT16:
            return "i16";
        case Type::INT32:
            return "i32";
        case Type::INT64:
            return "i64";
        case Type::FLOAT:
            return "float";
        case Type::DOUBLE:
            return "double";
        case Type::POINTER:
            return "ptr";
        case Type::ARRAY:
            return "array";
        case Type::STRUCT:
            return "struct";
        default:
            return "unknown";
    }
}

/**
 * @brief 将OpCode枚举转换为字符串表示
 * 
 * @param opcode OpCode枚举值
 * @return 操作码的字符串表示
 */
std::string opcodeToString(OpCode opcode) {
    switch (opcode) {
        case OpCode::ADD:
            return "add";
        case OpCode::SUB:
            return "sub";
        case OpCode::MUL:
            return "mul";
        case OpCode::DIV:
            return "div";
        case OpCode::MOD:
            return "mod";
        case OpCode::EQ:
            return "eq";
        case OpCode::NE:
            return "ne";
        case OpCode::LT:
            return "lt";
        case OpCode::LE:
            return "le";
        case OpCode::GT:
            return "gt";
        case OpCode::GE:
            return "ge";
        case OpCode::AND:
            return "and";
        case OpCode::OR:
            return "or";
        case OpCode::NOT:
            return "not";
        case OpCode::BIT_AND:
            return "bitand";
        case OpCode::BIT_OR:
            return "bitor";
        case OpCode::BIT_XOR:
            return "bitxor";
        case OpCode::SHL:
            return "shl";
        case OpCode::SHR:
            return "shr";
        case OpCode::USHR:
            return "ushr";
        case OpCode::LOAD:
            return "load";
        case OpCode::STORE:
            return "store";
        case OpCode::ALLOC:
            return "alloc";
        case OpCode::FREE:
            return "free";
        case OpCode::BR:
            return "br";
        case OpCode::COND_BR:
            return "cond_br";
        case OpCode::PHI:
            return "phi";
        case OpCode::CALL:
            return "call";
        case OpCode::RET:
            return "ret";
        case OpCode::CONST:
            return "const";
        case OpCode::GLOBAL:
            return "global";
        case OpCode::ALLOCA:
            return "alloca";
        case OpCode::GETELEMENTPTR:
            return "getelementptr";
        default:
            return "unknown";
    }
}

} // namespace middleend
} // namespace nexus
