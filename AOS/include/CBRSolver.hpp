#pragma once

#include <AOSMLSolver.hpp>
#include <experimental/filesystem>
#include <string>
#include <database.hpp>
#include <memory>
#include <vector>
#include <similarity.hpp>
#include <AOSParams.hpp>

#include "llvm/IR/Module.h"
#include "llvm/Transforms/Utils/Cloning.h"

namespace dbt {
  class CBRSolver : public AOSMLSolver {
    std::vector<RegionData> DataSet;
    AOSParams::mcStrategyType::ParamsType Params;
  public:
    CBRSolver(const AOSParams::mcStrategyType::ParamsType &Params, const AOSParams::SimilarityType &S,
        const std::string& BinPath, const std::string& BinArgs, const std::string& AOSPath): 
      Params(Params), AOSMLSolver(S, BinPath, BinArgs, AOSPath) {
    }
    void Solve(llvm::Module *M, const std::string&, const std::string&, unsigned) override;
    void loadDatabase(const std::string&) override;
  };
}
