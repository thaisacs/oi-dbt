#pragma once

#include <codeAnalyzer.hpp>
#include <AOSParams.hpp>
#include <searchSpace.hpp>
#include <util.hpp>

#include <vector>
#include <iostream>
#include <memory>

namespace dbt {
  class DNA {
  protected:
    std::unique_ptr<CodeAnalyzer> CA;
    std::vector<uint16_t> Genes;
    unsigned Times;
    uint64_t ExecutionTime, CompilationTime, Fitness;
    double CompilationWeight, ExecutionWeight;
  public:
    DNA(double, double, uint16_t, std::vector<uint16_t>);
    
    DNA(unsigned, double, double, uint16_t, InitialSearchSpaceType);

    void calculateFitness(std::unique_ptr<llvm::Module>, unsigned, const std::string&,
        const std::string&, const std::string&);
    
    uint16_t getLocus(unsigned);
    std::vector<uint16_t> getGenes();
    double getFitness();
    void setFitness(double);
    void setCompilationTime(double);
    double getCompilationTime();
    void setExecutionTime(double);
    double getExecutionTime();
  };

  class GADNA : public DNA {
    double Probability;
  public:
    GADNA(double CW, double EW, uint16_t T, std::vector<uint16_t> Genes) : 
      DNA(CW, EW, T, std::move(Genes)) {} 
    
    GADNA(unsigned Size, double CompilationWeight, double ExecutionWeight, 
        uint16_t Times, InitialSearchSpaceType Type) : 
      DNA(Size, CompilationWeight, ExecutionWeight, Times, Type), Probability(0) {}

    void calculateProbability(uint64_t);
    double getProbability();
    void setProbability(double);
    GADNA* crossover(GADNA*);
    void mutate(double);
    GADNA* clone();
    void print();
  };
}
