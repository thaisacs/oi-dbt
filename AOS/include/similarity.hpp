#pragma once

#include <string>

namespace dbt {
  class Similarity {
  public:
    Similarity() {}
    virtual int run(const std::string&, const std::string&) = 0;
  };
  
  class NeedlemanWunsch : public Similarity {
  public:
    NeedlemanWunsch() {}
    int run(const std::string&, const std::string&) override;
  };
}
