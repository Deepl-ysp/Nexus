#ifndef NEXUS_OPTIMIZER_H
#define NEXUS_OPTIMIZER_H

#include "ir.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace nexus {
namespace middleend {

class Optimizer {
public:
    Optimizer();
    
    // 优化模块
    std::unique_ptr<Module> optimize(std::unique_ptr<Module> module);
    
    // 获取优化后的模块
    const std::unique_ptr<Module>& getModule() const;
    
private:
    // 模块
    std::unique_ptr<Module> module;
    
    // 优化函数
    void optimizeFunction(Function* func);
    
    // 优化基本块
    void optimizeBlock(BasicBlock* block);
    
    // 常量折叠
    void constantFolding(BasicBlock* block);
    
    // 死代码消除
    void deadCodeElimination(BasicBlock* block);
    
    // 表达式简化
    void simplifyExpressions(BasicBlock* block);
    
    // 合并常量
    bool tryMergeConstants(BinaryInst* instr);
    
    // 检查指令是否是死代码
    bool isDeadCode(const Instruction* instr);
    
    // 替换指令使用
    void replaceInstructionUse(const std::string& oldName, const std::string& newName, BasicBlock* block);
};

} // namespace middleend
} // namespace nexus

#endif // NEXUS_OPTIMIZER_H
