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

    std::vector<std::unique_ptr<GADNA>> crossover(double);
    void calculateFitness(llvm::Module*, unsigned, const std::string&, 
        const std::string&, const std::string&);
    void normalize();
    unsigned pickOne();
    void setBest();
    unsigned getConvergenceCount() { return convergenceCount; }
    void calculateProbability(); 
    double calculateDiversity();
    void shake();

    std::vector<std::unique_ptr<GADNA>> getChromosomes() {
      return std::move(Chromosomes);
    }

    void print();
  };

  class GASolver : public AOSICSolver {
    AOSParams::GASolverParams Params;
    std::string BinPath, BinArgs, AOSPath;
    std::unique_ptr<Population> CurrentPopulation;
    std::vector<std::vector<std::unique_ptr<GADNA>>> Historic;
    llvm::Module* Region;
  public:
    GASolver(const AOSParams::GASolverParams &Params, const std::string &BinPath,
        const std::string &BinArgs, const std::string &AOSPath) : 
      AOSICSolver(), Params(Params), BinPath(BinPath), BinArgs(BinArgs), AOSPath(AOSPath) {}

    std::vector<std::vector<std::unique_ptr<GADNA>>> Solve(llvm::Module*, unsigned) override;

    void Solve(llvm::Module*, ROIInfo, unsigned) override;
    
    void Evaluate(unsigned) override;
  };
}