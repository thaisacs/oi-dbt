#include <codeAnalyzer.hpp>
#include <machineModel.hpp>

#include "llvm/Transforms/Utils/Cloning.h"

using namespace dbt;

static inline uint64_t rdtscp() {
  uint64_t rax, rdx;
  uint32_t aux;
  asm volatile ( "rdtscp\n" : "=a" (rax), "=d" (rdx), "=c" (aux) : : );
  return (rdx << 32) + rax;
}

uint64_t CodeAnalyzer::getCompilationTime(std::unique_ptr<llvm::Module> M) {
  auto IRJIT = llvm::make_unique<llvm::orc::IRJIT>(); 
  uint64_t start = rdtscp();
  IRJIT->addModule(std::move(llvm::CloneModule(*M.get())));
  uint64_t end = rdtscp();

  return end - start;
}

uint64_t CodeAnalyzer::getExecutionTime(std::vector<uint16_t> Genes, unsigned RegionID, 
    const std::string &BinPath, const std::string &BinArgs, const std::string &AOSPath) {
  
  auto MM = std::make_unique<MachineModel>();
  auto RegionTime = MM->runOnMachine(Genes, RegionID, AOSPath, BinPath, BinArgs);
  
  return RegionTime;
}

uint64_t CodeAnalyzer::getOptTime(llvm::Module *M, std::vector<uint16_t> Genes) {
  uint64_t start = rdtscp();
  bool Opt = IRO->optimizeIRFunction(M, Genes);
  uint64_t end = rdtscp();

  if(!Opt)
    return 1000000000000000000;

  return end - start;
}
