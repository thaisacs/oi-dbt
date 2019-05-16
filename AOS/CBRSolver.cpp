#include "CBRSolver.hpp"

#include <iostream>

extern "C" int nw_cmdline(const char*, const char*);

using namespace dbt;

void CBRSolver::Solve(const std::string &llvmDNA, const std::string &oiDNA) {
  unsigned Index = 0;
  int Max = nw_cmdline(llvmDNA.c_str(), DataSet[0].llvmDNA.c_str());
  for(unsigned i = 1; i < DataSet.size(); i++) {
    int Buffer = nw_cmdline(llvmDNA.c_str(), DataSet[i].llvmDNA.c_str());
    if(Buffer > Max) {
      Max = Buffer;
      Index = i;
    }
  }
  std::cout << "Index " << Index << std::endl;
}
    
void CBRSolver::loadDatabase(const std::string &Database) {
  for(const auto & entry : std::experimental::filesystem::directory_iterator(Database)) {
    RegionData RD;
    auto InputBuffer = llvm::MemoryBuffer::getFile((entry.path()).c_str());
    llvm::yaml::Input yin(InputBuffer->get()->getBuffer());

    yin >> RD;
  
    DataSet.push_back(RD);
  }
}
