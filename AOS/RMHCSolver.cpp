#include <RMHCSolver.hpp>
#include <manager.hpp>

using namespace dbt;

std::unique_ptr<DNA> RMHCSolver::Solve(llvm::Module* M, unsigned RegionID,
    const std::string &Database, const std::string &BinName) {
  Region = M;
    
  Current = std::make_unique<RMHCDNA>(Params.Max, Params.Min, 
      Params.CompileWeight, Params.ExecutionWeight, InitialSearchSpaceType::RANDOM);
  Current->calculateFitness(llvm::CloneModule(*Region), RegionID, BinPath, BinArgs, AOSPath);
  Best = std::unique_ptr<RMHCDNA>(Current->clone());
  
  Evaluate(RegionID, Database, BinName);

  return  std::move(Best);
}

void RMHCSolver::Solve(llvm::Module *M, ROIInfo R, unsigned RegionID) {
  if(RegionID == R.RegionID) {
    auto IRO = llvm::make_unique<IROpt>();
    IRO->optimizeIRFunction(M, R.Opts);
  }
}

void RMHCSolver::Evaluate(unsigned RegionID, const std::string &Database, 
    const std::string &BinName) {
  unsigned Generation = 1;
  
  Current->print(Database, BinName, std::to_string(RegionID));

  while(Generation < Params.Generations) {
    Current->mutate();
    Current->calculateFitness(llvm::CloneModule(*Region), RegionID, BinPath, BinArgs, AOSPath);
    Current->print(Database, BinName, std::to_string(RegionID));
    setBest();
    
    ++Generation;
  }

}

void RMHCSolver::setBest() {
  if(Best->getFitness() > Current->getFitness()) {
    Best = std::unique_ptr<RMHCDNA>(Current->clone());
  }
}
