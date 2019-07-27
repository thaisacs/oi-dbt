#pragma once

#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/LinkAllPasses.h"

#include <AOSPasses.hpp>
#include <memory>
#include <string>
#include <iostream>

namespace dbt {
  class IROpt {
    std::unique_ptr<llvm::legacy::FunctionPassManager> BasicPM;
    
    void populatePassManager(llvm::legacy::PassManager*, llvm::legacy::FunctionPassManager*, std::vector<uint16_t>);
    void populateFuncPassManager(llvm::legacy::FunctionPassManager*, std::vector<std::string>);
  public:
    IROpt() {}; 

    enum OptLevel { None, Basic, Normal, Aggressive, Adaptative };

    // oi-dbt default
    void optimizeIRFunction(llvm::Module*, OptLevel, uint32_t);
    void customOptimizeIRFunction(llvm::Module*, std::vector<std::string>);

    // aos
    void optimizeIRFunction(llvm::Module*, std::vector<uint16_t>);
  };
}
