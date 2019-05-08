#include <AOS.hpp>
#include <manager.hpp>

using namespace dbt;

struct less_than_fitness {
  inline bool operator() (const std::unique_ptr<GADNA>& DNA1, const std::unique_ptr<GADNA>& DNA2) {
    return (DNA1->getFitness() < DNA2->getFitness());
  }
};

AOS::AOS(const std::string &AOSPath, const std::string &BinPath, const std::string &BinArgs) : NOR(0) {

  auto InputBuffer = llvm::MemoryBuffer::getFile(AOSPath);
  llvm::yaml::Input yin(InputBuffer->get()->getBuffer());

  yin >> Params;

  if(yin.error()) {
    std::cerr << yin.error().message() << std::endl;
    exit(1);
  }

  ICSolver = std::make_unique<GASolver>(Params.icStrategy.Params.GAParams, 
      BinPath, BinArgs, AOSPath);

  switch(Params.mcStrategy) {
    case AOSParams::mcStrategyType::CBR:
      //MLSolver = std::make_unique<CBRSolver>();
      break;
    case AOSParams::mcStrategyType::DPL:
      break;
    case AOSParams::mcStrategyType::RFL:
      break;
  }

  switch(Params.CharacterizationParam) {
    case AOSParams::CharacterizationType::DNA:
      CTZ = std::make_unique<DNAEncoding>();
      break;
    case AOSParams::CharacterizationType::DND:
      break;
    case AOSParams::CharacterizationType::FLL:
      break;
  }

  setBinName(BinPath);

  if(Params.Training && Params.CreateDatabase) {
    std::string CMD = "mkdir ";
    CMD += Params.Database;
    system(CMD.c_str());
  }else if(!Params.Training){
    //load database
  }
}

void AOS::run(llvm::Module *M, OIInstList OIRegion) {
  NOR++;

  if(Params.Training)
    iterativeCompilation(M, OIRegion);
  else
    machineLearning(M);
}

void AOS::machineLearning(llvm::Module *M) {}

void AOS::run(llvm::Module *M, ROIInfo R) {
  NOR++;
  ICSolver->Solve(M, R, NOR);
}

void AOS::iterativeCompilation(llvm::Module *M, OIInstList OIRegion) {
  std::string llvmDNA = CTZ->encode(M);
  std::string oiDNA = CTZ->encode(OIRegion);
  
  auto ICData = ICSolver->Solve(M, NOR);
  exit(0);
  auto DBData = makeDatabaseData(std::move(ICData), llvmDNA, oiDNA);
  generateDatabase(std::move(DBData));  
}

std::unique_ptr<RegionData> AOS::makeDatabaseData(std::vector<std::vector<std::unique_ptr<GADNA>>> ICData, 
    const std::string& llvmDNA, const std::string& oiDNA) {

  std::ofstream myHistoric;
  std::string HistName = Params.Database + BinName + std::to_string(NOR) + "H.txt";
  myHistoric.open(HistName, std::ios::app);
  myHistoric.close();
  
  for(unsigned i = 0; i < ICData.size(); i++) {
    std::ofstream myHistoric;
    myHistoric.open(HistName, std::ios::app);
    myHistoric << "Generation #" << std::to_string(i) << std::endl;
    myHistoric.close();
    for(unsigned j = 0; j < ICData[i].size(); j++) {
      ICData[i][j]->print(Params.Database, BinName, std::to_string(NOR));
    } 
  }
  
  auto RD = std::make_unique<RegionData>();
  RD->llvmDNA = llvmDNA;
  RD->oiDNA = oiDNA;

  std::vector<std::unique_ptr<GADNA>> Buffer;

  for(unsigned i = 0; i < ICData.size(); i++) {
    for(unsigned j = 0; j < ICData[i].size(); j++) {
      Buffer.push_back(std::move(ICData[i][j]));
    } 
  }
  
  for(unsigned i = 0; i < ICData.size(); i++) {
    std::sort(Buffer.begin(), Buffer.end(), less_than_fitness());
  }
  
  for(unsigned i = 0; i < 10; i++) {
    Data D;   
    D.TAs = std::move((Buffer[i]->getGenes()));
    D.CompilationTime = Buffer[i]->getCompilationTime();
    D.ExecutionTime = Buffer[i]->getExecutionTime();
    D.Fitness = Buffer[i]->getFitness();
    RD->BESTs.push_back(D);
  }

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

void AOS::loadDatabase() {
  //RegionData RD;

  //for(const auto & entry : fs::directory_iterator(Params.Database)) {
  //  auto InputBuffer = llvm::MemoryBuffer::getFile((entry.path()).c_str());
  //  llvm::yaml::Input yin(InputBuffer->get()->getBuffer());

  //  yin >> RD;

  //  std::cout << RD.DNA << std::endl;
  //  std::cout << RD.Best.TAs.size() << std::endl;
  //}
}
