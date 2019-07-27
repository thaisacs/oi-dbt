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
    uint64_t ExecutionTime, CompilationTime, Fitness;
    double CompilationWeight, ExecutionWeight;
  public:
    DNA(double, double, std::vector<uint16_t>);
    DNA(unsigned, unsigned, double, double, InitialSearchSpaceType);

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
    GADNA(double CW, double EW, std::vector<uint16_t> Genes) : 
      DNA(CW, EW, std::move(Genes)) {} 
    
    GADNA(unsigned Size, unsigned Min, double CompilationWeight, double ExecutionWeight, 
        InitialSearchSpaceType Type) : 
      DNA(Size, Min, CompilationWeight, ExecutionWeight, Type), Probability(0) {}

    double getProbability();
    void setProbability(double);
    GADNA* crossover(GADNA*);
    void mutate(double);
    GADNA* clone();
    void calculateProbability(uint64_t); 
    void print(const std::string&, const std::string&, const std::string&);
  };

  class RMHCDNA : public DNA {
  public:
    RMHCDNA(double CW, double EW, std::vector<uint16_t> Genes) : 
      DNA(CW, EW, std::move(Genes)) {} 
    
    RMHCDNA(unsigned Size, unsigned Min, double CompilationWeight, double ExecutionWeight, 
        InitialSearchSpaceType Type) : 
      DNA(Size, Min, CompilationWeight, ExecutionWeight, Type) {}
    
    RMHCDNA* clone();
    void mutate();
    void print(const std::string&, const std::string&, const std::string&);
  };
}
