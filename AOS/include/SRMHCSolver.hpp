#pragma once

#include <AOSICSolver.hpp>
#include <searchSpace.hpp>
#include <DNA.hpp>

namespace dbt {
  struct ROIInfo;
  class SRMHCSolver : public AOSICSolver {
    AOSParams::SRMHCSolverParams Params;
    std::vector<std::unique_ptr<SRMHCDNA>> TAs;
  public:
    SRMHCSolver(const AOSParams::SRMHCSolverParams &Params, const std::string &BinPath,
        const std::string &BinArgs, const std::string &AOSPath) : Params(Params), 
    AOSICSolver(BinPath, BinArgs, AOSPath) {}
    
    std::unique_ptr<DNA> Solve(llvm::Module*, unsigned, const std::string&, 
        const std::string&) override;
    void Solve(llvm::Module*, ROIInfo, unsigned) override;
    void Evaluate(unsigned, const std::string&, const std::string&) override;
  };
}
