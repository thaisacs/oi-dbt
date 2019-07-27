#pragma once

#include <AOSICSolver.hpp>
#include <searchSpace.hpp>
#include <DNA.hpp>

namespace dbt {
  class RMHCSolver : public AOSICSolver {
    std::unique_ptr<RMHCDNA> Best;
    std::unique_ptr<RMHCDNA> Current;
    AOSParams::RMHCSolverParams Params;
  public:
    RMHCSolver(const AOSParams::RMHCSolverParams &Params, const std::string &BinPath,
        const std::string &BinArgs, const std::string &AOSPath) : 
      Params(Params), AOSICSolver(BinPath, BinArgs, AOSPath) {}
    
    std::unique_ptr<DNA> Solve(llvm::Module*, unsigned, const std::string&, 
        const std::string&) override;
    void Solve(llvm::Module*, ROIInfo, unsigned) override;
    void Evaluate(unsigned, const std::string&, const std::string&) override;
    void setBest();
  };
}
