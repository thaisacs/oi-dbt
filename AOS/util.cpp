#include <util.hpp>

int getRandomNumber(int Min, int Max) { 
  return (rand() % Max) + Min; 
}  

double getRandomRate() { 
  double R = getRandomNumber(0, 10);
  return R /= 10;
}
