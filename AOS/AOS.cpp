#include <AOS.hpp>
#include <manager.hpp>

using namespace dbt;

struct less_than_fitness {
  inline bool operator() (const std::unique_ptr<GADNA>& DNA1, const std::unique_ptr<GADNA>& DNA2) {
    return (DNA1->getFitness() < DNA2->getFitness());
  }
};

AOS::AOS(bool ROIMode, const std::string &AOSPath, const std::string &BinPath, const std::string &BinArgs) : NOR(0) {
  auto InputBuffer = llvm::MemoryBuffer::getFile(AOSPath);
  llvm::yaml::Input yin(InputBuffer->get()->getBuffer());

  yin >> Params;

  if(yin.error()) {
    std::cerr << yin.error().message() << std::endl;
    exit(1);
  }

  switch(Params.icStrategy.Value) {
    case AOSParams::icStrategyType::GA:
      ICSolver = std::make_unique<GASolver>(Params.icStrategy.Params.GAParams, 
          BinPath, BinArgs, AOSPath);
      break;
    case AOSParams::icStrategyType::RMHC:
      ICSolver = std::make_unique<RMHCSolver>(Params.icStrategy.Params.RMHCParams, 
          BinPath, BinArgs, AOSPath);
      break;
  }

  switch(Params.mcStrategy.Value) {
    case AOSParams::mcStrategyType::CBR:
      MLSolver = std::make_unique<CBRSolver>(Params.mcStrategy.Params, BinPath, BinArgs, AOSPath);
      break;
    case AOSParams::mcStrategyType::DPL:
      break;
    case AOSParams::mcStrategyType::RFL:
      break;
  }

  switch(Params.mcStrategy.Params.CharacterizationParam) {
    case AOSParams::mcStrategyType::ParamsType::CharacterizationType::DNA:
      CTZ = std::make_unique<DNAEncoding>();
      break;
    case AOSParams::mcStrategyType::ParamsType::CharacterizationType::DND:
      break;
    case AOSParams::mcStrategyType::ParamsType::CharacterizationType::FLL:
      break;
  }

  if(!ROIMode && Params.Strategy == AOSParams::StrategyType::ML)
    MLSolver->loadDatabase(Params.Database);
  else if(!ROIMode && Params.CreateDatabase && Params.Strategy == AOSParams::StrategyType::IC)
    system(("mkdir " + Params.Database).c_str());

  setBinName(BinPath);
  this->BinPath = BinPath;
  this->BinArgs = BinArgs;
  this->AOSPath = AOSPath;
}

void AOS::run(llvm::Module *M, OIInstList OIRegion) {
  NOR++;
  
  if(!Params.SequenceBefore.size())
    baseline(M);  
  
  if(Params.Strategy == AOSParams::StrategyType::IC)
    iterativeCompilation(M, OIRegion);
  else
    machineLearning(M, OIRegion);
}

void AOS::baseline(llvm::Module *M) {
  auto IRO = llvm::make_unique<IROpt>();
  IRO->optimizeIRFunction(M, Params.SequenceBefore);
}

void AOS::machineLearning(llvm::Module *M, OIInstList OIRegion) {
  std::string llvmDNA = CTZ->encode(M);
  std::string oiDNA = CTZ->encode(OIRegion);
  
  if(Params.mcStrategy.Params.Serialized) {
    auto MLData = MLSolver->Solve(M, llvmDNA, oiDNA, NOR);
    
    if(Params.mcStrategy.Params.DumpData) {
      std::ofstream FileResult;
      std::string HistName = BinName + std::to_string(NOR) + "CBR.txt";
      FileResult.open(HistName, std::ios::app);
      for(unsigned i = 0; i < MLData.TAs.size(); i++) {
        FileResult << MLData.TAs[i] << " ";
      }
      FileResult << " - " << " CompilationTime: " << MLData.CompilationTime << 
        " ExecutionTime: " << MLData.ExecutionTime << " Fitness " << MLData.Fitness << std::endl;
      FileResult.close();
    }
  }else {
    MLSolver->Solve(M, llvmDNA, oiDNA);
  }
}

void AOS::iterativeCompilation(llvm::Module *M, OIInstList OIRegion) {
  std::string llvmDNA = CTZ->encode(M);
  std::string oiDNA = CTZ->encode(OIRegion);
  auto ICData = ICSolver->Solve(M, NOR, Params.Database, BinName);
  auto DBData = makeDatabaseData(std::move(ICData), llvmDNA, oiDNA);
  generateDatabase(std::move(DBData));
}

void AOS::run(llvm::Module *M, ROIInfo R) {
  NOR++;
  
  if(!Params.SequenceBefore.size())
    baseline(M);  
  
  ICSolver->Solve(M, R, NOR);
}

std::unique_ptr<RegionData> AOS::makeDatabaseData(std::unique_ptr<DNA> ICData, 
    const std::string& llvmDNA, const std::string& oiDNA) {
  
  auto RD = std::make_unique<RegionData>();
  RD->llvmDNA = llvmDNA;
  RD->oiDNA = oiDNA;

  Data D;   
  D.TAs = std::move((ICData->getGenes()));
  D.CompilationTime = ICData->getCompilationTime();
  D.ExecutionTime = ICData->getExecutionTime();
  D.Fitness = ICData->getFitness();
  RD->BESTs.push_back(D);
  
  return std::move(RD);
}

void AOS::setBinName(const std::string& Path) {
  int Index = -1;

  BinName = "";

  for(unsigned i = 0; i < Path.size(); i++) {
    if(Path[i] == '/')
      Index = i;
  }

  if(Index == -1) {
    BinName += Path;
  }else {
    for(unsigned i = Index + 1; i < Path.size(); i++) {
      BinName.push_back(Path[i]);
    }
  }
}

void AOS::generateDatabase(std::unique_ptr<RegionData> RD) {
  std::ofstream File;
  std::string Text;

  std::string FileName = Params.Database + BinName + std::to_string(NOR) + ".yaml";

  File.open(FileName, std::fstream::app);

  if(!File) {
    std::cerr << "Error while generating the database.\n";
    exit(1);
  }

  llvm::raw_string_ostream Stream(Text);
  llvm::yaml::Output yout(Stream);

  yout << *(RD.get());

  File << Stream.str();

  File.close();
}
