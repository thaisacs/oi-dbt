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
    case AOSParams::icStrategyType::ValueType::SRMHC:
      io.mapRequired("icStrategyParams", Params.icStrategy.Params.SRMHCParams);
    case AOSParams::icStrategyType::ValueType::RANDOM:
      io.mapRequired("icStrategyParams", Params.icStrategy.Params.RANDOMParams);
  }

  io.mapRequired("mlStrategy", Params.mlStrategy.Value);
  io.mapRequired("mlStrategyParams", Params.mlStrategy.Params);
}

void ScalarEnumerationTraits<AOSParams::icStrategyType::ValueType>::enumeration(
    IO &io, AOSParams::icStrategyType::ValueType &Strategy) {
  io.enumCase(Strategy, "GA", AOSParams::icStrategyType::ValueType::GA);
  io.enumCase(Strategy, "RMHC", AOSParams::icStrategyType::ValueType::RMHC);
  io.enumCase(Strategy, "SRMHC", AOSParams::icStrategyType::ValueType::SRMHC);
  io.enumCase(Strategy, "RANDOM", AOSParams::icStrategyType::ValueType::RANDOM);
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

void MappingTraits<AOSParams::mlStrategyType::ParamsType>::mapping(IO &io, 
    AOSParams::mlStrategyType::ParamsType &Params) {
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

void MappingTraits<AOSParams::SRMHCSolverParams>::mapping(IO &io, 
    AOSParams::SRMHCSolverParams &Params) {
  io.mapRequired("max", Params.Max);
  io.mapRequired("min", Params.Min);
  io.mapRequired("dumpData", Params.DumpData);
}

void MappingTraits<AOSParams::RANDOMSolverParams>::mapping(IO &io, 
    AOSParams::RANDOMSolverParams &Params) {
  io.mapRequired("max", Params.Max);
  io.mapRequired("min", Params.Min);
  io.mapRequired("dumpData", Params.DumpData);
}

void ScalarEnumerationTraits<AOSParams::mlStrategyType::ValueType>::enumeration(
    IO &io, AOSParams::mlStrategyType::ValueType& Param) {
  io.enumCase(Param, "CBR", 
      AOSParams::mlStrategyType::CBR);
  io.enumCase(Param, "DPL", 
      AOSParams::mlStrategyType::DPL);
  io.enumCase(Param, "RFL", 
      AOSParams::mlStrategyType::RFL);
  io.enumCase(Param, "LTL", 
      AOSParams::mlStrategyType::LTL);
}

void ScalarEnumerationTraits<AOSParams::mlStrategyType::ParamsType::DNATypes>::enumeration(
    IO &io, AOSParams::mlStrategyType::ParamsType::DNATypes& Param) {
  io.enumCase(Param, "llvm", 
      AOSParams::mlStrategyType::ParamsType::DNATypes::llvm);
  io.enumCase(Param, "oi", 
      AOSParams::mlStrategyType::ParamsType::DNATypes::oi);
}

void ScalarEnumerationTraits<AOSParams::mlStrategyType::ParamsType::CharacterizationType>::enumeration(
    IO &io, AOSParams::mlStrategyType::ParamsType::CharacterizationType& Param) {
  io.enumCase(Param, "DNA", 
      AOSParams::mlStrategyType::ParamsType::CharacterizationType::DNA);
  io.enumCase(Param, "DND", 
      AOSParams::mlStrategyType::ParamsType::CharacterizationType::DND);
  io.enumCase(Param, "FLL", 
      AOSParams::mlStrategyType::ParamsType::CharacterizationType::FLL);
}

void ScalarEnumerationTraits<AOSParams::StrategyType>::enumeration(
    IO &io, AOSParams::StrategyType& Param) {
  io.enumCase(Param, "IC", 
      AOSParams::StrategyType::IC);
  io.enumCase(Param, "ML", 
      AOSParams::StrategyType::ML);
}

void ScalarEnumerationTraits<AOSParams::mlStrategyType::ParamsType::SimilarityType>::enumeration(
    IO &io, AOSParams::mlStrategyType::ParamsType::SimilarityType& Param) {
  io.enumCase(Param, "NaW", 
      AOSParams::mlStrategyType::ParamsType::SimilarityType::NaW);
  io.enumCase(Param, "CMP", 
      AOSParams::mlStrategyType::ParamsType::SimilarityType::CMP);
}
