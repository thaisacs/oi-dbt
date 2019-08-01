#include <DNA.hpp>

#include <iostream>
#include <fstream>

using namespace dbt;

DNA::DNA(unsigned Size, unsigned Min, double CW, double EW, InitialSearchSpaceType Type) {

  CompilationWeight = CW;
  ExecutionWeight = EW;

  CA = llvm::make_unique<CodeAnalyzer>();

  if(Type == InitialSearchSpaceType::RANDOM) {
    Gene = std::move(SearchSpace::generateRandomSpace(Size, Min));
  }else if(Type == InitialSearchSpaceType::BEST10) {
    Gene = std::move(SearchSpace::generateBest10Space(Size));
  }else if(Type == InitialSearchSpaceType::ZERO) {
    Gene = std::move(SearchSpace::generateZeroSpace(Size));
  }
}

DNA::DNA(double CW, double EW, std::vector<uint16_t> G) {
  CompilationWeight = CW;
  ExecutionWeight = EW;
  
  CA = llvm::make_unique<CodeAnalyzer>();

  Gene = std::move(G);
}

void DNA::calculateFitness(std::unique_ptr<llvm::Module> M, unsigned RegionID, 
    const std::string &BinPath, const std::string &BinArgs, const std::string &AOSPath) {
  uint64_t OptTime;  
  OptTime = CA->getOptTime(M.get(), Gene);	
  CompilationTime = CA->getCompilationTime(std::move(M));
  ExecutionTime = CA->getExecutionTime(Gene, RegionID, BinPath, BinArgs, AOSPath);
  CompilationTime += OptTime;
  Fitness = CompilationWeight*CompilationTime + ExecutionWeight*ExecutionTime;
}

uint16_t DNA::getLocus(unsigned Index) {
  return Gene[Index];
}

std::vector<uint16_t> DNA::getGene() {
  return Gene;
}

double DNA::getFitness() {
  return Fitness;
}

void DNA::setFitness(double F) {
  Fitness = F;
}

void GADNA::print(const std::string& Database, const std::string& BinName, const std::string& NOR) {
  std::ofstream myHistoric;
  std::string HistName = Database + BinName + NOR + "H.txt";
  myHistoric.open(HistName, std::ios::app);
  for(unsigned i = 0; i < Gene.size(); i++) {
    myHistoric << Gene[i] << " ";
  }
  myHistoric << " - " << " CompilationTime: " << CompilationTime << 
    " ExecutionTime: " << ExecutionTime << " Fitness " << Fitness << " Probability " << Probability << std::endl;
  myHistoric.close();
}

double GADNA::getProbability() {
  return Probability;
}

void GADNA::setProbability(double P) {
  Probability = P;
}

void DNA::setCompilationTime(double C) {
  CompilationTime = C;
}

double DNA::getCompilationTime() {
  return CompilationTime;
}

void DNA::setExecutionTime(double E) {
  ExecutionTime = E;
}

double DNA::getExecutionTime() {
  return ExecutionTime;
}

GADNA* GADNA::clone() {
  auto Clone = new GADNA(CompilationWeight, ExecutionWeight, Gene);
  Clone->setFitness(Fitness);
  Clone->setProbability(Probability);
  Clone->setCompilationTime(CompilationTime);
  Clone->setExecutionTime(ExecutionTime);
  return Clone;
}

GADNA* GADNA::crossover(GADNA* Parent) {
  std::vector<uint16_t> ChildGene(Gene.size());

  for(unsigned i = 0; i < Gene.size()/2; i++) {
    ChildGene[i] = Gene[i];
  }

  for(unsigned i = Gene.size()/2; i < Gene.size(); i++) {
    ChildGene[i] = Parent->getLocus(i);
  }

  return new GADNA(CompilationWeight, ExecutionWeight, std::move(ChildGene));
}

void GADNA::mutate(double MutationRate) {
  for(unsigned i = 0; i < Gene.size(); i++) {
    double Random = getRandomRate();
    if(Random <= MutationRate)
      Gene[i] = getRandomNumber(MIN_OPT, MAX_OPT+1);
  }
}

void GADNA::calculateProbability(uint64_t Sum) {
  Probability = Sum - Fitness;
}
    
RMHCDNA* RMHCDNA::clone() {
  auto Clone = new RMHCDNA(CompilationWeight, ExecutionWeight, Gene);
  Clone->setFitness(Fitness);
  Clone->setCompilationTime(CompilationTime);
  Clone->setExecutionTime(ExecutionTime);
  return Clone;
}

void RMHCDNA::mutate() {
  unsigned Opc = getRandomNumber(0, 4);
  unsigned IndexOne = 0, IndexTwo = 0;
  unsigned Buffer;

  switch(Opc) {
    case 0:
      for(unsigned i = Gene.size()-1; i >= 0; i++) {
        if(!Gene[i]) { 
          Gene[i] = getRandomNumber(MIN_OPT, MAX_OPT+1);
          break;
        }
      }
      break;
    case 1:
      for(unsigned i = 0; i < Gene.size(); i++) {
        if(Gene[i] != 0) {
          Gene[i] = 0;
          break;
        }
      }
      break;
    case 2:
      IndexOne = getRandomNumber(0, Gene.size());
      IndexTwo = getRandomNumber(0, Gene.size());
      Buffer = Gene[IndexOne];
      Gene[IndexOne] = Gene[IndexTwo];
      Gene[IndexTwo] = Buffer;
      break;
    case 3:
      IndexOne = getRandomNumber(0, Gene.size());
      Gene[IndexOne] = getRandomNumber(MIN_OPT, MAX_OPT+1);
      break;
    default:
      std::cerr << "RMHC mutate problem!!!\n";
  }
}

void RMHCDNA::print(const std::string& Database, const std::string& BinName, const std::string& NOR) {
  std::ofstream myHistoric;
  std::string HistName = Database + BinName + NOR + "H.txt";
  myHistoric.open(HistName, std::ios::app);
  for(unsigned i = 0; i < Gene.size(); i++) {
    myHistoric << Gene[i] << " ";
  }
  myHistoric << " - " << " CompilationTime: " << CompilationTime << 
    " ExecutionTime: " << ExecutionTime << " Fitness " << Fitness << std::endl;
  myHistoric.close();
}

void SRMHCDNA::mutate() {
  unsigned Opc = getRandomNumber(0, 4);
  unsigned IndexOne = 0, IndexTwo = 0;
  unsigned Buffer;

  switch(Opc) {
    case 0:
      for(unsigned i = Gene.size()-1; i >= 0; i++) {
        if(!Gene[i]) { 
          Gene[i] = getRandomNumber(MIN_OPT, MAX_OPT+1);
          break;
        }
      }
      break;
    case 1:
      for(unsigned i = 0; i < Gene.size(); i++) {
        if(Gene[i] != 0) {
          Gene[i] = 0;
          break;
        }
      }
      break;
    case 2:
      IndexOne = getRandomNumber(0, Gene.size());
      IndexTwo = getRandomNumber(0, Gene.size());
      Buffer = Gene[IndexOne];
      Gene[IndexOne] = Gene[IndexTwo];
      Gene[IndexTwo] = Buffer;
      break;
    case 3:
      IndexOne = getRandomNumber(0, Gene.size());
      Gene[IndexOne] = getRandomNumber(MIN_OPT, MAX_OPT+1);
      break;
    default:
      std::cerr << "SRMHC mutate problem!!!\n";
  }
}

SRMHCDNA* SRMHCDNA::clone() {
  auto Clone = new SRMHCDNA(Gene);
  Clone->setFitness(0);
  Clone->setCompilationTime(0);
  Clone->setExecutionTime(0);
  return Clone;
}

void SRMHCDNA::print(const std::string& Database, const std::string& BinName, const std::string& NOR) {
  std::ofstream myHistoric;
  std::string HistName = Database + BinName + NOR + "H.txt";
  myHistoric.open(HistName, std::ios::app);
  for(unsigned i = 0; i < Gene.size(); i++) {
    myHistoric << Gene[i] << " ";
  }
  myHistoric << std::endl;
  myHistoric.close();
}

RANDOMDNA* RANDOMDNA::clone() {
  auto Clone = new RANDOMDNA(Gene);
  Clone->setFitness(0);
  Clone->setCompilationTime(0);
  Clone->setExecutionTime(0);
  return Clone;
}

void RANDOMDNA::print(const std::string& Database, const std::string& BinName, const std::string& NOR) {
  std::ofstream myHistoric;
  std::string HistName = Database + BinName + NOR + "H.txt";
  myHistoric.open(HistName, std::ios::app);
  for(unsigned i = 0; i < Gene.size(); i++) {
    myHistoric << Gene[i] << " ";
  }
  myHistoric << std::endl;
  myHistoric.close();
}
