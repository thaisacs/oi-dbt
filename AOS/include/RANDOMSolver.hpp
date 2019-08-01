#pragma once

#include <AOSICSolver.hpp>
#include <searchSpace.hpp>
#include <DNA.hpp>

namespace dbt {
  struct ROIInfo;
  class RANDOMSolver : public AOSICSolver {
    AOSParams::RANDOMSolverParams Params;
    std::vector<std::unique_ptr<RANDOMDNA>> TAs;
  public:
    RANDOMSolver(const AOSParams::RANDOMSolverParams &Params, const std::string &BinPath,
        const std::string &BinArgs, const std::string &AOSPath) : Params(Params), 
    AOSICSolver(BinPath, BinArgs, AOSPath) {}
    
    std::unique_ptr<DNA> Solve(llvm::Module*, unsigned, const std::string&, 
        const std::string&) override;
    void Solve(llvm::Module*, ROIInfo, unsigned) override;
    void Evaluate(unsigned, const std::string&, const std::string&) override;
  };
}
