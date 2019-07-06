#include <GASolver.hpp>

#include <iostream>
#include <manager.hpp>
#include <fstream>

using namespace dbt;

Population::Population(const AOSParams::GASolverParams &Params, llvm::Module* M, 
    unsigned RegionID, const std::string &BinPath, const std::string &BinArgs,
    const std::string &AOSPath) : convergenceCount(0) {
  
  std::vector<std::unique_ptr<GADNA>> Buffer(Params.PopulationSize);

  Best = nullptr;

  for(unsigned i = 0; i < BEST10_SET.size(); i++) {
    Buffer[i] = std::make_unique<GADNA>(Params.Max, Params.Min, 
        Params.CompileWeight, Params.ExecutionWeight, Params.Times, InitialSearchSpaceType::BEST10);
  }

  for(unsigned i = BEST10_SET.size(); i < Params.PopulationSize; i++) {
    Buffer[i] = std::make_unique<GADNA>(Params.Max, Params.Min,
        Params.CompileWeight, Params.ExecutionWeight, Params.Times, InitialSearchSpaceType::RANDOM);
  }
  
  for(unsigned i = 0; i < Buffer.size(); i++) {
    Buffer[i]->calculateFitness(llvm::CloneModule(*M), RegionID, 
        BinPath, BinArgs, AOSPath);
  }
  
  Chromosomes = std::move(Buffer);
}

void Population::calculateFitness(llvm::Module* M, unsigned RegionID, 
    const std::string& BinPath, const std::string& BinArgs, const std::string& AOSPath) {
  
  for(unsigned i = 0; i < Chromosomes.size(); i++) {
    Chromosomes[i]->calculateFitness(std::move(llvm::CloneModule(*M)), RegionID, 
        BinPath, BinArgs, AOSPath);
  }

}

void Population::calculateProbability() {
  uint64_t Sum = 0, Max = Chromosomes[0]->getFitness();

  for(unsigned i = 1; i < Chromosomes.size(); i++)
    if(Chromosomes[i]->getFitness() > Max)
       Max = Chromosomes[i]->getFitness();

  for(unsigned i = 0; i < Chromosomes.size(); i++)
    Chromosomes[i]->calculateProbability(Max);
  
  normalize();
}

void Population::normalize() {
  uint64_t Sum = 0;

  for(unsigned i = 0; i < Chromosomes.size(); i++)
    Sum += Chromosomes[i]->getProbability();

  for(unsigned i = 0; i < Chromosomes.size(); i++)
    Chromosomes[i]->setProbability(((double)Chromosomes[i]->getProbability())/Sum);
}

unsigned Population::pickOne() {
  double Rate = getRandomRate();
  unsigned Index = 0;
  
  while(Rate > 0 && Index < Chromosomes.size()) {
    Rate -= Chromosomes[Index]->getProbability();
    Index++;
  }

  if(Index > 0)
    Index--;

  return Index;
}

void Population::crossover(double MutationRate) {
  std::vector<std::unique_ptr<GADNA>> NewChromosomes; 
  std::unique_ptr<GADNA> ChildOne, ChildTwo;
  unsigned ParentOne, ParentTwo;

  auto Buffer = std::move(std::unique_ptr<GADNA>(Best->clone()));
  Buffer->mutate(MutationRate);
  NewChromosomes.push_back(std::move(Buffer));
  
  while(NewChromosomes.size() < Chromosomes.size()) {
    ParentOne = pickOne();
    ParentTwo = pickOne();

    ChildOne = std::unique_ptr<GADNA>
      (Chromosomes[ParentOne]->crossover(Chromosomes[ParentTwo].get()));
    ChildTwo = std::unique_ptr<GADNA>
      (Chromosomes[ParentTwo]->crossover(Chromosomes[ParentOne].get()));

    ChildOne->mutate(MutationRate);
    ChildTwo->mutate(MutationRate);
    
    if(NewChromosomes.size() < Chromosomes.size()) {
      NewChromosomes.push_back(std::move(ChildOne));
    }

    if(NewChromosomes.size() < Chromosomes.size()) {
      NewChromosomes.push_back(std::move(ChildTwo));
    }
  }
  
  Chromosomes = std::move(NewChromosomes);
}

void Population::print(unsigned Generation, const std::string &Database,
    const std::string &BinName, const std::string &NOR) {
  std::ofstream myHistoric;
  std::string HistName = Database + BinName + NOR + "H.txt";
  myHistoric.open(HistName, std::ios::app);
  myHistoric << "Generation #" << std::to_string(Generation) << "\n";
  myHistoric << "Best\n";
  myHistoric.close();
  Best->print(Database, BinName, NOR);
  myHistoric.open(HistName, std::ios::app);
  myHistoric << "\nHistoric\n";
  myHistoric.close();
  for(unsigned i = 0; i < Chromosomes.size(); i++) {
    Chromosomes[i]->print(Database, BinName, NOR);
  }
}

std::unique_ptr<GADNA> GASolver::Solve(llvm::Module* M, unsigned RegionID,
    const std::string &Database, const std::string &BinName) {
  Region = M;

  CurrentPopulation = std::make_unique<Population>(Params, 
      M, RegionID, BinPath, BinArgs, AOSPath);
  CurrentPopulation->calculateProbability();
  CurrentPopulation->setBest();
  Evaluate(RegionID, Database, BinName);
    
  auto IRO = llvm::make_unique<IROpt>();
  IRO->optimizeIRFunction(M, CurrentPopulation->getBestTAs());

  return std::move(CurrentPopulation->getBest());
}

void GASolver::Solve(llvm::Module *M, ROIInfo R, unsigned RegionID) {
  if(RegionID == R.RegionID) {
    auto IRO = llvm::make_unique<IROpt>();
    IRO->optimizeIRFunction(M, R.Opts);
  }
}

void GASolver::Evaluate(unsigned RegionID, const std::string &Database, 
    const std::string &BinName) {
  unsigned Generation = 1;

  CurrentPopulation->print(0, Database, BinName, std::to_string(RegionID));

  while(Generation < Params.Generations) {
    CurrentPopulation->crossover(Params.MutationRate);
    CurrentPopulation->calculateFitness(Region, RegionID, BinPath, BinArgs, AOSPath);
    CurrentPopulation->calculateProbability();
    CurrentPopulation->setBest();
    CurrentPopulation->print(Generation, Database, BinName, 
        std::to_string(RegionID));

    Generation++; 
  }
}

void Population::setBest() {
  unsigned Index = 0;

  for(unsigned i = 1; i < Chromosomes.size(); i++) {
    if(Chromosomes[Index]->getFitness() > Chromosomes[i]->getFitness()) {
      Index = i;
    }
  }

  if(!Best) {
    Best = std::unique_ptr<GADNA>(Chromosomes[Index]->clone());
  }else {
    if(Best->getFitness() > Chromosomes[Index]->getFitness()) {
      Best = std::unique_ptr<GADNA>(Chromosomes[Index]->clone());
    }else {
      ++convergenceCount;
    }
  }
}
    
double Population::calculateDiversity() {
  double min = Chromosomes[0]->getProbability(); 
  double max = Chromosomes[0]->getProbability();

  for(unsigned i = 1; i < Chromosomes.size(); i++) {
    if(min > Chromosomes[i]->getProbability())
      min = Chromosomes[i]->getProbability();
    if(max < Chromosomes[i]->getProbability())
      max = Chromosomes[i]->getProbability();
  }
  
  return max - min;
}
