#ifndef NEXUS_BACKEND_CODEGEN_H
#define NEXUS_BACKEND_CODEGEN_H

#include "../middleend/ir.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace nexus {
namespace backend {

class CodeGenerator {
public:
    CodeGenerator();
    
    // 生成目标代码
    std::string generate(const middleend::Module& module);
    
    // 获取生成的代码
    const std::string& getCode() const;
    
private:
    // 生成的代码
    std::string code;
    
    // 函数名称映射
    std::map<std::string, std::string> functionMap;
    
    // 变量名称映射
    std::map<std::string, std::string> variableMap;
    
    // 指令计数器
    int instrCount;
    
    // 生成指令名称
    std::string generateInstrName();
    
    // 生成函数代码
    void generateFunction(const middleend::Function* func);
    
    // 生成基本块代码
    void generateBlock(const middleend::BasicBlock* block);
    
    // 生成指令代码
    void generateInstruction(const middleend::Instruction* instr);
    
    // 生成常量指令代码
    void generateConstInst(const middleend::ConstInst* instr);
    
    // 生成二元操作指令代码
    void generateBinaryInst(const middleend::BinaryInst* instr);
    
    // 生成一元操作指令代码
    void generateUnaryInst(const middleend::UnaryInst* instr);
    
    // 生成条件分支指令代码
    void generateCondBrInst(const middleend::CondBrInst* instr);
    
    // 生成无条件分支指令代码
    void generateBrInst(const middleend::BrInst* instr);
    
    // 生成函数调用指令代码
    void generateCallInst(const middleend::CallInst* instr);
    
    // 生成返回指令代码
    void generateRetInst(const middleend::RetInst* instr);
    
    // 生成栈分配指令代码
    void generateAllocaInst(const middleend::AllocaInst* instr);
    
    // 生成加载指令代码
    void generateLoadInst(const middleend::LoadInst* instr);
    
    // 生成存储指令代码
    void generateStoreInst(const middleend::StoreInst* instr);
    
    // 生成 PHI 节点指令代码
    void generatePhiInst(const middleend::PhiInst* instr);
    
    // 生成类型转换
    std::string getTypeCode(middleend::Type type);
};

} // namespace backend
} // namespace nexus

#endif // NEXUS_BACKEND_CODEGEN_H
