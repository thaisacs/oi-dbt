#include <searchSpace.hpp>

using namespace dbt;

std::vector<uint16_t> SearchSpace::generateRandomSpace(unsigned Size) {
  std::vector<uint16_t> Genes(Size);

  for(unsigned i = 0; i < Size; i++) {
    Genes[i] = getRandomNumber(MIN_OPT, MAX_OPT + 1);
  }

  return std::move(Genes);
}

std::vector<uint16_t> SearchSpace::generateBest10Space(unsigned Size) {
  std::vector<uint16_t> Genes(Size);
  static unsigned Index = 0;
  unsigned i;	
  
  for(i = 0; i < BEST10_SET[Index].size(); i++) {
    Genes[i] = BEST10_SET[Index][i];
  }

  while(i < Size) {
    Genes[i] = NONE;
    i++;
  }

  ++Index;

  Index = Index % BEST10_SET.size();

  return std::move(Genes);
}
