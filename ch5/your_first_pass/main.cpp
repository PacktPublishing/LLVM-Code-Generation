#include "llvm/ADT/STLExtras.h"    // For llvm::all_of.
#include "llvm/AsmParser/Parser.h" // For parseAssemblyString.
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h" // For legacy::PassManager.
#include "llvm/IR/Module.h"
#include "llvm/IR/PassInstrumentation.h"
#include "llvm/IR/PassManager.h" // For the new PassManager.
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"        // For parseIRFile.
#include "llvm/Support/Debug.h"            // For errs().
#include "llvm/Support/SourceMgr.h"        // For SMDiagnostic.
#include "llvm/Transforms/Utils/Cloning.h" // For CloneModule.

#include "solution/passWithNewPM.h"
#include "your_turn/passWithNewPM.h"

using namespace llvm;

extern Pass *createSolutionPassForLegacyPM();
extern Pass *createYourTurnPassForLegacyPM();

bool checkFunctionCorrectness(const llvm::Function &Res) {
  Res.print(errs());
  // verifyFunction returns true if it finds errors and
  // print them on the provided output stream (errs() here).
  if (verifyFunction(Res, &errs())) {
    errs() << Res.getName() << " does not verify\n";
    return false;
  }
  return true;
}

// Default input in case no file was provided.
const char *InputIR =
    "define i32 @foo(i32 noundef %arg) {\n"
    "bb:\n"
    "  %i = shl i32 5, 3\n"
    "  %i1 = icmp ne i32 %arg, 0\n"
    "  br i1 %i1, label %bb2, label %bb4\n"
    "\n"
    "bb2:                                              ; preds = %bb\n"
    "  %i3 = sdiv i32 %i, 5\n"
    "  br label %bb6\n"
    "\n"
    "bb4:                                              ; preds = %bb\n"
    "  %i5 = or i32 %i, 3855\n"
    "  br label %bb6\n"
    "\n"
    "bb6:                                              ; preds = %bb4, %bb2\n"
    "  %.0 = phi i32 [ %i3, %bb2 ], [ %i5, %bb4 ]\n"
    "  ret i32 %.0\n"
    "}\n"
    "\n"
    "define i32 @bar(i32 noundef %arg) {\n"
    "bb:\n"
    "  %i = shl i32 -1, 3\n"
    "  %i1 = icmp ne i32 %arg, 0\n"
    "  br i1 %i1, label %bb2, label %bb4\n"
    "\n"
    "bb2:                                              ; preds = %bb\n"
    "  %i3 = udiv i32 %i, 3\n"
    "  br label %bb6\n"
    "\n"
    "bb4:                                              ; preds = %bb\n"
    "  %i5 = or i32 %i, 3855\n"
    "  br label %bb6\n"
    "\n"
    "bb6:                                             ; preds = %bb4, %bb2\n"
    "  %.0 = phi i32 [ %i3, %bb2 ], [ %i5, %bb4 ]\n"
    "  %i7 = add i32 %.0, 1\n"
    "  ret i32 %i7\n"
    "}\n";

int main(int argc, char **argv) {
  LLVMContext Context;
  SMDiagnostic Err;
  std::unique_ptr<Module> MyModule;
  // To be able to play with the optimization a little bit,
  // support a mode where you can feed your own IR files.
  if (argc == 2) {
    outs() << "Reading module from '" << argv[1] << "'\n";
    MyModule = parseIRFile(argv[1], Err, Context);
  } else {
    MyModule = parseAssemblyString(InputIR, Err, Context);
  }
  if (!MyModule) {
    errs() << "Unable to build module\n";
    return -1;
  }

  bool hadError = false;
  for (int i = 0; i < 2; ++i) {
    outs() << "Checking the run for "
           << ((i == 0) ? "Provided solution" : "Your turn") << "\n";
    // Start with the legacy pass manager.
    std::unique_ptr<Module> ModuleForLegacyPM = CloneModule(*MyModule);

    legacy::PassManager LegacyPM;

    Pass *PassForLegacyPM = (i == 0) ? createSolutionPassForLegacyPM()
                                     : createYourTurnPassForLegacyPM();
    if (PassForLegacyPM)
      LegacyPM.add(PassForLegacyPM);

    LegacyPM.run(*ModuleForLegacyPM);
    bool solutionIsCorrect =
        all_of(ModuleForLegacyPM->functions(),
               [](const Function &F) { return checkFunctionCorrectness(F); });
    hadError |= !solutionIsCorrect;

    // Then do the same thing with the new pass manager.
    std::unique_ptr<Module> ModuleForNewPM = CloneModule(*MyModule);

    // CAREFUL the order of the manager is important here since the destructor
    // needs to be called in the right order otherwise it will crash.
    FunctionAnalysisManager FAM;
    ModuleAnalysisManager MAM;
    // Register the passes used implicitly at the start of the pipeline.
    MAM.registerPass([&] { return PassInstrumentationAnalysis(); });
    // Well this one is not as implicit, we ask for it with the call to the
    // adaptor below.
    MAM.registerPass([&] { return FunctionAnalysisManagerModuleProxy(FAM); });
    FAM.registerPass([&] { return ModuleAnalysisManagerFunctionProxy(MAM); });
    ModulePassManager NewPM;
    // Wrap the function pass into a module pass.
    if (i == 0)
      NewPM.addPass(createModuleToFunctionPassAdaptor(
          SolutionConstantPropagationNewPass()));
#if YOUR_TURN_IS_READY
    else
      NewPM.addPass(createModuleToFunctionPassAdaptor(
          YourTurnConstantPropagationNewPass()));
#endif
    NewPM.run(*ModuleForNewPM, MAM);

    solutionIsCorrect =
        all_of(ModuleForNewPM->functions(),
               [](const Function &F) { return checkFunctionCorrectness(F); });
    hadError |= !solutionIsCorrect;
  }
  return hadError;
}
