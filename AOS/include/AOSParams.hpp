#pragma once

#include <string>

#include "llvm/Support/YAMLTraits.h"

namespace dbt {
  struct AOSParams {
    //bool UpdateDatabase;
    bool CreateDatabase;
    std::string Database;
    
    enum StrategyType {
      IC, ML
    } Strategy;
    
    std::vector<uint16_t> SequenceBefore;

    struct SolverParams {
      unsigned Max, Min;
      unsigned Generations;
      double CompileWeight;
      double ExecutionWeight;
    };

    struct GASolverParams : public SolverParams {
      double MutationRate;
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
        enum DNATypes {
          llvm, oi
        } DNAType;
        bool Serialized;
        double CompileWeight;
        double ExecutionWeight;
        bool DumpData;
        unsigned DatabaseTAs;
        enum SimilarityType {
          NaW, CMP
        } SimilarityParam;
        enum CharacterizationType {
          DNA, DND, FLL
        } CharacterizationParam;
      } Params;
    } mcStrategy;
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

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mcStrategyType::ParamsType::DNATypes> {
  static void enumeration(llvm::yaml::IO &, dbt::AOSParams::mcStrategyType::ParamsType::DNATypes&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mcStrategyType::ParamsType::CharacterizationType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::mcStrategyType::ParamsType::CharacterizationType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mcStrategyType::ParamsType::SimilarityType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::mcStrategyType::ParamsType::SimilarityType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::StrategyType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::StrategyType&);
};

