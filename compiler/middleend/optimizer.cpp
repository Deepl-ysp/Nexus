/**
 * @file optimizer.cpp
 * @brief 优化器实现，对 IR 进行各种优化
 * @author Nexus 编译器团队
 * @version 1.0
 */

#include "./optimizer.h"

namespace nexus {
namespace middleend {

/**
 * @brief Optimizer 构造函数
 * 
 * 初始化优化器的成员变量
 */
Optimizer::Optimizer()
    : module(nullptr) {
}

/**
 * @brief 对模块进行优化
 * 
 * @param module 要优化的模块
 * @return std::unique_ptr<Module> 优化后的模块
 */
std::unique_ptr<Module> Optimizer::optimize(std::unique_ptr<Module> module) {
    this->module = std::move(module);
    
    // 优化所有函数
    for (auto& func : this->module->functions) {
        optimizeFunction(func.get());
    }
    
    return std::move(this->module);
}

/**
 * @brief 获取优化后的模块
 * 
 * @return const std::unique_ptr<Module>& 优化后的模块
 */
const std::unique_ptr<Module>& Optimizer::getModule() const {
    return module;
}

/**
 * @brief 优化函数
 * 
 * @param func 要优化的函数
 */
void Optimizer::optimizeFunction(Function* func) {
    // 优化所有基本块
    for (auto& block : func->basicBlocks) {
        optimizeBlock(block.get());
    }
}

/**
 * @brief 优化基本块
 * 
 * @param block 要优化的基本块
 */
void Optimizer::optimizeBlock(BasicBlock* block) {
    // 执行常量折叠
    constantFolding(block);
    
    // 执行表达式简化
    simplifyExpressions(block);
    
    // 执行死代码消除
    deadCodeElimination(block);
}

/**
 * @brief 常量折叠优化
 * 
 * @param block 要优化的基本块
 */
void Optimizer::constantFolding(BasicBlock* block) {
    // 遍历所有指令
    for (auto& instr : block->instructions) {
        // 检查是否是二元操作指令
        if (auto* binaryInst = dynamic_cast<BinaryInst*>(instr.get())) {
            // 尝试合并常量
            if (tryMergeConstants(binaryInst)) {
                // 替换指令使用
                replaceInstructionUse(binaryInst->name, binaryInst->name, block);
            }
        }
    }
}

/**
 * @brief 死代码消除
 * 
 * @param block 要优化的基本块
 */
void Optimizer::deadCodeElimination(BasicBlock* block) {
    // 标记要删除的指令
    std::vector<int> toDelete;
    
    // 遍历所有指令
    for (size_t i = 0; i < block->instructions.size(); i++) {
        const auto& instr = block->instructions[i];
        
        // 检查是否是死代码
        if (isDeadCode(instr.get())) {
            toDelete.push_back(i);
        }
    }
    
    // 删除标记的指令
    for (int i = toDelete.size() - 1; i >= 0; i--) {
        block->instructions.erase(block->instructions.begin() + toDelete[i]);
    }
}

/**
 * @brief 表达式简化
 * 
 * @param block 要优化的基本块
 */
void Optimizer::simplifyExpressions(BasicBlock* block) {
    // 遍历所有指令
    for (auto& instr : block->instructions) {
        // 检查是否是二元操作指令
        if (auto* binaryInst = dynamic_cast<BinaryInst*>(instr.get())) {
            // 简化表达式
            // 例如：x + 0 → x
            // x * 1 → x
            // 等等
            // 后续需要实现具体的表达式简化逻辑
        }
    }
}

/**
 * @brief 尝试合并常量
 * 
 * @param instr 二元操作指令
 * @return bool 是否成功合并常量
 */
bool Optimizer::tryMergeConstants(BinaryInst* instr) {
    // 尝试从指令名称中提取常量值
    // 例如：%instr0 = const i32 10
    // 这里简化处理，假设指令名称对应的是常量指令
    // 后续需要实现更复杂的常量分析
    
    // 暂时返回false，后续实现具体的常量合并逻辑
    return false;
}

/**
 * @brief 检查指令是否是死代码
 * 
 * @param instr 要检查的指令
 * @return bool 是否是死代码
 */
bool Optimizer::isDeadCode(const Instruction* instr) {
    // 检查指令是否有名称
    if (instr->name.empty()) {
        return false;
    }
    
    // 检查指令是否是返回指令
    if (dynamic_cast<const RetInst*>(instr)) {
        return false;
    }
    
    // 检查指令是否是分支指令
    if (dynamic_cast<const BrInst*>(instr) || dynamic_cast<const CondBrInst*>(instr)) {
        return false;
    }
    
    // 检查指令是否是存储指令
    if (dynamic_cast<const StoreInst*>(instr)) {
        return false;
    }
    
    // 检查指令是否是函数调用指令
    if (dynamic_cast<const CallInst*>(instr)) {
        return false;
    }
    
    // 检查指令是否是常量指令
    if (dynamic_cast<const ConstInst*>(instr)) {
        return false;
    }
    
    // 检查指令是否是分配指令
    if (dynamic_cast<const AllocaInst*>(instr)) {
        return false;
    }
    
    // 检查指令是否被其他指令使用
    // 暂时简单实现：如果指令有名称，就认为它不是死代码
    // 后续需要实现更复杂的使用分析
    return false;
}

/**
 * @brief 替换指令使用
 * 
 * @param oldName 旧指令名称
 * @param newName 新指令名称
 * @param block 要处理的基本块
 */
void Optimizer::replaceInstructionUse(const std::string& oldName, const std::string& newName, BasicBlock* block) {
    // 遍历所有指令
    for (auto& instr : block->instructions) {
        // 检查是否是二元操作指令
        if (auto* binaryInst = dynamic_cast<BinaryInst*>(instr.get())) {
            if (binaryInst->left == oldName) {
                binaryInst->left = newName;
            }
            if (binaryInst->right == oldName) {
                binaryInst->right = newName;
            }
        }
        
        // 检查是否是一元操作指令
        if (auto* unaryInst = dynamic_cast<UnaryInst*>(instr.get())) {
            if (unaryInst->operand == oldName) {
                unaryInst->operand = newName;
            }
        }
        
        // 检查是否是函数调用指令
        if (auto* callInst = dynamic_cast<CallInst*>(instr.get())) {
            for (auto& arg : callInst->arguments) {
                if (arg == oldName) {
                    arg = newName;
                }
            }
        }
        
        // 检查是否是加载指令
        if (auto* loadInst = dynamic_cast<LoadInst*>(instr.get())) {
            if (loadInst->pointer == oldName) {
                loadInst->pointer = newName;
            }
        }
        
        // 检查是否是存储指令
        if (auto* storeInst = dynamic_cast<StoreInst*>(instr.get())) {
            if (storeInst->value == oldName) {
                storeInst->value = newName;
            }
            if (storeInst->pointer == oldName) {
                storeInst->pointer = newName;
            }
        }
        
        // 检查是否是条件分支指令
        if (auto* condBrInst = dynamic_cast<CondBrInst*>(instr.get())) {
            if (condBrInst->condition == oldName) {
                condBrInst->condition = newName;
            }
        }
    }
}

} // namespace middleend
} // namespace nexus
