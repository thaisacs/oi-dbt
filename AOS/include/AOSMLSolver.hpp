#pragma once 

#include <string>

#include "llvm/IR/Module.h"

namespace dbt {
  class AOSMLSolver {
  public:
    AOSMLSolver() {}
    virtual void Solve(const std::string&, const std::string&) = 0;
    virtual void loadDatabase(const std::string&) = 0;
  };
}
