#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/MachineFrameInfo.h" // For CreateStackObject.
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h" // For MachinePointerInfo.
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/TargetOpcodes.h"     // For INLINEASM.
#include "llvm/CodeGenTypes/LowLevelType.h" // For LLT.
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h" // For ICMP_EQ.

using namespace llvm;

// The goal of this function is to build a MachineFunction that
// represents the lowering of the following foo, a C function:
// extern int baz();
// extern void bar(int);
// void foo(int a, int b) {
//   int var = a + b;
//   if (var == 0xFF) {
//     bar(var);
//     var = baz();
//   }
//   bar(var);
// }
//
// The proposed ABI is:
// - 32-bit arguments are passed through registers: w0, w1
// - 32-bit returned values are passed through registers: w0, w1
// w0 and w1 are given as argument of this Function.
//
// The local variable named var is expected to live on the stack.
MachineFunction *populateMachineIR(MachineModuleInfo &MMI, Function &Foo,
                                   Register W0, Register W1)
{
  MachineFunction &MF = MMI.getOrCreateMachineFunction(Foo);

  // The type for bool.
  LLT I1 = LLT::scalar(1);
  // The type of var.
  LLT I32 = LLT::scalar(32);

  // To use to create load and store for var.
  MachinePointerInfo PtrInfo;
  Align VarStackAlign(4);

  // The type for the address of var.
  LLT VarAddrLLT = LLT::pointer(/*AddressSpace=*/0, /*SizeInBits=*/64);

  // The stack slot for var.
  int FrameIndex = MF.getFrameInfo().CreateStackObject(32, VarStackAlign,
                                                       /*IsSpillSlot=*/false);

  // TODO: Populate MF.

  // building basic blocks
  MachineBasicBlock *BB1 = MF.CreateMachineBasicBlock();
  MF.push_back(BB1);
  MachineBasicBlock *BB2 = MF.CreateMachineBasicBlock();
  MF.push_back(BB2);
  MachineBasicBlock *BB3 = MF.CreateMachineBasicBlock();
  MF.push_back(BB3);

  // connecting basic blocks
  BB1->addSuccessor(BB2);
  BB1->addSuccessor(BB3);
  BB2->addSuccessor(BB3);

  // populating bb1
  MachineIRBuilder builder(*BB1, BB1->end());

  Register A = builder.buildCopy(I32, W0).getReg(0);
  Register B = builder.buildCopy(I32, W1).getReg(0);

  Register VarAddr = builder.buildFrameIndex(VarAddrLLT, FrameIndex).getReg(0);

  Register Add = builder.buildAdd(I32, A, B).getReg(0);

  builder.buildStore(Add, VarAddr, PtrInfo, VarStackAlign);

  Register Zero = builder.buildConstant(I32, 0).getReg(0);
  Register Var = builder.buildLoad(I32, VarAddr, PtrInfo, VarStackAlign).getReg(0);

  Register Cmp = builder.buildICmp(CmpInst::ICMP_EQ, I1, Zero, Var).getReg(0);

  builder.buildBrCond(Cmp, *BB2);
  builder.buildBr(*BB3);

  // populating bb2
  builder.setInsertPt(*BB2, BB2->end());
  Var = builder.buildLoad(I32, VarAddr, PtrInfo, VarStackAlign).getReg(0);
  builder.buildCopy(W0, Var);
  builder.buildInstr(TargetOpcode::INLINEASM, {}, {})
      .addExternalSymbol("bl @bar")
      .addImm(0)
      .addReg(W0, RegState::Implicit);

  builder.buildInstr(TargetOpcode::INLINEASM, {}, {})
      .addExternalSymbol("bl @baz")
      .addImm(0)
      .addReg(W0, RegState::Implicit | RegState::Define);

  Register BazRes = builder.buildCopy(I32, W0).getReg(0);

  builder.buildStore(BazRes, VarAddr, PtrInfo, VarStackAlign);

  builder.buildBr(*BB3);

  // populating bb3
  builder.setInsertPt(*BB3, BB3->end());
  Var = builder.buildLoad(I32, VarAddr, PtrInfo, VarStackAlign).getReg(0);
  builder.buildCopy(W0, Var);
  builder.buildInstr(TargetOpcode::INLINEASM, {}, {})
      .addExternalSymbol("bl @bar")
      .addImm(0)
      .addReg(W0, RegState::Implicit);

  builder.buildInstr(TargetOpcode::INLINEASM, {}, {})
      .addExternalSymbol("ret")
      .addImm(0);

  return &MF;
}
