#include "llvm/ADT/APInt.h"
#include "llvm/ADT/PostOrderIterator.h" // For ReversePostOrderTraversal.
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"       // To instantiate RPOTraversal.
#include "llvm/IR/Constants.h" // For ConstantInt.
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h" // For BinaryOperator, etc.
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h" // For errs().

#include <optional>
using namespace llvm;

// Takes \p Foo and apply a simple constant propagation optimization.
// \returns true if \p Foo was modified (i.e., something had been constant
// propagated), false otherwise.
bool myConstantPropagation(Function &Foo) {
  // TODO: populate this function.
  return false;
}
