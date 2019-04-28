#pragma once

#include <arglib/arglib.hpp>
#include <interpreter.hpp>
#include <RFT.hpp>
#include <manager.hpp>
#include <syscall.hpp>
#include <timer.hpp>
#include <algorithm>
#include <iostream>
#include <memory>
#include <machine.hpp>
#include <string>

namespace dbt {
  class MachineModel {
    std::unique_ptr<dbt::RFT> RftDefault;
    std::unique_ptr<dbt::Machine> TheMachine;
  public:
    MachineModel() {
      TheMachine = std::make_unique<dbt::Machine>();
    }
    double runOnMachine(const std::string&);
  };
}
