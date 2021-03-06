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
#include <machineModel.hpp>
#include <AOS.hpp>
#include <AOSParams.hpp>

clarg::argString RFTFlag("-rft", "Region Formation Technique (inet)", "netplus-e-r");
clarg::argString AOSFlag("-aos", "Adaptive Optimization System file", "");
clarg::argString ROIFlag("-roi", "Region of investigation", "");
clarg::argBool LockFlag("-lock", "Activates lock mode");
clarg::argBool NLockFlag("-unlock", "Disables lock mode");
clarg::argInt    HotnessFlag("-hot", "Hotness threshold for the RFTs", 50);
clarg::argString ReportFileFlag("-report", "Write down report to a file", "");
clarg::argBool   InterpreterFlag("-interpret",  "Only interpret.");
clarg::argString BinaryFlag("-bin",  "Path to the binary which will should be emulated.", "");
clarg::argBool   PreheatFlag("-p",  "Run one time to compile all regions and then reexecute measuring the time.");
clarg::argBool   VerboseFlag("-v",  "display the compiled regions");
clarg::argBool   HelpFlag("-h",  "display the help message");
clarg::argInt    RegionLimitSize("-l", "region size limit", 0);
clarg::argString ToCompileFlag("-tc", "Functions to compile", "");
clarg::argString ArgumentsFlag("-args", "Pass Parameters to binary file (as string)", "");
clarg::argInt	 StackSizeFlag("-stack", "Set new stack size. (Default: 128mb)" , STACK_SIZE);
clarg::argInt	 HeapSizeFlag ("-heap", "Set new heap size (Default: 128mb)", HEAP_SIZE);
clarg::argInt	 NumThreadsFlag ("-threads", "Number of compilation threads (min 1)", 1);
clarg::argString RegionPath ("-reg", "Set default path to load region files", "./");
clarg::argBool   InlineFlag ("-inline", "Set the compiler to emit a LLVM function to each called function", "./");

clarg::argBool   WholeCompilationFlag("-wc",  "load .bc files and compile them all as one region (whole compilation).");

/* Iterative Compiler Tools */
clarg::argBool   DumpRegionsFlag("-dr", "Dump Regions (llvm ir and OI) to files");
clarg::argBool   DumpOIRegionsFlag("-doi", "Dump OI Regions to files");
clarg::argBool   LoadRegionsFlag("-lr", "Load Regions (.bc) from files");
clarg::argBool   LoadOIFlag("-loi", "Load Regions (.oi) from files");
clarg::argBool   MergeOIFlag("-moi", "Merge OI Regions before dumping");
clarg::argString CustomOptsFlag("-opts", "path to regions optimization list file", "");
clarg::argInt    ExecsFlag("-execs",  "number of times to execute a binary.", 1);
clarg::argString BinariesFlag("-bins",  "File with list of binaries to be executed.", "");
clarg::argString OptPolicyFlag("-level", "optimization level (none, basic, normal, agrresive)", "basic");

#ifdef DEBUG
clarg::argInt debugFlag ("-d", "Set Debug Level. This value can be 1 or 2 (1 - Less verbosive; 2 - More Verbosive)", 1);
#endif

void usage(char* PrgName) {
  cout << "Version: 0.0.1 (09-04-2018)";
  #ifdef DEBUG
  cout << " (Debug Build) ";
  #endif
  cout << "\n\n";
  cout << "Usage: " << PrgName <<
    " [-rft {net, net-r, mret2, lef, lei, netplus, netplus-e-r, mb}] [-interpret] -bin PathToBinary\n\n";

  cout << "DESCRIPTION:\n";
  cout << "This program implements the OpenISA DBT (Dynamic Binary Translator)\n" <<
    "Vanderson Martins do Rosario <vandersonmr2@gmail.com>, 2018.\n\n";

  cout << "ARGUMENTS:\n";
  clarg::arguments_descriptions(cout, "  ", "\n");
}

int validateArguments() {
  if (InterpreterFlag.was_set() && RFTFlag.was_set()) {
    cerr << "You can't use a RFT when you are only interpreting!\n";
    return 1;
  }

  //if (!AOSFlag.was_set()) {
  //  cerr << "You must set the path of the aos file which will be usage!\n";
  //  return 1;
  //}

  if (!BinaryFlag.was_set() && !BinariesFlag.was_set()) {
    cerr << "You must set the path of the binary which will be emulated!\n";
    return 1;
  }

  if (LoadRegionsFlag.was_set() && LoadOIFlag.was_set()) {
    cerr << "You cannot use -lr and -loi together!\n";
    return 1;
  }

  if (BinaryFlag.was_set() && BinariesFlag.was_set()) {
    cerr << "You cannot use -bin and -bins at the same time!\n";
    return 1;
  }

  return 0;
}

std::unique_ptr<dbt::RFT> RftChosen;
dbt::Machine M;

void  sigHandler(int sig) {
  if (M.isOnNativeExecution()) {
    std::cerr << "Error while executing region " << std::hex << M.getRegionBeingExecuted() << "\n";
  } else {
    std::cerr << "Error while executing the interpreter.\n";
    if (M.getRegionBeingExecuted() != 0)
      std::cerr << "The last executed region was " << std::hex << M.getRegionBeingExecuted() << "\n";
  }

  if(sig == SIGABRT)
    std::cerr << "SIGABRT (" << sig << ") while emulating at PC: " << std::hex << M.getPC() << std::dec << "\n";
  else
    std::cerr << "SIGSEGV (" << sig << ") while emulating at PC: " << std::hex << M.getPC() << std::dec << "\n";

  std::cerr << "Last Machine state:";

  exit(1);
}

void doNothingHandler(int sig) {}

void 
emulateBinary(std::string file, uint32_t NumExecs, dbt::SyscallManager *SyscallM, dbt::Manager &TheManager, 
                dbt::Machine &M, dbt::RFT *RftChosen) {

  int LoadStatus = M.loadELF(file);

  if (!LoadStatus) {
    std::cerr << "Can't find or process ELF file " << file << std::endl;
    exit(2);
  }

  TheManager.setDataMemOffset(M.getDataMemOffset());

  dbt::Timer GlobalTimer;

  if (PreheatFlag.was_set()) {
    if(M.setCommandLineArguments(ArgumentsFlag.get_value()) < 0)
      exit(1);

    M.setPreheating(true);
    std::cerr << "Preheating...\n";

    GlobalTimer.startClock();
    dbt::ITDInterpreter I(*SyscallM, *RftChosen);
    I.executeAll(M);
    GlobalTimer.stopClock();

    std::cerr << "done\n";
    std::cerr << "Cleaning VM... ";
    M.reset();
    std::cerr << "done\n";

    RftChosen = new dbt::PreheatRFT(TheManager);
    GlobalTimer.printReport("Preheat");

    while (TheManager.getNumOfOIRegions() != 0) {}
    M.setPreheating(false);
  }

  for (int E = 0; E < NumExecs; E++) {
    if(M.setCommandLineArguments(ArgumentsFlag.get_value()) < 0)
        exit(1);

    dbt::ITDInterpreter I(*SyscallM, *RftChosen);
    TheManager.incExecCount();
    std::cerr << "Starting execution:\n";

    GlobalTimer.startClock();
    I.executeAll(M);
    GlobalTimer.stopClock();

    if (DumpRegionsFlag.was_set() || DumpOIRegionsFlag.was_set())
        TheManager.dumpRegions(MergeOIFlag.was_set(), DumpOIRegionsFlag.was_set());

    TheManager.dumpStats();
    GlobalTimer.printReport("Global");
    RftChosen->reset();
    M.reset();
    TheManager.reset();
  }

  if (PreheatFlag.was_set()) {
      delete RftChosen;
  }
}

std::unordered_map<uint32_t, std::vector<std::string>>* loadCustomOpts(std::string CustomOptsPath) {
  auto CustomOpts = new std::unordered_map<uint32_t, std::vector<std::string>>;

  ifstream File;
  File.open(CustomOptsPath);
  if (!File.is_open()) {
    std::cerr << "You must provide a valid path to the custom opts file\n";
    exit(1);
  }

  std::string Line;
  while (std::getline(File, Line)) {
    std::istringstream ISS(Line);
    uint32_t EntryAddrs;
    ISS >> EntryAddrs;

    std::string OPT;
    while (ISS >> OPT) (*CustomOpts)[EntryAddrs].push_back(OPT);
  }

  return CustomOpts;
}

int main(int argc, char** argv) {
  srand(time(NULL));
  
  signal(SIGSEGV, sigHandler);
  signal(SIGABRT, sigHandler);

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

  if (StackSizeFlag.was_set()) {
    std::cerr << "Stack size was set to " << StackSizeFlag.get_value() << std::endl;
    M.setStackSize(StackSizeFlag.get_value());
  }

  if (HeapSizeFlag.was_set()) {
    std::cerr << "Heap size was set to " << HeapSizeFlag.get_value() << std::endl;
    M.setHeapSize(HeapSizeFlag.get_value());
  }

  std::unique_ptr<dbt::Manager> TheManager;

  if(!AOSFlag.was_set()) {
    TheManager = std::make_unique<dbt::Manager>(
        M, nullptr, VerboseFlag.was_set(), InlineFlag.was_set());
  }else {
    std::shared_ptr<dbt::AOS> TheAOS;
    
    TheAOS = std::make_shared<dbt::AOS>(ROIFlag.was_set(), AOSFlag.get_value(), 
        BinaryFlag.get_value(), ArgumentsFlag.get_value());
    
    TheManager = std::make_unique<dbt::Manager>(M, TheAOS, VerboseFlag.was_set(), 
        InlineFlag.was_set());
  
    if(TheAOS->isTraining() || TheAOS->isCBRSerialized())
      TheManager->setLockMode(true);

    if(ROIFlag.was_set()) {
      dbt::ROIInfo ROI;
      
      std::string Info = ROIFlag.get_value();

      unsigned i = 0, Buffer = 0;
      char c = Info[i];

      ROI.RegionID = 0;

      while(c != ':') {
        ROI.RegionID = ROI.RegionID * 10 + c - 48;
        c = Info[++i];
      }

      c = Info[++i];

      while(i < Info.size()) {
        if(c == '-') {
          ROI.Opts.push_back(Buffer);
          Buffer = 0;
          c = Info[++i];
        }

        Buffer = Buffer * 10 + c - 48;
        c = Info[++i];
      }

      ROI.Opts.push_back(Buffer);

      TheManager->setROI(ROI);
      TheManager->setROIMode();
      TheManager->setLockMode(true);
    }
  }

  if (LockFlag.was_set())
    TheManager->setLockMode(true);

  if (NLockFlag.was_set())
    TheManager->setLockMode(false);

  if (LoadRegionsFlag.was_set() || LoadOIFlag.was_set() || WholeCompilationFlag.was_set())
    TheManager->setToLoadRegions(RegionPath.get_value(), (!LoadOIFlag.was_set() && !WholeCompilationFlag.was_set()), WholeCompilationFlag.was_set());

  if (CustomOptsFlag.was_set()) {
    TheManager->setOptPolicy(dbt::Manager::OptPolitic::Custom);
    TheManager->setCustomOpts(loadCustomOpts(CustomOptsFlag.get_value()));
  } else {
    if (OptPolicyFlag.was_set()) {
      std::string policy = OptPolicyFlag.get_value();
      if (policy.compare("none") == 0)
        TheManager->setOptPolicy(dbt::Manager::OptPolitic::None);
      else if (policy.compare("basic") == 0)
        TheManager->setOptPolicy(dbt::Manager::OptPolitic::Basic);
      else if (policy.compare("normal") == 0)
        TheManager->setOptPolicy(dbt::Manager::OptPolitic::Normal);
      else if (policy.compare("aggressive") == 0)
        TheManager->setOptPolicy(dbt::Manager::OptPolitic::Aggressive);
      else {
        std::cout << "OptPolicy not found!!!";
        exit(1);
      }
    } else {
      TheManager->setOptPolicy(dbt::Manager::OptPolitic::Basic);
    }
  }

  TheManager->startCompilationThr();

  if (InterpreterFlag.was_set()) {
    RftChosen = std::make_unique<dbt::NullRFT>(*TheManager.get());
  } else {
    std::string RFTName = RFTFlag.get_value();
    transform(RFTName.begin(), RFTName.end(), RFTName.begin(), ::tolower);

    if (LoadRegionsFlag.was_set() || LoadOIFlag.was_set() || WholeCompilationFlag.was_set()) {
      std::cerr << "Preheated RFT Selected\n";
      RftChosen = std::make_unique<dbt::PreheatRFT>(*TheManager.get());
    } else if (RFTName == "net") {
      std::cerr << "NET RFT Selected\n";
      RftChosen = std::make_unique<dbt::NET>(*TheManager.get());
    } else if (RFTName == "net-r") {
      std::cerr << "NET-R RFT Selected\n";
      RftChosen = std::make_unique<dbt::NET>(*TheManager.get(), true);
    } else if (RFTName == "mret2") {
      std::cerr << "MRET2 RFT Selected\n";
      RftChosen = std::make_unique<dbt::MRET2>(*TheManager.get());
    } else if (RFTName == "netplus") {
      std::cerr << "NETPlus RFT Selected\n";
      RftChosen = std::make_unique<dbt::NETPlus>(*TheManager.get());
    } else if (RFTName == "netplus-c") {
      std::cerr << "NETPlus-c RFT Selected\n";
      RftChosen = std::make_unique<dbt::NETPlus>(*TheManager.get(), false, true);
    } else if (RFTName == "netplus-e-r") {
      std::cerr << "NETPlus-e-r RFT Selected\n";
      RftChosen = std::make_unique<dbt::NETPlus>(*TheManager.get(), true);
    } else if (RFTName == "netplus-e-r-c") {
      std::cerr << "NETPlus-e-r-c RFT Selected\n";
      RftChosen = std::make_unique<dbt::NETPlus>(*TheManager.get(), true, true);
    } else {
      std::cerr << "You should select a valid RFT!\n";
      return 1;
    }
  }

  if(HotnessFlag.was_set()) {
    std::cerr << "The Hotness Threshold was set to " << HotnessFlag.get_value() << std::endl;
    RftChosen->setHotnessThreshold(HotnessFlag.get_value());
  }

  if (RegionLimitSize.was_set())
    RftChosen->setRegionLimitSize(RegionLimitSize.get_value());

  std::unique_ptr<dbt::SyscallManager> SyscallM;
  if(ROIFlag.was_set())
    SyscallM = std::make_unique<dbt::LinuxSyscallManager>(true);
  else
    SyscallM = std::make_unique<dbt::LinuxSyscallManager>(false);

  if (BinaryFlag.was_set()) {
    emulateBinary(BinaryFlag.get_value(), ExecsFlag.get_value(), SyscallM.get(), *TheManager.get(), M, RftChosen.get());
  } else if (BinariesFlag.was_set()) {
    std::cout << "Not supported\n";
    exit(0);
    //ifstream is(BinariesFlag.get_value());
    //string str;
    //while(getline(is, str)) {
    //    std::cout << "Starting emulations of " << BinaryFlag.get_value() << "\n"; 
    //    emulateBinary(str, ExecsFlag.get_value(), SyscallM.get(), *TheManager.get(), M, RftChosen.get());
    //}
  }

  std::cerr.flush();
  std::cout.flush();

  signal(SIGSEGV, doNothingHandler);
  signal(SIGABRT, doNothingHandler);

  exit(SyscallM->getExitStatus());
}
