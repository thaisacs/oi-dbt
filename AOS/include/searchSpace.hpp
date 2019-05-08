#pragma once

#include <util.hpp>
#include <AOSPasses.hpp>

#include <vector>

namespace dbt {
  enum InitialSearchSpaceType {
    RANDOM, BEST10, ZERO
  };
  class SearchSpace {
  public:
    static std::vector<uint16_t> generateRandomSpace(unsigned, unsigned);
    static std::vector<uint16_t> generateBest10Space(unsigned);
    static std::vector<uint16_t> generateZeroSpace(unsigned);
  };
}
