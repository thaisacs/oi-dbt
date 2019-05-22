#include <similarity.hpp>

extern "C" int nw_cmdline(const char*, const char*);

using namespace dbt;

int NeedlemanWunsch::run(const std::string &DNA_A, const std::string &DNA_B) {
  return nw_cmdline(DNA_A.c_str(), DNA_B.c_str());
}
