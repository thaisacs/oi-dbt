#include <util.hpp>

unsigned getRandomNumber(unsigned Min, unsigned Max) { 
  unsigned i = (rand() % Max) + Min; 
  if(i >= Max)
    return Max - 1;
  else
    return i;
}  

double getRandomRate() { 
  return double(rand()) / (double(RAND_MAX) + 1.0);
}
