#include <DNA.hpp>

#include <iostream>
#include <fstream>

using namespace dbt;

DNA::DNA(unsigned Size, unsigned Min, double CW, double EW, uint16_t T, InitialSearchSpaceType Type) {

  CompilationWeight = CW;
  ExecutionWeight = EW;
  Times = T;

  CA = llvm::make_unique<CodeAnalyzer>(T);

  if(Type == InitialSearchSpaceType::RANDOM) {
    Genes = std::move(SearchSpace::generateRandomSpace(Size, Min));
  }else if(Type == InitialSearchSpaceType::BEST10) {
    Genes = std::move(SearchSpace::generateBest10Space(Size));
  }else if(Type == InitialSearchSpaceType::ZERO) {
    Genes = std::move(SearchSpace::generateZeroSpace(Size));
  }
}

DNA::DNA(double CW, double EW, uint16_t T, std::vector<uint16_t> G) {
  CompilationWeight = CW;
  ExecutionWeight = EW;
  Times = T;

  CA = llvm::make_unique<CodeAnalyzer>(T);

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
  return std::move(Genes);
}

double DNA::getFitness() {
  return Fitness;
}

void DNA::setFitness(double F) {
  Fitness = F;
}

void GADNA::print(const std::string& Database, const std::string& BinName, const std::string& NOR) {
  for(unsigned i = 0; i < Genes.size(); i++) {
    std::cout << Genes[i] << " ";
  }
  std::cout.precision(4);
  std::cout << "- " << std::fixed << Probability << std::endl;
  std::cout << " " << Fitness << " " << " " << CompilationTime << " " << ExecutionTime << std::endl;
  //std::ofstream myHistoric;
  //std::string HistName = Database + BinName + NOR + "H.txt";
  //myHistoric.open(HistName, std::ios::app);
  //for(unsigned i = 0; i < Genes.size(); i++) {
  //  myHistoric << Genes[i] << " ";
  //}
  //myHistoric << " - " << " CompilationTime: " << CompilationTime << 
  //  " ExecutionTime: " << ExecutionTime << " Fitness " << Fitness << " Probability " << Probability << std::endl;
  //myHistoric.close();
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
  auto Clone = new GADNA(CompilationWeight, ExecutionWeight, Times, Genes);
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

  return new GADNA(CompilationWeight, ExecutionWeight, Times, std::move(ChildGenes));
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
