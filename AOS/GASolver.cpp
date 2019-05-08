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
    //Buffer[i] = std::make_unique<GADNA>(Params.Max, 
    //    Params.CompileWeight, Params.ExecutionWeight, Params.Times, InitialSearchSpaceType::BEST10);
    //Buffer[i]->mutate(0.25);
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

std::vector<std::unique_ptr<GADNA>> Population::crossover(double MutationRate) {
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
  
  auto OldChromosomes = std::move(Chromosomes);
  Chromosomes = std::move(NewChromosomes);

  return std::move(OldChromosomes); 
}

void Population::print() {
  std::cout << "BEST: ";
  Best->print("", "", "");
  std::cout << std::endl;
  for(unsigned i = 0; i < Chromosomes.size(); i++) {
    Chromosomes[i]->print("", "", "");
  }
}

std::vector<std::vector<std::unique_ptr<GADNA>>> GASolver::Solve(llvm::Module* M, unsigned RegionID) {
  Region = M;
  
  CurrentPopulation = std::make_unique<Population>(Params, 
      M, RegionID, BinPath, BinArgs, AOSPath);
  CurrentPopulation->calculateProbability();
  
  CurrentPopulation->setBest();
  
  Evaluate(RegionID);

  return std::move(Historic);
}

void GASolver::Solve(llvm::Module *M, ROIInfo R, unsigned RegionID) {
  if(RegionID == R.RegionID) {
    auto IRO = llvm::make_unique<IROpt>();
    IRO->optimizeIRFunction(M, R.Opts);
  }
}

void GASolver::Evaluate(unsigned RegionID) {
  unsigned Generation = 1;
    
  CurrentPopulation->print();
  
  while(Generation < Params.Generations) {
    auto Buffer = CurrentPopulation->crossover(Params.MutationRate);
    CurrentPopulation->calculateFitness(Region, RegionID, BinPath, BinArgs, AOSPath);
    CurrentPopulation->calculateProbability();
    CurrentPopulation->setBest();
    CurrentPopulation->print();

    Historic.push_back(std::move(Buffer));
    
    if(CurrentPopulation->getConvergenceCount() >= Params.convergenceThreshold)
      std::cout << "convergence problem...\n";
      //break;
    
    if(CurrentPopulation->calculateDiversity() <= Params.diversityThreshold)
      CurrentPopulation->shake();
    
    Generation++; 
  }
  
  Historic.push_back(std::move(CurrentPopulation->getChromosomes()));
}

void Population::shake() {
  std::cout << "shake...\n";
  for(unsigned i = 1; i < Chromosomes.size(); i++) {
    Chromosomes[i]->mutate(0.016);
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
      convergenceCount = 0;
      std::cout.precision(4);
      std::cout << "FITNESSSSSS " <<std::fixed << Best->getFitness() << std::endl;
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
