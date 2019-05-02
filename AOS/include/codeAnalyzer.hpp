#pragma once

#include <memory>

#include <IRJIT.hpp>
#include <IROpt.hpp>
#include <util.hpp>

#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Bitcode/BitcodeWriter.h"

namespace dbt {
  class CodeAnalyzer {
    unsigned Times;
    std::unique_ptr<IROpt> IRO;
  public:
    CodeAnalyzer(unsigned Times) : Times(Times) { 
      IRO = llvm::make_unique<IROpt>();
    }
    
    uint64_t getCompilationTime(std::unique_ptr<llvm::Module>);
    
    uint64_t getExecutionTime(std::vector<uint16_t>, unsigned, const std::string&,
        const std::string&, const std::string&);
    
    uint64_t getOptTime(llvm::Module*, std::vector<uint16_t>);
  };
}
