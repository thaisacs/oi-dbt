#include <RANDOMSolver.hpp>
#include <manager.hpp>

using namespace dbt;

std::unique_ptr<DNA> RANDOMSolver::Solve(llvm::Module* M, unsigned RegionID,
    const std::string &Database, const std::string &BinName) {
  
  if(TAs.size() < RegionID) {
    auto TA = std::make_unique<RANDOMDNA>(Params.Max, Params.Min, InitialSearchSpaceType::RANDOM);
    TAs.push_back(std::move(TA));
  }
    
  if(Params.DumpData)
    TAs[RegionID-1]->print(Database, BinName, std::to_string(RegionID));

  return std::move(std::unique_ptr<RANDOMDNA>(TAs[RegionID-1]->clone()));
}

void RANDOMSolver::Solve(llvm::Module*, ROIInfo, unsigned) {}

void RANDOMSolver::Evaluate(unsigned, const std::string&, const std::string&) {}
