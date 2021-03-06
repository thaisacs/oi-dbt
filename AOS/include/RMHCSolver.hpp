#pragma once

#include <AOSICSolver.hpp>
#include <searchSpace.hpp>
#include <DNA.hpp>

namespace dbt {
  struct ROIInfo;
  class RMHCSolver : public AOSICSolver {
    AOSParams::RMHCSolverParams Params;
    std::unique_ptr<RMHCDNA> Best;
    std::unique_ptr<RMHCDNA> Current;
  public:
    RMHCSolver(const AOSParams::RMHCSolverParams &Params, const std::string &BinPath,
        const std::string &BinArgs, const std::string &AOSPath) : Params(Params), 
    AOSICSolver(BinPath, BinArgs, AOSPath) {}
    
    std::unique_ptr<DNA> Solve(llvm::Module*, unsigned, const std::string&, 
        const std::string&) override;
    void Solve(llvm::Module*, ROIInfo, unsigned) override;
    void Evaluate(unsigned, const std::string&, const std::string&) override;
    void setBest();
  };
}
