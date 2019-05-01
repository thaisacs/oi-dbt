#pragma once

#include <util.hpp>
#include <AOSPasses.hpp>

#include <vector>

namespace dbt {
  enum InitialSearchSpaceType {
    RANDOM, BEST10
  };
  class SearchSpace {
  public:
    static std::vector<uint16_t> generateRandomSpace(unsigned);
    static std::vector<uint16_t> generateBest10Space(unsigned);
  };
}
