#include "CBRSolver.hpp"

#include <iostream>

using namespace dbt;

Data CBRSolver::Solve(llvm::Module *M, const std::string &llvmDNA, const std::string &oiDNA,
    unsigned RegionID) {
  
  unsigned Index = 0;

  switch(Params.DNA) {
    int Max;
    case AOSParams::mcStrategyType::ParamsType::DNAType::llvm:
      Max = SM->run(llvmDNA, DataSet[Index].llvmDNA);
      for(unsigned i = 1; i < DataSet.size(); i++) {
        int Buffer = SM->run(llvmDNA, DataSet[i].llvmDNA);
        if(Buffer > Max) {
          Max = Buffer;
          Index = i;
        }
      }
      break;
    case AOSParams::mcStrategyType::ParamsType::DNAType::oi:
      Max = SM->run(oiDNA, DataSet[Index].oiDNA);
      for(unsigned i = 1; i < DataSet.size(); i++) {
        int Buffer = SM->run(oiDNA, DataSet[i].oiDNA);
        if(Buffer > Max) {
          Max = Buffer;
          Index = i;
        }
      }
      break;
  }
 
  unsigned BestIndex;
  Data D;   
  D.Fitness;
  for(unsigned i = 0; i < DataSet[Index].BESTs.size(); i++) {
    auto CM = llvm::CloneModule(*M); 
    auto OT = CA->getOptTime(CM.get(), DataSet[Index].BESTs[i].TAs);	
    auto CT = CA->getCompilationTime(std::move(CM));
    auto ET = CA->getExecutionTime(DataSet[Index].BESTs[i].TAs, RegionID, BinPath, BinArgs, AOSPath);
    
    CT += OT;
  
    auto Buffer =  Params.CompileWeight*CT + Params.ExecutionWeight*ET;
    
    if(Buffer < D.Fitness) {
      BestIndex = i;
      D.Fitness = Buffer;
      D.CompilationTime = CT; 
      D.ExecutionTime = ET;
    }
  }

  auto IRO = llvm::make_unique<IROpt>();
  IRO->optimizeIRFunction(M, DataSet[Index].BESTs[BestIndex].TAs);
  
  D.TAs = std::move(DataSet[Index].BESTs[BestIndex].TAs);
  
  return D;
}
    
void CBRSolver::loadDatabase(const std::string &Database) {
  for(const auto & entry : std::experimental::filesystem::directory_iterator(Database)) {
    RegionData RD;
    auto InputBuffer = llvm::MemoryBuffer::getFile((entry.path()).c_str());
    llvm::yaml::Input yin(InputBuffer->get()->getBuffer());

    for(unsigned i = 0; i < 10; i++) {
      Data D;
      RD.BESTs.push_back(D);
    }

    yin >> RD;
 
    DataSet.push_back(RD);
  }
}
