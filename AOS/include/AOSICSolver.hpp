#pragma once

#include <vector>
#include <DNA.hpp>

#include "llvm/IR/Module.h"

namespace dbt {
  struct ROIInfo;
  class Population;
  class AOSICSolver {
  public:
    AOSICSolver() {}
    virtual ~AOSICSolver() {}
    
    virtual std::unique_ptr<GADNA> Solve(llvm::Module*, unsigned, 
        const std::string&, const std::string&) = 0;
    
    virtual void Solve(llvm::Module*, ROIInfo, unsigned) = 0;
    
    virtual void Evaluate(unsigned, const std::string&, const std::string&) = 0;
  };
}
