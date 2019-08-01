#pragma once

#include <string>

#include "llvm/Support/YAMLTraits.h"

namespace dbt {
  struct AOSParams {
    bool CreateDatabase;
    std::string Database;
    
    enum StrategyType {
      IC, ML
    } Strategy;
    
    std::vector<uint16_t> SequenceBefore;

    struct SolverParams {
      unsigned Max, Min;
      bool DumpData;
    };

    struct GASolverParams : public SolverParams {
      unsigned Generations;
      double CompileWeight;
      double ExecutionWeight;
      double MutationRate;
      unsigned PopulationSize;
    };

    struct RMHCSolverParams : public SolverParams {
      unsigned Generations;
      double CompileWeight;
      double ExecutionWeight;
    };

    struct SRMHCSolverParams : public SolverParams {};
    struct RANDOMSolverParams : public SolverParams {};
    
    struct icStrategyType {
      enum ValueType {
        GA, RMHC, SRMHC, RANDOM
      } Value;
      union ParamsType {
        GASolverParams GAParams;
        RMHCSolverParams RMHCParams;
        SRMHCSolverParams SRMHCParams;
        RANDOMSolverParams RANDOMParams;
      } Params;
    } icStrategy;

    struct mlStrategyType {
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
    } mlStrategy;
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

template <> struct llvm::yaml::MappingTraits<dbt::AOSParams::mlStrategyType::ParamsType> {
  static void mapping(llvm::yaml::IO &io, dbt::AOSParams::mlStrategyType::ParamsType&);
};

template <> struct llvm::yaml::MappingTraits<dbt::AOSParams::RMHCSolverParams> {
  static void mapping(llvm::yaml::IO &, dbt::AOSParams::RMHCSolverParams&);
};

template <> struct llvm::yaml::MappingTraits<dbt::AOSParams::SRMHCSolverParams> {
  static void mapping(llvm::yaml::IO &, dbt::AOSParams::SRMHCSolverParams&);
};

template <> struct llvm::yaml::MappingTraits<dbt::AOSParams::RANDOMSolverParams> {
  static void mapping(llvm::yaml::IO &, dbt::AOSParams::RANDOMSolverParams&);
};

template <>
struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mlStrategyType::ValueType> {
  static void enumeration(llvm::yaml::IO&, 
      dbt::AOSParams::mlStrategyType::ValueType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mlStrategyType::ParamsType::DNATypes> {
  static void enumeration(llvm::yaml::IO &, dbt::AOSParams::mlStrategyType::ParamsType::DNATypes&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mlStrategyType::ParamsType::CharacterizationType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::mlStrategyType::ParamsType::CharacterizationType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::mlStrategyType::ParamsType::SimilarityType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::mlStrategyType::ParamsType::SimilarityType&);
};

template <> struct llvm::yaml::ScalarEnumerationTraits<dbt::AOSParams::StrategyType> {
  static void enumeration(llvm::yaml::IO &io, dbt::AOSParams::StrategyType&);
};

