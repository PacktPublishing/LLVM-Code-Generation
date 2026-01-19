#include "llvm/AsmParser/Parser.h" // For parseAssemblyString.
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h" // For parseIRFile.
#include "llvm/Support/Debug.h"     // For errs().
#include "llvm/Support/SourceMgr.h" // For SMDiagnostic.

using namespace llvm;

extern bool buggyConstantPropagation(llvm::Function &);

bool checkFunctionCorrectness(llvm::Function &Res) {
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
    "bb2:\n"
    "  %i3 = sdiv i32 %i, 5\n"
    "  br label %bb6\n"
    "\n"
    "bb4:\n"
    "  %i5 = or i32 %i, 3855\n"
    "  br label %bb6\n"
    "\n"
    "bb6:\n"
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
    "bb2:\n"
    "  %i3 = udiv i32 %i, 3\n"
    "  br label %bb6\n"
    "\n"
    "bb4:\n"
    "  %i5 = or i32 %i, 3855\n"
    "  br label %bb6\n"
    "\n"
    "bb6:\n"
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
  for (Function &Func : *MyModule) {
    outs() << "Processing function '" << Func.getName() << "'\n";
    Func.print(outs());

    // Clone the function before the optimization to make sure each
    // implementation sees the same input.

    outs() << "\n\n## Reference implementation\n";
    bool solutionDidSomething = buggyConstantPropagation(Func);
    bool solutionIsCorrect = checkFunctionCorrectness(Func);

    if (!solutionIsCorrect) {
      hadError = true;
      errs() << "Solution does not verify:\n"
                "- provided implementation("
             << (solutionIsCorrect ? "passed" : "failed")
             << ")\n";
    }

    outs() << "\n######\n";
  }

  return hadError;
}
