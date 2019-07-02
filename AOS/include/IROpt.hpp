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
    void populatePassManager(llvm::legacy::PassManager*, 
        llvm::legacy::FunctionPassManager*, std::vector<uint16_t>);
  public:
    IROpt() {}; 

    void optimizeIRFunction(llvm::Module*, std::vector<uint16_t>);
  };
}
