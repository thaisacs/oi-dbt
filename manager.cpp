#include <manager.hpp>
#include <OIPrinter.hpp>
#include <fstream>
#include <vector>
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "timer.hpp"

using namespace dbt;

void Manager::runPipeline() {
  if (!IRE) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    IRE = llvm::make_unique<IREmitter>();
    IRO = llvm::make_unique<IROpt>();
    IRJIT = new llvm::orc::IRLazyJIT();
  }

  while (isRunning) { 
    uint32_t EntryAddress;
    OIInstList OIRegion;
    spp::sparse_hash_map<uint32_t, uint32_t> BrTargets;

    if (getNumOfOIRegions() > 0) {
      OIRegionsMtx.lock_shared();
      EntryAddress = OIRegions.begin()->first;
      OIRegion     = OIRegions.begin()->second;
      BrTargets    = OIBrTargets[EntryAddress];
      OIRegionsMtx.unlock_shared();
    } 

    if (OIRegion.size() == 0) { 
      continue;
    }

    CompiledOIRegionsMtx.lock();
    CompiledOIRegions[EntryAddress] = OIRegion; 
    CompiledOIRegionsMtx.unlock();
    std::cerr << "Trying to compile: " << std::hex <<  EntryAddress << std::dec << "...";

    OICompiled += OIRegion.size();

    auto Module = IRE->generateRegionIR(EntryAddress, OIRegion, DataMemOffset, BrTargets, IRJIT->getTargetMachine()); 

    //if (EntryAddress == 0x85c)
    // for (auto& F : *Module) 
    //   F.print(llvm::errs());

    std::cout << "---------------------- Printing OIRegion (OpenISA instr.) --------------------" << std::endl;

    for (auto Pair : OIRegion)
      std::cout << std::hex << Pair[0] << ":\t" << dbt::OIPrinter::getString(OIDecoder::decode(Pair[1])) << "\n";

    std::cout << "\n" << std::endl;

    unsigned Size = 1;
    for (auto& F : *Module) 
      for (auto& BB : F)
        Size += BB.size(); 

    //if (EntryAddress == 0x38ec)
    //  for (auto& F : *Module) 
    //    F.print(llvm::errs());

    //Module->print(llvm::errs(), nullptr);    

    IRO->optimizeIRFunction(Module, IROpt::OptLevel::Basic); 

    unsigned OSize = 1;
    for (auto& F : *Module) 
      for (auto& BB : F)
        OSize += BB.size(); 

    //Module->setSourceFileName("r"+std::to_string(EntryAddress));
    Module->print(llvm::errs(), nullptr);

    IRJIT->addModule(std::unique_ptr<llvm::Module>(Module));

    // std::ostringstream buffer;
    // std::string s;

    // buffer << LLVMGetValueName(std::unique_ptr<llvm::Module>(Module)) << ":\n";
    // disassemble(code, buffer);
    // s = buffer.str();
    // std::cerr << s.c_str() << std::endl;

    // dbt::IREmitter::disassemble(NativeRegions[JumpTo], std::cout);
    //dump(Module);

    llvm::errs() << ".. we've compiled (" << (float) OSize/Size << ")\n";
    CompiledRegions += 1;
    LLVMCompiled += OSize;
    AvgOptCodeSize += (float) OSize/Size;

    NativeRegionsMtx.lock();
    
    auto Addr = IRJIT->findSymbol("r"+std::to_string(EntryAddress)).getAddress();

    if (Addr)
      NativeRegions[EntryAddress] = static_cast<intptr_t>(*Addr);
    else 
      std::cerr << EntryAddress << " was not successfully compiled!\n";

    std::cerr << "Disassembly of Region: " << EntryAddress << ":" << std::endl; 
    std::ostringstream buffer;
    size_t t = IREmitter::disassemble((const void*) *Addr, buffer);
    std::cerr << buffer.str().c_str() << std::endl;

    buffer.clear();
    buffer.str("");
    
    std::cerr << "Dumping Region: " << EntryAddress << ":" << std::endl; 
    IREmitter::regionDump((const void*) *Addr, buffer, t);
    std::cerr << buffer.str().c_str() << std::endl;

    // IREmitter::regionDump((void*) *Addr, buffer, OSize, p[0]);   
    // s = buffer.str();
    // std::cerr << s.c_str() << std::endl;

    NativeRegionsMtx.unlock();

    OIRegionsMtx.lock();
    OIRegions.erase(EntryAddress);
    OIRegionsMtx.unlock();
  }
  isFinished = true;
}

void Manager::addOIRegion(uint32_t EntryAddress, OIInstList OIRegion, spp::sparse_hash_map<uint32_t, uint32_t> BrTargets) {
  if (!isRegionEntry(EntryAddress) && OIRegion.size() > 3) {
    OIRegionsMtx.lock();
    OIRegions[EntryAddress]   = OIRegion;
    OIBrTargets[EntryAddress] = BrTargets;
    OIRegionsMtx.unlock();
  }
}

int32_t Manager::jumpToRegion(uint32_t EntryAddress, dbt::Machine& M) {
  uint32_t JumpTo = EntryAddress;

  if(isRegionRecorging)
  {
    RegionAddresses.clear();
    //std::cout << "Recorging Region" << std::endl;
  }

  while (isNativeRegionEntry(JumpTo)) {
    uint32_t LastTo = JumpTo;

    if(isRegionRecorging)
     RegionAddresses.push_back(LastTo);

    M.setOnNativeExecution(JumpTo);
    uint32_t (*FP)(int32_t*, uint32_t*) = (uint32_t (*)(int32_t*, uint32_t*)) NativeRegions[JumpTo];    
    JumpTo = FP(M.getRegisterPtr(), M.getMemoryPtr());

    //std::cout << "Jumping from "  << LastTo << " to " << JumpTo << std::endl;
  }

  //std::cout << "out" << std::endl;
  M.setOffNativeExecution();

  return JumpTo;
}
