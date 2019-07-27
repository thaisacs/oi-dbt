#pragma once

#include <vector>
#include <DNA.hpp>

#include "llvm/IR/Module.h"

namespace dbt {
  struct ROIInfo;
  class Population;
  class AOSICSolver {
  protected:
    std::string BinPath, BinArgs, AOSPath;
    llvm::Module* Region;
  public:
    AOSICSolver(const std::string& BinPath, const std::string& BinArgs, 
        const std::string& AOSPath) : BinPath(BinPath), BinArgs(BinArgs), AOSPath(AOSPath) {}
    virtual ~AOSICSolver() {}
    
    virtual std::unique_ptr<DNA> Solve(llvm::Module*, unsigned, 
        const std::string&, const std::string&) = 0;
    
    virtual void Solve(llvm::Module*, ROIInfo, unsigned) = 0;
    virtual void Evaluate(unsigned, const std::string&, const std::string&) = 0;
  };
}
