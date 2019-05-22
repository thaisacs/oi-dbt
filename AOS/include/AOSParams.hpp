#pragma once

#include <string>

#include "llvm/Support/YAMLTraits.h"

namespace dbt {
  struct AOSParams {
    bool UpdateDatabase;
    bool CreateDatabase;
    std::string Database;
    bool Training;

    struct SolverParams {
      unsigned Max, Min, Times;

      unsigned Generations;
      double diversityThreshold;
      unsigned convergenceThreshold;
      double MutationRate;
      double CompileWeight;
      double ExecutionWeight;
    };

    struct GASolverParams : public SolverParams {
      unsigned PopulationSize;
    };

    struct RMHCSolverParams : public SolverParams {};

    struct icStrategyType {
      enum ValueType {
        GA, RMHC
      } Value;
      union ParamsType {
        GASolverParams GAParams;
        RMHCSolverParams RMHCParams;
      } Params;
    } icStrategy;

    struct mcStrategyType {
      enum ValueType {
        CBR, DPL, RFL, LTL
      } Value;
      struct ParamsType {
        enum DNAType {
          llvm, oi
        } DNA;
      } Params;
    } mcStrategy;

    enum CharacterizationType {
      DNA, DND, FLL
    } CharacterizationParam;

    enum SimilarityType {
      NaW, CMP
    } SimilarityParam;

    enum RetrievingType {
      ELITE, JUST, NEARLY
    } RetrievingParam;

    bool InvokeIC;
  };
}

template <> struct llvm::yaml::MappingTraits<dbt::AOSParams> {
  static void mapping(llvm::yaml::IO&, dbt::AOSParams&);
};

template <>
struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::icStrategyType::ValueType> {
  static void enumeration(llvm::yaml::IO&, 
      dbt::AOSParams::icStrategyType::ValueType&);
};

template <> struct llvm::yaml::MappingTraits<dbt::AOSParams::GASolverParams> {
  static void mapping(llvm::yaml::IO &io, dbt::AOSParams::GASolverParams&);
};

template <> struct llvm::yaml::MappingTraits<dbt::AOSParams::mcStrategyType::ParamsType> {
  static void mapping(llvm::yaml::IO &io, dbt::AOSParams::mcStrategyType::ParamsType&);
};

template <> struct llvm::yaml::MappingTraits<dbt::AOSParams::RMHCSolverParams> {
  static void mapping(llvm::yaml::IO &, dbt::AOSParams::RMHCSolverParams&);
};

template <>
struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mcStrategyType::ValueType> {
  static void enumeration(llvm::yaml::IO&, 
      dbt::AOSParams::mcStrategyType::ValueType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mcStrategyType::ParamsType::DNAType> {
  static void enumeration(llvm::yaml::IO &, dbt::AOSParams::mcStrategyType::ParamsType::DNAType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::CharacterizationType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::CharacterizationType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::RetrievingType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::RetrievingType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::SimilarityType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::SimilarityType&);
};
