#pragma once

#include "llvm/Support/YAMLTraits.h"

namespace dbt {
  struct Data {
    std::vector<uint16_t> TAs;
    uint64_t CompilationTime;
    uint64_t ExecutionTime;
    uint64_t Fitness;
  };

  struct RegionData {
    std::string llvmDNA;
    std::string oiDNA;
    Data Best;
  };
}

template <> struct llvm::yaml::MappingTraits<dbt::RegionData> {
  static void mapping(llvm::yaml::IO&, dbt::RegionData&);
};

template <> struct llvm::yaml::MappingTraits<dbt::Data> {
  static void mapping(llvm::yaml::IO&, dbt::Data&);
};

template <> struct llvm::yaml::SequenceTraits<std::vector<dbt::Data>> {
  static size_t size(llvm::yaml::IO&, std::vector<dbt::Data>&);
  static dbt::Data &element(llvm::yaml::IO&, std::vector<dbt::Data>&, size_t);
};
