#pragma once 

#include <string>
#include <similarity.hpp>
#include <memory>
#include <similarity.hpp>
#include <AOSParams.hpp>
#include <codeAnalyzer.hpp>
#include <database.hpp>

#include "llvm/IR/Module.h"

namespace dbt {
  class AOSMLSolver {
  protected:
    std::unique_ptr<Similarity> SM;
    std::unique_ptr<CodeAnalyzer> CA;
    std::string BinPath, BinArgs, AOSPath;
  public:
    AOSMLSolver(const AOSParams::SimilarityType &S, const std::string& BinPath, const std::string& BinArgs, 
        const std::string& AOSPath) : BinPath(BinPath), BinArgs(BinArgs), AOSPath(AOSPath) {
      
      switch(S) {
        case AOSParams::SimilarityType::NaW:
          SM = std::make_unique<NeedlemanWunsch>();
      }
    
      CA = std::make_unique<CodeAnalyzer>();
    }
    virtual Data Solve(llvm::Module *M, const std::string&, const std::string&, unsigned) = 0;
    virtual void Solve(llvm::Module *M, const std::string&, const std::string&) = 0;
    virtual void loadDatabase(const std::string&) = 0;
  };
}
