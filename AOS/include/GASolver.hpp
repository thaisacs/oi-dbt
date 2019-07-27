#pragma once

#include <util.hpp>
#include <AOSICSolver.hpp>
#include <AOSParams.hpp>
#include <AOSPasses.hpp>
#include <DNA.hpp>

#include <vector>
#include <memory>

#include "llvm/IR/Module.h"
#include "llvm/Transforms/Utils/Cloning.h"

namespace dbt {
  struct ROIInfo;

  class Population {
    std::vector<std::unique_ptr<GADNA>> Chromosomes;
    std::unique_ptr<GADNA> Best;
    unsigned convergenceCount;
  public:
    Population(const AOSParams::GASolverParams&, llvm::Module*, unsigned, 
        const std::string&, const std::string&, const std::string&);

    void crossover(double);
    void calculateFitness(llvm::Module*, unsigned, const std::string&, 
        const std::string&, const std::string&);
    void normalize();
    unsigned pickOne();
    void setBest();
    std::unique_ptr<GADNA> getBest() { return std::move(Best); }
    std::vector<uint16_t> getBestTAs() { return Best->getGenes(); }
    unsigned getConvergenceCount() { return convergenceCount; }
    void calculateProbability(); 
    double calculateDiversity();

    std::vector<std::unique_ptr<GADNA>> getChromosomes() {
      return std::move(Chromosomes);
    }

    void print(unsigned, const std::string&, const std::string&, const std::string&);
  };

  class GASolver : public AOSICSolver {
    AOSParams::GASolverParams Params;
    std::unique_ptr<Population> CurrentPopulation;
  public:
    GASolver(const AOSParams::GASolverParams &Params, const std::string &BinPath,
        const std::string &BinArgs, const std::string &AOSPath) : 
      Params(Params), AOSICSolver(BinPath, BinArgs, AOSPath) {}

    std::unique_ptr<DNA> Solve(llvm::Module*, unsigned, 
        const std::string&, const std::string&) override;
    void Solve(llvm::Module*, ROIInfo, unsigned) override;
    
    void Evaluate(unsigned, const std::string&, const std::string&) override;
  };
}
