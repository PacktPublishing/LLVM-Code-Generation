#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"    // For ConstantInt.
#include "llvm/IR/DerivedTypes.h" // For PointerType, FunctionType.
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h" // For errs().

#include <memory> // For unique_ptr

using namespace llvm;

// The goal of this function is to build a Module that
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
// The IR for this snippet (at O0) is:
// define void @foo(i32 %arg, i32 %arg1) {
// bb:
//   %i = alloca i32
//   %i2 = alloca i32
//   %i3 = alloca i32
//   store i32 %arg, ptr %i
//   store i32 %arg1, ptr %i2
//   %i4 = load i32, ptr %i
//   %i5 = load i32, ptr %i2
//   %i6 = add i32 %i4, %i5
//   store i32 %i6, ptr %i3
//   %i7 = load i32, ptr %i3
//   %i8 = icmp eq i32 %i7, 255
//   br i1 %i8, label %bb9, label %bb12
//
// bb9:
//   %i10 = load i32, ptr %i3
//   call void @bar(i32 %i10)
//   %i11 = call i32 @baz()
//   store i32 %i11, ptr %i3
//   br label %bb12
//
// bb12:
//   %i13 = load i32, ptr %i3
//   call void @bar(i32 %i13)
//   ret void
// }
//
// declare void @bar(i32)
// declare i32 @baz(...)
std::unique_ptr<Module> myBuildModule(LLVMContext &Ctxt) {

     Type* intType = Type::getInt32Ty(Ctxt);
     Type* voidType = Type::getVoidTy(Ctxt);

     std::unique_ptr<Module> MyModule = std::make_unique<Module>("MyModule", Ctxt);

     FunctionType *bazType = FunctionType::get(intType, false);
     FunctionType *barType = FunctionType::get(voidType, {intType}, false);

     FunctionCallee bazFun = MyModule->getOrInsertFunction("baz", bazType);
     FunctionCallee barFun = MyModule->getOrInsertFunction("bar", barType);

     FunctionType *fooType = FunctionType::get(voidType, {intType, intType}, false);
     Function *fooFun = Function::Create(fooType, GlobalValue::ExternalLinkage, "foo", *MyModule);
     
     
     
     BasicBlock *bb1 = BasicBlock::Create(Ctxt, "bb1", fooFun);
     BasicBlock *bb2 = BasicBlock::Create(Ctxt, "bb2", fooFun);
     BasicBlock *bb3 = BasicBlock::Create(Ctxt, "bb3", fooFun);
     


     IRBuilder builder(bb1);

     // creating instructions of bb1
     AllocaInst *var = builder.CreateAlloca(intType);
     AllocaInst *a = builder.CreateAlloca(intType);
     AllocaInst *b = builder.CreateAlloca(intType);

     builder.CreateStore(fooFun->getArg(0), a);
     builder.CreateStore(fooFun->getArg(1), b);

     Value *aVal = builder.CreateLoad(intType, a);
     Value *bVal = builder.CreateLoad(intType, b);


     Value *addab = builder.CreateAdd(aVal, bVal);

     builder.CreateStore(addab, var);

     Value *varVal = builder.CreateLoad(intType, var);

     Value *cmp = builder.CreateCmp(CmpInst::Predicate::ICMP_EQ, varVal, ConstantInt::get(intType, 0));

     builder.CreateCondBr(cmp, bb2, bb3);
     


     // creating instructions of bb2
     builder.SetInsertPoint(bb2);
     varVal = builder.CreateLoad(intType, var);

     builder.CreateCall(barFun, {varVal});
     Value *res = builder.CreateCall(bazFun);
     builder.CreateStore(res, var);
     builder.CreateBr(bb3);


     // creating instructions of bb3
     builder.SetInsertPoint(bb3);
     varVal = builder.CreateLoad(intType, var);
     builder.CreateCall(barFun, {varVal});
     builder.CreateRetVoid();
     builder.SetInsertPoint(bb3);

     return MyModule; 
}
