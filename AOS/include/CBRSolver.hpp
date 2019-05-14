#pragma once

#include <AOSMLSolver.hpp>
#include <experimental/filesystem>
#include <string>
#include <database.hpp>
#include <memory>
#include <vector>

#include "llvm/IR/Module.h"

namespace dbt {
  class CBRSolver : public AOSMLSolver {
    std::vector<RegionData> DataSet;
  public:
    CBRSolver(); 
    void Solve(const std::string&, const std::string&) override;
    void loadDatabase(const std::string&) override;
  };
}
