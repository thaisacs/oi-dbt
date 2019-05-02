#include <GASolver.hpp>

#include <iostream>
#include <manager.hpp>

#include <fstream>

using namespace dbt;

Population::Population(const AOSParams::GASolverParams &Params, llvm::Module* M, 
    unsigned RegionID, const std::string &BinPath, const std::string &BinArgs,
    const std::string &AOSPath) {
  
  std::vector<std::unique_ptr<GADNA>> Buffer(Params.PopulationSize);

  for(unsigned i = 0; i < BEST10_SET.size(); i++) {

    Buffer[i] = std::make_unique<GADNA>(Params.Max, 
        Params.CompileWeight, Params.ExecutionWeight, Params.Times, InitialSearchSpaceType::BEST10);

  }

  for(unsigned i = BEST10_SET.size(); i < Params.PopulationSize; i++) {

    Buffer[i] = std::make_unique<GADNA>(Params.Max, 
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
    Chromosomes[i]->setProbability(Chromosomes[i]->getProbability()/Sum);
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
  std::vector<std::unique_ptr<GADNA>> OldChromosomes; 

  std::unique_ptr<GADNA> ChildOne, ChildTwo;

  unsigned ParentOne, ParentTwo;

  OldChromosomes = std::move(Chromosomes);

  while(Chromosomes.size() < OldChromosomes.size()) {
    ParentOne = pickOne();
    ParentTwo = pickOne();

    ChildOne = std::unique_ptr<GADNA>
      (OldChromosomes[ParentOne]->crossover(OldChromosomes[ParentTwo].get()));
    ChildTwo = std::unique_ptr<GADNA>
      (OldChromosomes[ParentTwo]->crossover(OldChromosomes[ParentOne].get()));

    ChildOne->mutate(MutationRate);
    ChildTwo->mutate(MutationRate);

    Chromosomes.push_back(std::move(ChildOne));
    Chromosomes.push_back(std::move(ChildTwo));
  }

  return std::move(OldChromosomes); 
}

void Population::print() {
  std::ofstream myfile;
  myfile.open("historic.txt", std::ios::app);
  myfile << "Population\n";
  myfile.close();
  for(unsigned i = 0; i < Chromosomes.size(); i++) {
    Chromosomes[i]->print();
  }
}

std::vector<std::vector<std::unique_ptr<GADNA>>> GASolver::Solve(llvm::Module* M, unsigned RegionID) {
  Region = M;
  
  CurrentPopulation = nullptr;

  CurrentPopulation = std::make_unique<Population>(Params, 
      M, RegionID, BinPath, BinArgs, AOSPath);
  CurrentPopulation->calculateProbability();
  
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
  
  while(Generation < Params.Generations) {
    auto Buffer = CurrentPopulation->crossover(Params.MutationRate);
    Historic.push_back(std::move(Buffer));
    CurrentPopulation->calculateFitness(Region, RegionID, BinPath, BinArgs, AOSPath);
    CurrentPopulation->calculateProbability();
    CurrentPopulation->print();
    Generation++; 
  }
  
  Historic.push_back(std::move(CurrentPopulation->getChromosomes()));
}
