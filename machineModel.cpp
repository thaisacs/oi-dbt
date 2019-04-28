#include <machineModel.hpp>

double dbt::MachineModel::runOnMachine(const std::string &BinPath) {
  int LoadStatus = TheMachine->loadELF(BinPath);

  if (!LoadStatus) {
    std::cerr << "Can't find or process ELF file " << std::endl;
    return 2;
  }

  dbt::Manager TheManager(TheMachine->getDataMemOffset(), *(TheMachine.get()), false, false);
  TheManager.setOptPolicy(dbt::Manager::OptPolitic::Normal);
  TheManager.startCompilationThr();
  std::unique_ptr<dbt::SyscallManager> SyscallM;

  dbt::Timer GlobalTimer;

  LoadStatus = TheMachine->loadELF(BinPath);

  if (!LoadStatus) {
    std::cerr << "Can't find or process ELF file " << std::endl;
    return 2;
  }

  RftDefault = std::make_unique<dbt::NET>(TheManager);

  SyscallM = std::make_unique<dbt::LinuxSyscallManager>();

  GlobalTimer.startClock();
  dbt::ITDInterpreter I(*SyscallM.get(), *RftDefault.get());
  std::cerr << "Starting execution:\n";
  
  I.executeAll((*TheMachine.get()));

  GlobalTimer.stopClock();
  TheManager.dumpStats();
  GlobalTimer.printReport("Global");

  std::cerr.flush();
  std::cout.flush();

  return SyscallM->getExitStatus();
}
