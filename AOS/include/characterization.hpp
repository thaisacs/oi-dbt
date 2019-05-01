#pragma once

#include <string>
#include <vector>
#include <OIDecoder.hpp>
#include <OIPrinter.hpp>

#include "llvm/IR/Module.h"

#define OIInstList std::vector<std::array<uint32_t,2>>

namespace dbt {
  class Characterization {
  public:
    Characterization() {}
    virtual std::string encode(llvm::Module*) = 0;
    virtual std::string encode(OIInstList) = 0;
  };

  class DNAEncoding : public Characterization {
  public:
    std::string encode(llvm::Module*) override; 
    std::string encode(OIInstList) override; 
  };
}
