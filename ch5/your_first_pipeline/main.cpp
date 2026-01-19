#include "llvm/ADT/STLExtras.h"           // For llvm::all_of.
#include "llvm/ADT/STLFunctionalExtras.h" // For llvm::function_ref.
#include "llvm/AsmParser/Parser.h"        // For parseAssemblyString.
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h" // For legacy::PassManager.
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"    // For the new PassManager.
#include "llvm/IR/PassTimingInfo.h" // For reportAndResetTimings.
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"        // For parseIRFile.
#include "llvm/Support/CommandLine.h"      // For cl::.
#include "llvm/Support/Debug.h"            // For errs().
#include "llvm/Support/SourceMgr.h"        // For SMDiagnostic.
#include "llvm/Transforms/Utils/Cloning.h" // For CloneModule.

using namespace llvm;

extern void runSolutionPassPipelineForLegacyPM(Module &);
extern void runSolutionPassPipelineForNewPM(Module &);
extern void runYourTurnPassPipelineForLegacyPM(Module &);
extern void runYourTurnPassPipelineForNewPM(Module &);

bool checkFunctionCorrectness(const llvm::Function &Res) {
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

static cl::opt<std::string> InputFilename(cl::Positional,
                                          cl::desc("<input IR file>"));

int main(int argc, char **argv) {
  LLVMContext Context;
  SMDiagnostic Err;
  std::unique_ptr<Module> MyModule;

  cl::ParseCommandLineOptions(argc, argv, "CH5 pipeline example\n");

  // To be able to play with the optimization a little bit,
  // support a mode where you can feed your own IR files.
  if (!InputFilename.empty()) {
    outs() << "Reading module from '" << InputFilename << "'\n";
    MyModule = parseIRFile(InputFilename, Err, Context);
  } else {
    MyModule = parseAssemblyString(InputIR, Err, Context);
  }
  if (!MyModule) {
    errs() << "Unable to build module\n";
    return -1;
  }

  function_ref<void(Module &)> functions[] = {
      runSolutionPassPipelineForLegacyPM, runSolutionPassPipelineForNewPM,
      runYourTurnPassPipelineForLegacyPM, runYourTurnPassPipelineForNewPM};

  bool hadError = false;
  for (int i = 0; i < 4; ++i) {
    outs() << "Checking the run for "
           << ((i < 2) ? "Provided solution" : "Your turn");
    outs() << " on the " << ((i & 1) ? "new" : "legacy") << " pass manager\n";
    // Start with the legacy pass manager.
    std::unique_ptr<Module> ModuleForLegacyPM = CloneModule(*MyModule);
    functions[i](*ModuleForLegacyPM);

    reportAndResetTimings(&outs());

    bool solutionIsCorrect =
        all_of(ModuleForLegacyPM->functions(),
               [](const Function &F) { return checkFunctionCorrectness(F); });
    hadError |= !solutionIsCorrect;
  }
  return hadError;
}
