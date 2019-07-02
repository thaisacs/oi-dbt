#include <machineModel.hpp>

double dbt::MachineModel::runOnMachine(std::vector<uint16_t> Genes, unsigned RegionID, 
    const std::string &AOSPath, const std::string &BinPath, const std::string &BinArgs) {
  
  int LoadStatus = TheMachine->loadELF(BinPath);

  if (!LoadStatus) {
    std::cerr << "Can't find or process ELF file" << std::endl;
    return 2;
  }

  dbt::AOS A(true, AOSPath, BinPath, BinArgs);
  dbt::Manager TheManager(TheMachine->getDataMemOffset(), *(TheMachine.get()), A, false, false);
  TheManager.setOptPolicy(dbt::Manager::OptPolitic::Normal);
  TheManager.startCompilationThr();
  std::unique_ptr<dbt::SyscallManager> SyscallM;

  ROIInfo ROI;
  ROI.RegionID = RegionID;
  ROI.Opts = std::move(Genes);
  
  TheManager.setROI(ROI);
  TheManager.setROIMode();
  TheManager.setLockMode(true);

  dbt::Timer GlobalTimer;

  LoadStatus = TheMachine->loadELF(BinPath);

  if (!LoadStatus) {
    std::cerr << "Can't find or process ELF file" << std::endl;
    return 2;
  }

  RftDefault = std::make_unique<dbt::NETPlus>(TheManager, true);

  SyscallM = std::make_unique<dbt::LinuxSyscallManager>(true);

  TheMachine->setCommandLineArguments(BinArgs);
  GlobalTimer.startClock();
  dbt::ITDInterpreter I(*SyscallM.get(), *RftDefault.get());
  
  I.executeAll((*TheMachine.get()));

  GlobalTimer.stopClock();
  //TheManager.dumpStats();

  std::cerr.flush();
  std::cout.flush();

  return TheManager.getRegionTime();
}
