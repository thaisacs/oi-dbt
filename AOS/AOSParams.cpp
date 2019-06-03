#include <AOSParams.hpp>

using namespace dbt;
using namespace llvm::yaml;

void MappingTraits<AOSParams>::mapping(IO &io, AOSParams &Params) {
  io.mapRequired("updateDatabase", Params.UpdateDatabase);
  io.mapRequired("createDatabase", Params.CreateDatabase);
  io.mapRequired("training", Params.Training);
  io.mapRequired("database", Params.Database);
  io.mapRequired("optimization", Params.Optimization);
  
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

  io.mapRequired("characterization", Params.CharacterizationParam);
  io.mapRequired("retrieving", Params.RetrievingParam);
  io.mapRequired("similarity", Params.SimilarityParam);
  io.mapRequired("invokeIC", Params.InvokeIC);
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
  io.mapRequired("times", Params.Times);
  io.mapRequired("populationSize", Params.PopulationSize);
  io.mapRequired("generations", Params.Generations);
  io.mapRequired("compileWeight", Params.CompileWeight);
  io.mapRequired("executionWeight", Params.ExecutionWeight);
  io.mapRequired("diversityThreshold", Params.diversityThreshold);
  io.mapRequired("convergenceThreshold", Params.convergenceThreshold);
}

void MappingTraits<AOSParams::mcStrategyType::ParamsType>::mapping(IO &io, 
    AOSParams::mcStrategyType::ParamsType &Params) {
  io.mapRequired("DNA", Params.DNA);
  io.mapRequired("compileWeight", Params.CompileWeight);
  io.mapRequired("executionWeight", Params.ExecutionWeight);
}

void MappingTraits<AOSParams::RMHCSolverParams>::mapping(IO &io, 
    AOSParams::RMHCSolverParams &Params) {
  io.mapRequired("max", Params.Max);
  io.mapRequired("min", Params.Min);
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

void ScalarEnumerationTraits<AOSParams::mcStrategyType::ParamsType::DNAType>::enumeration(
    IO &io, AOSParams::mcStrategyType::ParamsType::DNAType& Param) {
  io.enumCase(Param, "llvm", 
      AOSParams::mcStrategyType::ParamsType::DNAType::llvm);
  io.enumCase(Param, "oi", 
      AOSParams::mcStrategyType::ParamsType::DNAType::oi);
}

void ScalarEnumerationTraits<AOSParams::CharacterizationType>::enumeration(
    IO &io, AOSParams::CharacterizationType& Param) {
  io.enumCase(Param, "DNA", 
      AOSParams::CharacterizationType::DNA);
  io.enumCase(Param, "DND", 
      AOSParams::CharacterizationType::DND);
  io.enumCase(Param, "FLL", 
      AOSParams::CharacterizationType::FLL);
}

void ScalarEnumerationTraits<AOSParams::RetrievingType>::enumeration(
    IO &io, AOSParams::RetrievingType& Param) {
  io.enumCase(Param, "ELITE", 
      AOSParams::RetrievingType::ELITE);
  io.enumCase(Param, "JUST", 
      AOSParams::RetrievingType::JUST);
  io.enumCase(Param, "NEARLY", 
      AOSParams::RetrievingType::NEARLY);
}

void ScalarEnumerationTraits<AOSParams::OptimizationType>::enumeration(
    IO &io, AOSParams::OptimizationType& Param) {
  io.enumCase(Param, "static", 
      AOSParams::OptimizationType::STATIC);
  io.enumCase(Param, "dynamic", 
      AOSParams::OptimizationType::DYNAMIC);
}

void ScalarEnumerationTraits<AOSParams::SimilarityType>::enumeration(
    IO &io, AOSParams::SimilarityType& Param) {
  io.enumCase(Param, "NaW", 
      AOSParams::SimilarityType::NaW);
  io.enumCase(Param, "CMP", 
      AOSParams::SimilarityType::CMP);
}
