#pragma once

#include <iostream>
#include <string>

#include <AOSParams.hpp>
#include <AOSICSolver.hpp>
#include <AOSMLSolver.hpp>
#include <CBRSolver.hpp>
#include <GASolver.hpp>
#include <RMHCSolver.hpp>
#include <SRMHCSolver.hpp>
#include <RANDOMSolver.hpp>
#include <characterization.hpp>
#include <database.hpp>
#include <AOSPasses.hpp>

#include <string>

#include "llvm/IR/Module.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/YAMLTraits.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/Format.h"

#define OIInstList std::vector<std::array<uint32_t,2>>

namespace dbt {
  struct ROIInfo;
  class AOS {
    AOSParams Params;

    std::unique_ptr<AOSICSolver> ICSolver;
    std::unique_ptr<AOSMLSolver> MLSolver;

    std::unique_ptr<Characterization> CTZ;
    
    std::string BinName, BinPath, BinArgs, AOSPath;
    unsigned NOR; //Number Of Regions
  public:
    AOS(bool ROIMode, const std::string&, const std::string&, const std::string&); 

    void run(llvm::Module*, OIInstList);
    void run(llvm::Module*, ROIInfo); 

    std::unique_ptr<RegionData> makeDatabaseData(std::unique_ptr<DNA>, 
        const std::string&, const std::string&);

    void generateDatabase(std::unique_ptr<RegionData>);

    bool isTraining() {
      if(Params.Strategy == AOSParams::StrategyType::IC)
        return true;
      return false;
    }

    bool isCBRSerialized() {
      return Params.mlStrategy.Params.Serialized;
    }
    
    void reset() { NOR = 0; }
  private:
    void setBinName(const std::string&);
    void iterativeCompilation(llvm::Module*, OIInstList);
    void machineLearning(llvm::Module*, OIInstList);
    void baseline(llvm::Module *M); 
  };
}
