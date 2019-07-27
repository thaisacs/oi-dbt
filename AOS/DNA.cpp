#include <DNA.hpp>

#include <iostream>
#include <fstream>

using namespace dbt;

DNA::DNA(unsigned Size, unsigned Min, double CW, double EW, InitialSearchSpaceType Type) {

  CompilationWeight = CW;
  ExecutionWeight = EW;

  CA = llvm::make_unique<CodeAnalyzer>();

  if(Type == InitialSearchSpaceType::RANDOM) {
    Genes = std::move(SearchSpace::generateRandomSpace(Size, Min));
  }else if(Type == InitialSearchSpaceType::BEST10) {
    Genes = std::move(SearchSpace::generateBest10Space(Size));
  }else if(Type == InitialSearchSpaceType::ZERO) {
    Genes = std::move(SearchSpace::generateZeroSpace(Size));
  }
}

DNA::DNA(double CW, double EW, std::vector<uint16_t> G) {
  CompilationWeight = CW;
  ExecutionWeight = EW;

  CA = llvm::make_unique<CodeAnalyzer>();

  Genes = std::move(G);
}

void DNA::calculateFitness(std::unique_ptr<llvm::Module> M, unsigned RegionID, 
    const std::string &BinPath, const std::string &BinArgs, const std::string &AOSPath) {
  uint64_t OptTime;  
  OptTime = CA->getOptTime(M.get(), Genes);	
  CompilationTime = CA->getCompilationTime(std::move(M));
  ExecutionTime = CA->getExecutionTime(Genes, RegionID, BinPath, BinArgs, AOSPath);
  CompilationTime += OptTime;
  Fitness = CompilationWeight*CompilationTime + ExecutionWeight*ExecutionTime;
}

uint16_t DNA::getLocus(unsigned Index) {
  return Genes[Index];
}

std::vector<uint16_t> DNA::getGenes() {
  return Genes;
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
  for(unsigned i = 0; i < Genes.size(); i++) {
    myHistoric << Genes[i] << " ";
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
  auto Clone = new GADNA(CompilationWeight, ExecutionWeight, Genes);
  Clone->setFitness(Fitness);
  Clone->setProbability(Probability);
  Clone->setCompilationTime(CompilationTime);
  Clone->setExecutionTime(ExecutionTime);
  return Clone;
}

GADNA* GADNA::crossover(GADNA* Parent) {
  std::vector<uint16_t> ChildGenes(Genes.size());

  for(unsigned i = 0; i < Genes.size()/2; i++) {
    ChildGenes[i] = Genes[i];
  }

  for(unsigned i = Genes.size()/2; i < Genes.size(); i++) {
    ChildGenes[i] = Parent->getLocus(i);
  }

  return new GADNA(CompilationWeight, ExecutionWeight, std::move(ChildGenes));
}

void GADNA::mutate(double MutationRate) {
  for(unsigned i = 0; i < Genes.size(); i++) {
    double Random = getRandomRate();
    if(Random <= MutationRate)
      Genes[i] = getRandomNumber(MIN_OPT, MAX_OPT+1);
  }
}

void GADNA::calculateProbability(uint64_t Sum) {
  Probability = Sum - Fitness;
}
    
RMHCDNA* RMHCDNA::clone() {
  auto Clone = new RMHCDNA(CompilationWeight, ExecutionWeight, Genes);
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
      for(unsigned i = Genes.size()-1; i >= 0; i++) {
        if(!Genes[i]) { 
          Genes[i] = getRandomNumber(MIN_OPT, MAX_OPT+1);
          break;
        }
      }
      break;
    case 1:
      for(unsigned i = 0; i < Genes.size(); i++) {
        if(Genes[i] != 0) {
          Genes[i] = 0;
          break;
        }
      }
      break;
    case 2:
      IndexOne = getRandomNumber(0, Genes.size());
      IndexTwo = getRandomNumber(0, Genes.size());
      Buffer = Genes[IndexOne];
      Genes[IndexOne] = Genes[IndexTwo];
      Genes[IndexTwo] = Buffer;
      break;
    case 3:
      IndexOne = getRandomNumber(0, Genes.size());
      Genes[IndexOne] = getRandomNumber(MIN_OPT, MAX_OPT+1);
      break;
    default:
      std::cerr << "RMHC mutate problem!!!\n";
  }
}

void RMHCDNA::print(const std::string& Database, const std::string& BinName, const std::string& NOR) {
  std::ofstream myHistoric;
  std::string HistName = Database + BinName + NOR + "H.txt";
  myHistoric.open(HistName, std::ios::app);
  for(unsigned i = 0; i < Genes.size(); i++) {
    myHistoric << Genes[i] << " ";
  }
  myHistoric << " - " << " CompilationTime: " << CompilationTime << 
    " ExecutionTime: " << ExecutionTime << " Fitness " << Fitness << std::endl;
  myHistoric.close();
}
