#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Debug.h" // For errs().

using namespace llvm;

extern std::unique_ptr<Module> buggyBuildModule();

int main(int argc, char **argv) {
  bool hadError = false;
  std::unique_ptr<Module> CurModule = buggyBuildModule();
  outs() << "\n\n## Processing module\n";
  if (!CurModule) {
    outs() << "Nothing built\n";
    return 0;
  }

  CurModule->print(errs(), /*AssemblyAnnotationWriter=*/nullptr);
  // verifyModule  returns true if it finds errors and
  // print them on the provided output stream (errs() here).
  if (verifyModule(*CurModule, &errs())) {
    errs() << "Impl does not verify\n";
    hadError |= true;
  }

  return hadError;
}
