/**
 * @file codegen.cpp
 * @brief 后端代码生成器实现，将 IR 转换为 x86_64 汇编代码
 * @author Nexus 编译器团队
 * @version 1.0
 */

#include "./codegen.h"

namespace nexus {
namespace backend {

/**
 * @brief CodeGenerator 构造函数
 * 
 * 初始化代码生成器的成员变量
 */
CodeGenerator::CodeGenerator()
    : code(""), instrCount(0) {
}

/**
 * @brief 生成汇编代码
 * 
 * @param module IR 模块
 * @return std::string 生成的汇编代码
 */
std::string CodeGenerator::generate(const middleend::Module& module) {
    // 重置状态
    code = "";
    functionMap.clear();
    variableMap.clear();
    instrCount = 0;
    
    // 添加文件头
    code += "; Nexus Backend Code Generator\n";
    code += "; Generated x86_64 Assembly Code\n\n";
    
    // 添加外部函数声明
    code += "; External functions\n";
    code += "extern printf\n\n";
    
    // 生成所有函数
    for (const auto& func : module.functions) {
        generateFunction(func.get());
    }
    
    return code;
}

/**
 * @brief 获取生成的代码
 * 
 * @return const std::string& 生成的汇编代码
 */
const std::string& CodeGenerator::getCode() const {
    return code;
}

/**
 * @brief 生成指令名称
 * 
 * @return std::string 生成的指令名称
 */
std::string CodeGenerator::generateInstrName() {
    return "instr" + std::to_string(instrCount++);
}

/**
 * @brief 生成函数的汇编代码
 * 
 * @param func 函数指针
 */
void CodeGenerator::generateFunction(const middleend::Function* func) {
    // 添加函数标签
    code += "; Function: " + func->name + "\n";
    code += func->name + ":\n";
    
    // 保存调用者保存的寄存器
    code += "    push rbp\n";
    code += "    mov rbp, rsp\n";
    
    // 生成所有基本块
    for (const auto& block : func->basicBlocks) {
        generateBlock(block.get());
    }
    
    // 恢复调用者保存的寄存器
    code += "    mov rsp, rbp\n";
    code += "    pop rbp\n";
    code += "    ret\n\n";
}

/**
 * @brief 生成基本块的汇编代码
 * 
 * @param block 基本块指针
 */
void CodeGenerator::generateBlock(const middleend::BasicBlock* block) {
    // 添加基本块标签
    code += "; Block: " + block->name + "\n";
    code += block->name + ":\n";
    
    // 生成所有指令
    for (const auto& instr : block->instructions) {
        generateInstruction(instr.get());
    }
}

/**
 * @brief 生成指令的汇编代码
 * 
 * @param instr 指令指针
 */
void CodeGenerator::generateInstruction(const middleend::Instruction* instr) {
    // 根据指令类型生成代码
    if (auto* constInst = dynamic_cast<const middleend::ConstInst*>(instr)) {
        generateConstInst(constInst);
    } else if (auto* binaryInst = dynamic_cast<const middleend::BinaryInst*>(instr)) {
        generateBinaryInst(binaryInst);
    } else if (auto* unaryInst = dynamic_cast<const middleend::UnaryInst*>(instr)) {
        generateUnaryInst(unaryInst);
    } else if (auto* condBrInst = dynamic_cast<const middleend::CondBrInst*>(instr)) {
        generateCondBrInst(condBrInst);
    } else if (auto* brInst = dynamic_cast<const middleend::BrInst*>(instr)) {
        generateBrInst(brInst);
    } else if (auto* callInst = dynamic_cast<const middleend::CallInst*>(instr)) {
        generateCallInst(callInst);
    } else if (auto* retInst = dynamic_cast<const middleend::RetInst*>(instr)) {
        generateRetInst(retInst);
    } else if (auto* allocaInst = dynamic_cast<const middleend::AllocaInst*>(instr)) {
        generateAllocaInst(allocaInst);
    } else if (auto* loadInst = dynamic_cast<const middleend::LoadInst*>(instr)) {
        generateLoadInst(loadInst);
    } else if (auto* storeInst = dynamic_cast<const middleend::StoreInst*>(instr)) {
        generateStoreInst(storeInst);
    } else if (auto* phiInst = dynamic_cast<const middleend::PhiInst*>(instr)) {
        generatePhiInst(phiInst);
    }
}

/**
 * @brief 生成常量指令的汇编代码
 * 
 * @param instr 常量指令指针
 */
void CodeGenerator::generateConstInst(const middleend::ConstInst* instr) {
    // 常量指令在汇编中通常不需要生成代码
    // 常量值会在使用时直接加载
    code += "; Const: " + instr->name + " = " + instr->value + "\n";
}

/**
 * @brief 生成二元操作指令的汇编代码
 * 
 * @param instr 二元操作指令指针
 */
void CodeGenerator::generateBinaryInst(const middleend::BinaryInst* instr) {
    // 生成二元操作指令
    code += "; BinaryOp: " + instr->name + " = " + 
            middleend::opcodeToString(instr->opcode) + " " + 
            middleend::typeToString(instr->type) + " " + 
            instr->left + ", " + instr->right + "\n";
    
    // 实现二元操作指令的代码生成
    // 这里简化处理，假设使用 rax 和 rbx 寄存器
    code += "    ; Load operands into registers\n";
    code += "    mov rax, " + instr->left + "\n";
    code += "    mov rbx, " + instr->right + "\n";
    
    // 根据操作码生成相应的指令
    switch (instr->opcode) {
        case middleend::OpCode::ADD:
            code += "    add rax, rbx\n";
            break;
        case middleend::OpCode::SUB:
            code += "    sub rax, rbx\n";
            break;
        case middleend::OpCode::MUL:
            code += "    imul rax, rbx\n";
            break;
        case middleend::OpCode::DIV:
            code += "    xor rdx, rdx\n";
            code += "    idiv rbx\n";
            break;
        case middleend::OpCode::MOD:
            code += "    xor rdx, rdx\n";
            code += "    idiv rbx\n";
            code += "    mov rax, rdx\n";
            break;
        case middleend::OpCode::EQ:
            code += "    cmp rax, rbx\n";
            code += "    sete al\n";
            code += "    movzx rax, al\n";
            break;
        case middleend::OpCode::NE:
            code += "    cmp rax, rbx\n";
            code += "    setne al\n";
            code += "    movzx rax, al\n";
            break;
        case middleend::OpCode::LT:
            code += "    cmp rax, rbx\n";
            code += "    setl al\n";
            code += "    movzx rax, al\n";
            break;
        case middleend::OpCode::LE:
            code += "    cmp rax, rbx\n";
            code += "    setle al\n";
            code += "    movzx rax, al\n";
            break;
        case middleend::OpCode::GT:
            code += "    cmp rax, rbx\n";
            code += "    setg al\n";
            code += "    movzx rax, al\n";
            break;
        case middleend::OpCode::GE:
            code += "    cmp rax, rbx\n";
            code += "    setge al\n";
            code += "    movzx rax, al\n";
            break;
        default:
            code += "    ; TODO: Implement other binary operations\n";
            break;
    }
    
    // 保存结果
    code += "    mov " + instr->name + ", rax\n";
}

/**
 * @brief 生成一元操作指令的汇编代码
 * 
 * @param instr 一元操作指令指针
 */
void CodeGenerator::generateUnaryInst(const middleend::UnaryInst* instr) {
    // 生成一元操作指令
    code += "; UnaryOp: " + instr->name + " = " + 
            middleend::opcodeToString(instr->opcode) + " " + 
            middleend::typeToString(instr->type) + " " + 
            instr->operand + "\n";
    
    // 实现一元操作指令的代码生成
    // 这里简化处理，假设使用 rax 寄存器
    code += "    ; Load operand into register\n";
    code += "    mov rax, " + instr->operand + "\n";
    
    // 根据操作码生成相应的指令
    switch (instr->opcode) {
        case middleend::OpCode::NOT:
            code += "    not rax\n";
            break;
        case middleend::OpCode::SUB:
            code += "    neg rax\n";
            break;
        default:
            code += "    ; TODO: Implement other unary operations\n";
            break;
    }
    
    // 保存结果
    code += "    mov " + instr->name + ", rax\n";
}

/**
 * @brief 生成条件分支指令的汇编代码
 * 
 * @param instr 条件分支指令指针
 */
void CodeGenerator::generateCondBrInst(const middleend::CondBrInst* instr) {
    // 生成条件分支指令
    code += "; CondBr: " + instr->condition + " ? " + 
            instr->trueBlock + " : " + instr->falseBlock + "\n";
    
    // 实现条件分支指令的代码生成
    // 这里简化处理，假设条件值在 rax 寄存器中
    code += "    ; Load condition into register\n";
    code += "    mov rax, " + instr->condition + "\n";
    code += "    ; Check if condition is zero\n";
    code += "    cmp rax, 0\n";
    code += "    ; Jump to false block if condition is zero\n";
    code += "    je " + instr->falseBlock + "\n";
    code += "    ; Jump to true block if condition is non-zero\n";
    code += "    jmp " + instr->trueBlock + "\n";
}

/**
 * @brief 生成无条件分支指令的汇编代码
 * 
 * @param instr 无条件分支指令指针
 */
void CodeGenerator::generateBrInst(const middleend::BrInst* instr) {
    // 生成无条件分支指令
    code += "; Br: jmp " + instr->targetBlock + "\n";
    code += "    jmp " + instr->targetBlock + "\n";
}

/**
 * @brief 生成函数调用指令的汇编代码
 * 
 * @param instr 函数调用指令指针
 */
void CodeGenerator::generateCallInst(const middleend::CallInst* instr) {
    // 生成函数调用指令
    code += "; Call: " + instr->name + " = " + 
            instr->funcName + "(";
    
    for (size_t i = 0; i < instr->arguments.size(); i++) {
        if (i > 0) {
            code += ", ";
        }
        code += instr->arguments[i];
    }
    code += ")\n";
    
    // 实现函数调用指令的代码生成
    // 这里简化处理，使用 x86_64 调用约定
    // 前 6 个参数使用寄存器 rdi, rsi, rdx, rcx, r8, r9
    // 更多参数使用栈
    
    // 保存调用者保存的寄存器
    code += "    ; Save caller-saved registers\n";
    code += "    push rdi\n";
    code += "    push rsi\n";
    code += "    push rdx\n";
    code += "    push rcx\n";
    code += "    push r8\n";
    code += "    push r9\n";
    
    // 传递参数
    if (instr->arguments.size() > 0) {
        code += "    ; Pass arguments\n";
        if (instr->arguments.size() >= 1) {
            code += "    mov rdi, " + instr->arguments[0] + "\n";
        }
        if (instr->arguments.size() >= 2) {
            code += "    mov rsi, " + instr->arguments[1] + "\n";
        }
        if (instr->arguments.size() >= 3) {
            code += "    mov rdx, " + instr->arguments[2] + "\n";
        }
        if (instr->arguments.size() >= 4) {
            code += "    mov rcx, " + instr->arguments[3] + "\n";
        }
        if (instr->arguments.size() >= 5) {
            code += "    mov r8, " + instr->arguments[4] + "\n";
        }
        if (instr->arguments.size() >= 6) {
            code += "    mov r9, " + instr->arguments[5] + "\n";
        }
        // 更多参数使用栈
        for (size_t i = 6; i < instr->arguments.size(); i++) {
            code += "    push " + instr->arguments[i] + "\n";
        }
    }
    
    // 调用函数
    code += "    ; Call function\n";
    code += "    call " + instr->funcName + "\n";
    
    // 清理栈上的参数
    if (instr->arguments.size() > 6) {
        size_t stackArgs = instr->arguments.size() - 6;
        code += "    ; Clean up stack arguments\n";
        code += "    add rsp, " + std::to_string(stackArgs * 8) + "\n";
    }
    
    // 保存返回值
    code += "    ; Save return value\n";
    code += "    mov " + instr->name + ", rax\n";
    
    // 恢复调用者保存的寄存器
    code += "    ; Restore caller-saved registers\n";
    code += "    pop r9\n";
    code += "    pop r8\n";
    code += "    pop rcx\n";
    code += "    pop rdx\n";
    code += "    pop rsi\n";
    code += "    pop rdi\n";
}

/**
 * @brief 生成返回指令的汇编代码
 * 
 * @param instr 返回指令指针
 */
void CodeGenerator::generateRetInst(const middleend::RetInst* instr) {
    // 生成返回指令
    code += "; Ret";
    if (!instr->value.empty()) {
        code += ": " + instr->value;
    }
    code += "\n";
    
    // 这里简化处理，假设返回值已经在正确的寄存器中
    // 实际实现需要根据返回类型生成相应的代码
    code += "    ; TODO: Implement return value handling\n";
    code += "    ret\n";
}

/**
 * @brief 生成栈分配指令的汇编代码
 * 
 * @param instr 栈分配指令指针
 */
void CodeGenerator::generateAllocaInst(const middleend::AllocaInst* instr) {
    // 生成栈分配指令
    code += "; Alloca: " + instr->name + " = alloca " + 
            middleend::typeToString(instr->type) + "\n";
    
    // 简化处理：在栈上分配空间
    code += "    sub rsp, 8\n";
    code += "    mov " + instr->name + ", rsp\n";
}

/**
 * @brief 生成加载指令的汇编代码
 * 
 * @param instr 加载指令指针
 */
void CodeGenerator::generateLoadInst(const middleend::LoadInst* instr) {
    // 生成加载指令
    code += "; Load: " + instr->name + " = load " + 
            middleend::typeToString(instr->type) + " from " + 
            instr->pointer + "\n";
    
    // 实现加载指令的代码生成
    code += "    ; Load value from memory\n";
    code += "    mov rax, " + instr->pointer + "\n";
    code += "    mov " + instr->name + ", [rax]\n";
}

/**
 * @brief 生成存储指令的汇编代码
 * 
 * @param instr 存储指令指针
 */
void CodeGenerator::generateStoreInst(const middleend::StoreInst* instr) {
    // 生成存储指令
    code += "; Store: store " + 
            middleend::typeToString(instr->type) + " " + 
            instr->value + " to " + 
            instr->pointer + "\n";
    
    // 实现存储指令的代码生成
    code += "    ; Store value to memory\n";
    code += "    mov rax, " + instr->pointer + "\n";
    code += "    mov rbx, " + instr->value + "\n";
    code += "    mov [rax], rbx\n";
}

/**
 * @brief 生成 PHI 节点指令的汇编代码
 * 
 * @param instr PHI 节点指令指针
 */
void CodeGenerator::generatePhiInst(const middleend::PhiInst* instr) {
    // 生成 PHI 节点指令
    code += "; Phi: " + instr->name + " = phi " + 
            middleend::typeToString(instr->type) + " [";
    
    for (size_t i = 0; i < instr->incoming.size(); i++) {
        if (i > 0) {
            code += ", ";
        }
        code += instr->incoming[i].first + " from " + instr->incoming[i].second;
    }
    code += "]\n";
    
    // 这里简化处理，PHI 节点通常在基本块开始时处理
    // 实际实现需要根据传入值和基本块生成相应的代码
    code += "    ; TODO: Implement phi node\n";
}

/**
 * @brief 获取类型的汇编代码表示
 * 
 * @param type 类型
 * @return std::string 类型的汇编代码表示
 */
std::string CodeGenerator::getTypeCode(middleend::Type type) {
    // 获取类型的汇编代码表示
    switch (type) {
        case middleend::Type::VOID:
            return "void";
        case middleend::Type::BOOL:
            return "byte";
        case middleend::Type::INT8:
            return "byte";
        case middleend::Type::INT16:
            return "word";
        case middleend::Type::INT32:
            return "dword";
        case middleend::Type::INT64:
            return "qword";
        case middleend::Type::FLOAT:
            return "dword";
        case middleend::Type::DOUBLE:
            return "qword";
        case middleend::Type::POINTER:
            return "qword";
        default:
            return "unknown";
    }
}

} // namespace backend
} // namespace nexus
