#include <SRMHCSolver.hpp>
#include <manager.hpp>

using namespace dbt;

std::unique_ptr<DNA> SRMHCSolver::Solve(llvm::Module* M, unsigned RegionID,
    const std::string &Database, const std::string &BinName) {
  
  if(TAs.size() >= RegionID) {
    TAs[RegionID-1]->mutate();
  }else {
    auto TA = std::make_unique<SRMHCDNA>(Params.Max, Params.Min, InitialSearchSpaceType::RANDOM);
    TAs.push_back(std::move(TA));
  }
    
  if(Params.DumpData)
    TAs[RegionID-1]->print(Database, BinName, std::to_string(RegionID));

  return std::move(std::unique_ptr<SRMHCDNA>(TAs[RegionID-1]->clone()));
}

void SRMHCSolver::Solve(llvm::Module*, ROIInfo, unsigned) {}

void SRMHCSolver::Evaluate(unsigned, const std::string&, const std::string&) {}
