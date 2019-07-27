#include <IROpt.hpp>

using namespace dbt;

constexpr unsigned int str2int(const char* str, int h = 0) {
  return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

void IROpt::populatePassManager(llvm::legacy::PassManager* MPM, llvm::legacy::FunctionPassManager* FPM, 
    std::vector<uint16_t> Passes) {
  for (int PassIndex = 0; PassIndex < Passes.size(); PassIndex++) {
    switch (Passes[PassIndex]) { 
      case BASICAA:
        FPM->add(llvm::createBasicAAWrapperPass());
        break;
      case EARLY_CSE:
        FPM->add(llvm::createEarlyCSEPass());
        break;
      case _GVN: //**
        FPM->add(llvm::createNewGVNPass());
        break;
      case INSTCOMBINE: //**
        FPM->add(llvm::createInstructionCombiningPass());
        break;
      case INDVARS: //**
        FPM->add(llvm::createIndVarSimplifyPass());
        break;
      case LICM: //**
        FPM->add(llvm::createLICMPass());
        break;
      case LOOP_INSTSIMPLIFY:
        FPM->add(llvm::createLoopInstSimplifyPass());
        break;
      case LOOP_REDUCE:
        FPM->add(llvm::createLoopStrengthReducePass());
        break;
      case LOOP_IDIOM: 
        FPM->add(llvm::createLoopIdiomPass());
        break;
      case LOOP_DELETION: //**
        FPM->add(llvm::createLoopDeletionPass());
        break;
      case LOOP_ROTATE: 
        FPM->add(llvm::createLoopRotatePass());
        break;
      case LOOP_UNROLL: //** 
        FPM->add(llvm::createSimpleLoopUnrollPass());
        break;
      case REASSOCIATE: //**
        FPM->add(llvm::createReassociatePass());
        break;
      case SIMPLIFYCFG: //**
        FPM->add(llvm::createCFGSimplificationPass());
        break;
      case TAILCALLELIM:
        FPM->add(llvm::createTailCallEliminationPass());
        break;
      case UNROLL_ALLOW_PARTIAL:
        // doesn't exist anymore
        break;
      case SIMPLIFY_LIBCALLS:
        // doesn't exist anymore
        break;
      case SCALARREPL:
        // doesn't exist anymore
        break;
      case CONSTPROP:
        FPM->add(llvm::createConstantPropagationPass());
        break;
      case ALIGNMENT_FROM_ASSSUMPTIONS:
        FPM->add(llvm::createAlignmentFromAssumptionsPass());
        break;
      case SCCP:
        FPM->add(llvm::createSCCPPass());
        break;
      case DCE: //**
        FPM->add(llvm::createDeadCodeEliminationPass());
        break;
      case DIE: //**
        FPM->add(llvm::createDeadInstEliminationPass());
        break;
      case DSE:
        FPM->add(llvm::createDeadStoreEliminationPass());
        break;
      case CALLSITE_SPLITTING:
        FPM->add(llvm::createCallSiteSplittingPass());
        break;
      case ADCE:
        FPM->add(llvm::createAggressiveDCEPass());
        break;
      case GUARD_WIDENING:
        FPM->add(llvm::createGuardWideningPass());
        break;
      case BDCE:
        FPM->add(llvm::createBitTrackingDCEPass());
        break;
      case IRCE:
        FPM->add(llvm::createInductiveRangeCheckEliminationPass());
        break;
      case LOOP_SINK:
        FPM->add(llvm::createLoopSinkPass());
        break;
      case LOOP_PREDICATION: //**
        FPM->add(llvm::createLoopPredicationPass());
        break;
      case LOOP_UNSWITCH: //**
        FPM->add(llvm::createLoopUnswitchPass());
        break;
      case LOOP_UNROLL_AND_JAM:
        FPM->add(llvm::createLoopUnrollAndJamPass());
        break;
      case LOOP_REROLL:
        FPM->add(llvm::createLoopRerollPass());
        break;
      case LOOP_VERSIONING_LICM:
        FPM->add(llvm::createLoopVersioningLICMPass());
        break;
      case JUMP_THREADING:
        FPM->add(llvm::createJumpThreadingPass());
        break;
      case FLATTENCFG:
        FPM->add(llvm::createFlattenCFGPass());
        break;
      case STRUCTURIZECFG:
        FPM->add(llvm::createStructurizeCFGPass());
        break;
      case _GVN_HOIST:
        FPM->add(llvm::createGVNHoistPass());
        break;
      case _GVN_SINK:
        FPM->add(llvm::createGVNSinkPass());
        break;
      case MLDST_MOTION:
        FPM->add(llvm::createMergedLoadStoreMotionPass());
        break;
      case NEWGVN:
        FPM->add(llvm::createNewGVNPass());
        break;
      case MEMCPYOPT: //**
        FPM->add(llvm::createMemCpyOptPass());
        break;
      case CONSTHOIST:
        FPM->add(llvm::createConstantHoistingPass());
        break;
      case SINK:
        FPM->add(llvm::createSinkingPass());
        break;
      case LOWERATOMIC:
        FPM->add(llvm::createLowerAtomicPass());
        break;
      case LOWER_GUARD_INTRINSIC:
        FPM->add(llvm::createLowerGuardIntrinsicPass());
        break;
      case MERGEICMPS:
        FPM->add(llvm::createMergeICmpsPass());
        break;
      case CORRELATED_PROPAGATION:
        FPM->add(llvm::createCorrelatedValuePropagationPass());
        break;
      case LOWER_EXPECT:
        FPM->add(llvm::createLowerExpectIntrinsicPass());
        break;
      case PARTIALLY_INLINE_LIBCALLS:
        FPM->add(llvm::createPartiallyInlineLibCallsPass());
        break;
      case SEPARATE_CONST_OFFSET_FROM_GEP:
        FPM->add(llvm::createSeparateConstOffsetFromGEPPass());
        break;
      case SLSR:
        FPM->add(llvm::createStraightLineStrengthReducePass());
        break;
      case PLACE_SAFEPOINTS:
        FPM->add(llvm::createPlaceSafepointsPass());
        break;
      case FLOAT2INT:
        FPM->add(llvm::createFloat2IntPass());
        break;
      case NARY_REASSOCIATE:
        FPM->add(llvm::createNaryReassociatePass());
        break;
      case LOOP_DISTRIBUTE:
        FPM->add(llvm::createLoopDistributePass());
        break;
      case LOOP_LEAD_ELIM:
        FPM->add(llvm::createLoopLoadEliminationPass());
        break;
      case LOOP_VERSIONING:
        FPM->add(llvm::createLoopVersioningPass());
        break;
      case LOOP_DATA_PREFETCH:
        FPM->add(llvm::createLoopDataPrefetchPass());
        break;
      case INSTSIMPLIFY:
        FPM->add(llvm::createInstSimplifyLegacyPass());
        break;
      case AGGRESSIVE_INSTCOMBINE:
        FPM->add(llvm::createAggressiveInstCombinerPass());
        break;
      case LOWERINVOKE:
        FPM->add(llvm::createLowerInvokePass());
        break;
      case INSTNAMER:
        FPM->add(llvm::createInstructionNamerPass());
        break;
      case LOWERSWITCH:
        FPM->add(llvm::createLowerSwitchPass());
        break;
      case BREAK_CRIT_EDGES:
        FPM->add(llvm::createBreakCriticalEdgesPass());
        break;
      case LCSSA:
        FPM->add(llvm::createLCSSAPass());
        break;
      case MEM2REG:
        FPM->add(llvm::createPromoteMemoryToRegisterPass());
        break;
      case LOOP_SINMPLIFY:
        FPM->add(llvm::createLoopSimplifyPass());
        break;
      case LOOP_VECTORIZE:
        FPM->add(llvm::createLoopVectorizePass());
        break;
      case SLP_VECTORIZER:
        FPM->add(llvm::createSLPVectorizerPass());
        break;
      case LOAD_STORE_VECTORIZER:
        FPM->add(llvm::createLoadStoreVectorizerPass());
        break;
      case LAZY_VALUE_INFO:
        FPM->add(llvm::createLazyValueInfoPass());
        break;
      case DA:
        FPM->add(llvm::createDependenceAnalysisWrapperPass());
        break;
      case COST_MODEL:
        FPM->add(llvm::createCostModelAnalysisPass());
        break;
      case DELINEARIZE:
        FPM->add(llvm::createDelinearizationPass());
        break;
      case DIVERGENCE:
        FPM->add(llvm::createDivergenceAnalysisPass());
        break;
      case INSTCOUNT:
        FPM->add(llvm::createInstCountPass());
        break;
      case REGIONS:
        FPM->add(llvm::createRegionInfoPass());
        break;
      case DOMTREE:
        FPM->add(llvm::createPostDomTree());
        break;
      case CONSTMERGE:
        MPM->add(llvm::createConstantMergePass());
        break;
      case GLOBALOPT:
        MPM->add(llvm::createGlobalOptimizerPass());
        break;
      case INLINE:
        MPM->add(llvm::createFunctionInliningPass());
        break;
      case IPSCCP:
        MPM->add(llvm::createIPSCCPPass());
        break;
      case PARTIAL_INLINER:
        MPM->add(llvm::createPartialInliningPass());
        break;
      case REWRITE_STATEPOINTS_FOR_GCC:
        MPM->add(llvm::createRewriteStatepointsForGCLegacyPass());
        break;
      case STRIP:
        MPM->add(llvm::createStripSymbolsPass());
        break;
      case SCALARIZER:
        FPM->add(llvm::createScalarizerPass());
        break;
      case STRIP_NONDEBUG:
        MPM->add(llvm::createStripNonDebugSymbolsPass());
        break;
      case STRIP_DEBUG_DECLARE:
        MPM->add(llvm::createStripDebugDeclarePass());
        break;
      case STRIP_DEAD_DEBUG_INFO:
        MPM->add(llvm::createStripDeadDebugInfoPass());
        break;
      case GLOBALDCE:
        MPM->add(llvm::createGlobalDCEPass());
        break;
      case ELIM_AVAIL_EXTERN:
        MPM->add(llvm::createEliminateAvailableExternallyPass());
        break;
      case PRUNE_EH:
        MPM->add(llvm::createPruneEHPass());
        break;
      case DEADARGELIM:
        MPM->add(llvm::createDeadArgEliminationPass());
        break;
      case ARGPROMOTION:
        MPM->add(llvm::createArgumentPromotionPass());
        break;
      case IPCONSTPROP:
        MPM->add(llvm::createIPConstantPropagationPass());
        break;
      case LOOP_EXTRACT:
        MPM->add(llvm::createLoopExtractorPass());
        break;
      case LOOP_EXTRACT_SINGLE:
        MPM->add(llvm::createSingleLoopExtractorPass());
        break;
      case EXTRACT_BLOCKS:
        MPM->add(llvm::createBlockExtractorPass());
        break;
      case STRIP_DEAD_PROTOTYPES:
        MPM->add(llvm::createStripDeadPrototypesPass());
        break;
      case MERGE_FUNC:
        MPM->add(llvm::createMergeFunctionsPass());
        break;
      case BARRIER:
        MPM->add(llvm::createBarrierNoopPass());
        break;
      case CALLED_VALUE_PROPAGATION:
        MPM->add(llvm::createCalledValuePropagationPass());
        break;
      case CROSS_DSO_CFI:
        MPM->add(llvm::createCrossDSOCFIPass());
        break;
      case GLOBALSPLIT:
        MPM->add(llvm::createGlobalSplitPass());
        break;
      case NONE:
        break;
      case METARENAMER:
        MPM->add(llvm::createMetaRenamerPass());
        break;
      case PA_EVAL:
        FPM->add(llvm::createPAEvalPass());
        break;
      case INTERNALIZE:
        //MPM->add(llvm::createInternalizePass());
        break;
      case INFER_ADDRESS_SPACES:
        FPM->add(llvm::createInferAddressSpacesPass());
        break;
      case SPECULATIVE_EXECUTION:
        FPM->add(llvm::createSpeculativeExecutionPass());
        break;
      case CODEGENPREPARE:
        FPM->add(llvm::createCodeGenPreparePass());
        break;
      case LOOP_GUARD_WIDENING:
        FPM->add(llvm::createLoopGuardWideningPass());
        break;
      case DIV_REM_PAIRS:
        FPM->add(llvm::createDivRemPairsPass());
        break;
      case LOOP_INTERCHANGE:
        FPM->add(llvm::createLoopInterchangePass());
        break;
      case SROA:
        FPM->add(llvm::createSROAPass());
        break;
      default:
        std::cerr << "Trying to use an invalid optimization pass!\n";
        exit(1);
        break;
    }
  }
}

void IROpt::populateFuncPassManager(llvm::legacy::FunctionPassManager* FPM, std::vector<std::string> PassesNames) {
  for (std::string PassName : PassesNames) {
    switch (str2int(PassName.c_str())) {
      case str2int("instcombine"):
        FPM->add(llvm::createInstructionCombiningPass());
        break;
      case str2int("simplifycfg"):
        FPM->add(llvm::createCFGSimplificationPass());
        break;
      case str2int("reassociate"):
        FPM->add(llvm::createReassociatePass());
        break;
      case str2int("gvn"):
        FPM->add(llvm::createNewGVNPass());
        break;
      case str2int("die"):
        FPM->add(llvm::createDeadInstEliminationPass());
        break;
      case str2int("dce"):
        FPM->add(llvm::createDeadCodeEliminationPass());
        break;
      case str2int("mem2reg"):
        //        FPM->add(llvm::createPromoteMemoryToRegisterPass());
        //        break;
      case str2int("licm"):
        FPM->add(llvm::createLICMPass());
        break;
      case str2int("memcpyopt"):
        FPM->add(llvm::createMemCpyOptPass());
        break;
      case str2int("loop-unswitch"):
        FPM->add(llvm::createLoopUnswitchPass());
        break;
      case str2int("indvars"):
        FPM->add(llvm::createIndVarSimplifyPass());       // Canonicalize indvars
        break;
      case str2int("loop-deletion"):
        FPM->add(llvm::createLoopDeletionPass());         // Delete dead loops
        break;
      case str2int("loop-predication"):
        FPM->add(llvm::createLoopPredicationPass());
        break;
      case str2int("loop-unroll"):
        FPM->add(llvm::createSimpleLoopUnrollPass());     // Unroll small loops
        break;
      default:
        std::cerr << "Trying to use an invalid optimization pass!\n";
        exit(1);
        break;
    }
  }
  std::cerr << "\n";
}

void IROpt::optimizeIRFunction(llvm::Module *M, std::vector<uint16_t> Opts) {
  auto FPM = std::make_unique<llvm::legacy::FunctionPassManager>(M);
  auto MPM = std::make_unique<llvm::legacy::PassManager>();

  populatePassManager(MPM.get(), FPM.get(), Opts);
  FPM->doInitialization();

  for (auto &F : *M) {
    FPM->run(F);
  }

  MPM->run(*M);
}

void IROpt::optimizeIRFunction(llvm::Module *M, OptLevel Level, uint32_t EntryAddress) {
  switch (Level) {
    case OptLevel::None:
      // Do not apply optimizations
      break;
    case OptLevel::Basic:
      if (!BasicPM) {
        BasicPM = std::make_unique<llvm::legacy::FunctionPassManager>(M);
        populateFuncPassManager(BasicPM.get(), 
            {"instcombine", "simplifycfg", "reassociate", "gvn", "die", "dce", "instcombine", "licm", 
            "memcpyopt", "loop-unswitch", "instcombine", "indvars", "loop-deletion", "loop-predication", "loop-unroll",
            "simplifycfg", "instcombine", "licm", "gvn"});
        BasicPM->doInitialization();

      }
      for (auto& F : *M)
        BasicPM->run(F);
      break;
    case OptLevel::Normal:
      break;
    case OptLevel::Aggressive:
      break;
  }
}

void IROpt::customOptimizeIRFunction(llvm::Module* M, std::vector<std::string> Opts) {
  std::cerr << "Custom opt " << Opts[1] << "\n";
  auto PM = std::make_unique<llvm::legacy::FunctionPassManager>(M);
  populateFuncPassManager(PM.get(), Opts);
  PM->doInitialization();
  for (auto &F : *M)
    PM->run(F);
}
