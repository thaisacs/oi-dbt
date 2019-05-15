#include <database.hpp>

using namespace dbt;

using llvm::yaml::MappingTraits;
using llvm::yaml::SequenceTraits;
using llvm::yaml::IO;

void MappingTraits<RegionData>::mapping(IO &io, RegionData &D) {
  io.mapRequired("llvmDNA", D.llvmDNA);
  io.mapRequired("oiDNA", D.oiDNA);
  io.mapRequired("best", D.Best);
}

void MappingTraits<Data>::mapping(IO &io, Data &D) {
  io.mapRequired("TAs", D.TAs);
  io.mapRequired("CompilationTime", D.CompilationTime);
  io.mapRequired("ExecutionTime", D.ExecutionTime);
  io.mapRequired("Fitness", D.Fitness);
}

size_t SequenceTraits<std::vector<dbt::Data>>::size(IO &io, 
    std::vector<dbt::Data> &D) {
  return D.size();
}

dbt::Data& SequenceTraits<std::vector<dbt::Data>>::element(IO &io, 
    std::vector<dbt::Data> &List, size_t Index) {
  return List[Index];
}
