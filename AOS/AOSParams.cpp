#include <AOSParams.hpp>

using namespace dbt;
using namespace llvm::yaml;

void MappingTraits<AOSParams>::mapping(IO &io, AOSParams &Params) {
  io.mapRequired("createDatabase", Params.CreateDatabase);
  io.mapRequired("database", Params.Database);
  io.mapRequired("sequenceBefore", Params.SequenceBefore);
  io.mapRequired("strategy", Params.Strategy);
  io.mapRequired("icStrategy", Params.icStrategy.Value);

  switch(Params.icStrategy.Value) {
    case AOSParams::icStrategyType::ValueType::GA:
      io.mapRequired("icStrategyParams", Params.icStrategy.Params.GAParams);
      break;
    case AOSParams::icStrategyType::ValueType::RMHC:
      io.mapRequired("icStrategyParams", Params.icStrategy.Params.RMHCParams);
  }

  io.mapRequired("mcStrategy", Params.mcStrategy.Value);
  io.mapRequired("mcStrategyParams", Params.mcStrategy.Params);
}

void ScalarEnumerationTraits<AOSParams::icStrategyType::ValueType>::enumeration(
    IO &io, AOSParams::icStrategyType::ValueType &Strategy) {
  io.enumCase(Strategy, "GA", AOSParams::icStrategyType::ValueType::GA);
  io.enumCase(Strategy, "RMHC", AOSParams::icStrategyType::ValueType::RMHC);
}

void MappingTraits<AOSParams::GASolverParams>::mapping(IO &io, 
    AOSParams::GASolverParams &Params) {
  io.mapRequired("max", Params.Max);
  io.mapRequired("min", Params.Min);
  io.mapRequired("mutationRate", Params.MutationRate);
  io.mapRequired("populationSize", Params.PopulationSize);
  io.mapRequired("generations", Params.Generations);
  io.mapRequired("compileWeight", Params.CompileWeight);
  io.mapRequired("executionWeight", Params.ExecutionWeight);
}

void MappingTraits<AOSParams::mcStrategyType::ParamsType>::mapping(IO &io, 
    AOSParams::mcStrategyType::ParamsType &Params) {
  io.mapRequired("DNA", Params.DNAType);
  io.mapRequired("serialized", Params.Serialized);
  io.mapRequired("dumpData", Params.DumpData);
  io.mapRequired("compileWeight", Params.CompileWeight);
  io.mapRequired("executionWeight", Params.ExecutionWeight);
  io.mapRequired("compileWeight", Params.CompileWeight);
  io.mapRequired("executionWeight", Params.ExecutionWeight);
  io.mapRequired("characterization", Params.CharacterizationParam);
  io.mapRequired("similarity", Params.SimilarityParam);
  io.mapRequired("databaseTAs", Params.DatabaseTAs);
}

void MappingTraits<AOSParams::RMHCSolverParams>::mapping(IO &io, 
    AOSParams::RMHCSolverParams &Params) {
  io.mapRequired("max", Params.Max);
  io.mapRequired("min", Params.Min);
  io.mapRequired("compileWeight", Params.CompileWeight);
  io.mapRequired("executionWeight", Params.ExecutionWeight);
  io.mapRequired("generations", Params.Generations);
}

void ScalarEnumerationTraits<AOSParams::mcStrategyType::ValueType>::enumeration(
    IO &io, AOSParams::mcStrategyType::ValueType& Param) {
  io.enumCase(Param, "CBR", 
      AOSParams::mcStrategyType::CBR);
  io.enumCase(Param, "DPL", 
      AOSParams::mcStrategyType::DPL);
  io.enumCase(Param, "RFL", 
      AOSParams::mcStrategyType::RFL);
  io.enumCase(Param, "LTL", 
      AOSParams::mcStrategyType::LTL);
}

void ScalarEnumerationTraits<AOSParams::mcStrategyType::ParamsType::DNATypes>::enumeration(
    IO &io, AOSParams::mcStrategyType::ParamsType::DNATypes& Param) {
  io.enumCase(Param, "llvm", 
      AOSParams::mcStrategyType::ParamsType::DNATypes::llvm);
  io.enumCase(Param, "oi", 
      AOSParams::mcStrategyType::ParamsType::DNATypes::oi);
}

void ScalarEnumerationTraits<AOSParams::mcStrategyType::ParamsType::CharacterizationType>::enumeration(
    IO &io, AOSParams::mcStrategyType::ParamsType::CharacterizationType& Param) {
  io.enumCase(Param, "DNA", 
      AOSParams::mcStrategyType::ParamsType::CharacterizationType::DNA);
  io.enumCase(Param, "DND", 
      AOSParams::mcStrategyType::ParamsType::CharacterizationType::DND);
  io.enumCase(Param, "FLL", 
      AOSParams::mcStrategyType::ParamsType::CharacterizationType::FLL);
}

void ScalarEnumerationTraits<AOSParams::StrategyType>::enumeration(
    IO &io, AOSParams::StrategyType& Param) {
  io.enumCase(Param, "IC", 
      AOSParams::StrategyType::IC);
  io.enumCase(Param, "ML", 
      AOSParams::StrategyType::ML);
}

void ScalarEnumerationTraits<AOSParams::mcStrategyType::ParamsType::SimilarityType>::enumeration(
    IO &io, AOSParams::mcStrategyType::ParamsType::SimilarityType& Param) {
  io.enumCase(Param, "NaW", 
      AOSParams::mcStrategyType::ParamsType::SimilarityType::NaW);
  io.enumCase(Param, "CMP", 
      AOSParams::mcStrategyType::ParamsType::SimilarityType::CMP);
}
