#include <arglib/arglib.hpp>
#include <interpreter.hpp>
#include <RFT.hpp>
#include <manager.hpp>
#include <syscall.hpp>
#include <timer.hpp>

#include <iostream>
#include <memory>

clarg::argString RFTFlag("-rft", "Region Formation Technique (net)", "net");
clarg::argBool   InterpreterFlag("-interpret",  "Only interpret.");
clarg::argString BinaryFlag("-bin",  "path to the binary which will should be emulated.", "");
clarg::argBool   PreheatFlag("-p",  "Run one time to compile all regions and then reexecute measuring the time.");
clarg::argBool   VerboseFlag("-v",  "display the compiled regions");
clarg::argBool   HelpFlag("-h",  "display the help message");
clarg::argInt    RegionLimitSize("-l", "region size limit", 0);

void usage(char* PrgName) {
  cout << "Version: 0.0.1 (03-01-2017)\n\n";

  cout << "Usage: " << PrgName << 
    " [-rft net] [-interpreter] -bin PathToBinary\n\n";

  cout << "DESCRIPTION:\n";
  cout << "This program implements the OpenISA DBT (Dynamic Binary Translator)\n" <<
    "Institute of Computing, 2017.\n\n";

  cout << "ARGUMENTS:\n";
  clarg::arguments_descriptions(cout, "  ", "\n");
}

int validateArguments() {
  if (InterpreterFlag.was_set() && RFTFlag.was_set()) {
    cerr << "You can't use a RFT when you are only interpreting!\n";
    return 1;
  }

  if (!BinaryFlag.was_set()) {
    cerr << "You must set the path of the binary which will be emulated!\n";
    return 1;
  }

  return 0;
}

std::unique_ptr<dbt::RFT> RftChosen;
dbt::Machine M;

void  sigHandler(int sig) {
  if (VerboseFlag.was_set())
    RftChosen->printRegions();

  if (M.isOnNativeExecution()) {
    std::cerr << "Error while executing region " << std::hex << M.getRegionBeingExecuted() << "\n"; 
  } else {
    std::cerr << "Error while executing the interpreter.\n";
    if (M.getRegionBeingExecuted() != 0)
      std::cerr << "The last executed region was " << std::hex << M.getRegionBeingExecuted() << "\n";
  }

  std::cerr << "Segfault (" << sig << ") while emulating at PC: " << std::hex << M.getPC() << std::dec << "\n";

  exit(1);
}

int main(int argc, char** argv) {
  signal(SIGSEGV, sigHandler);

  dbt::Timer GlobalTimer; 

  // Parse the arguments
  if (clarg::parse_arguments(argc, argv)) {
    cerr << "Error when parsing the arguments!" << endl;
    return 1;
  }

  if (HelpFlag.get_value() == true) {
    usage(argv[0]);
    return 1;
  }

  if (validateArguments())
    return 1;

  int loadStatus = M.loadELF(BinaryFlag.get_value());

  if (!loadStatus) {
    std::cerr << "Can't find or process ELF file " << argv[1] << std::endl;
    return 2;
  }

  dbt::Manager TheManager(1, dbt::Manager::OptPolitic::Normal, M.getDataMemOffset());

  if (InterpreterFlag.was_set()) {
    RftChosen = std::make_unique<dbt::NullRFT>(TheManager);
  } else {
    std::string RFTName = RFTFlag.get_value();
    if (RFTName == "net") {
      std::cerr << "NET RFT Selected\n";
      RftChosen = std::make_unique<dbt::NET>(TheManager);
    } else if (RFTName == "mret2") {
      std::cerr << "MRET2 RFT Selected\n";
      RftChosen = std::make_unique<dbt::MRET2>(TheManager);
    } else if (RFTName == "netplus") {
      std::cerr << "NETPlus RFT Selected\n";
      RftChosen = std::make_unique<dbt::NETPlus>(TheManager);
    } else if (RFTName == "lei") {
      std::cerr << "LEI rft selected\n";
      RftChosen = std::make_unique<dbt::LEI>(TheManager);
    } else if (RFTName == "lef") {
      std::cerr << "LEF rft selected\n";
      RftChosen = std::make_unique<dbt::LEF>(TheManager);
    } else {
      std::cerr << "You should select a valid RFT!\n";
      return 1;
    }
  }

  if (RegionLimitSize.was_set()) 
    RftChosen->setRegionLimitSize(RegionLimitSize.get_value());

  std::unique_ptr<dbt::SyscallManager> SyscallM;
  SyscallM = std::make_unique<dbt::LinuxSyscallManager>();

  if (PreheatFlag.was_set()) {
    std::cerr << "Preheating...";
    dbt::ITDInterpreter I(*SyscallM.get(), *RftChosen.get());
    I.executeAll(M);
    std::cerr << "done\n";

    std::cerr << "Cleaning VM...";
    M.reset();
    std::cerr << "done\n";

    RftChosen = std::make_unique<dbt::PreheatRFT>(TheManager);
  }

  GlobalTimer.startClock();
  dbt::ITDInterpreter I(*SyscallM.get(), *RftChosen.get());
  std::cerr << "Starting execution:\n";
  I.executeAll(M);
  GlobalTimer.stopClock();

  if (VerboseFlag.was_set())
    RftChosen->printRegions();

  GlobalTimer.printReport("Global");
  return SyscallM->getExitStatus();
}

/*
 * TODO:
 *  - Add all instructions need by printf (commit)
 *  - Correct bugs in at least 6 tests (commit)
 *  - Add new benchmarks (commit)
 *  - Make improvements on the code (commit)
 *  - Make improvements on the performance (commit)
 *  ---------------------------------------------------- Until: 24 May
*/
