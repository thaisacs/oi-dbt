#include "CBRSolver.hpp"

#include <iostream>

using namespace dbt;

void CBRSolver::Solve(const std::string &llvmDNA, const std::string &oiDNA) {
  std::cout << DataSet.size() << std::endl;
  //std::cout << "llvm: " << llvmDNA << std::endl;
  //std::cout << "oi: " << oiDNA << std::endl;
  //std::cout << std::endl;
}
    
CBRSolver::CBRSolver() {}

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
