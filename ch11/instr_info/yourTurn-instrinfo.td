// Your turn:

// BREAKVALUE opcode
// - Add an instruction that creates two 32-bit values out of a 64-bit value.
// - The input value will be on the GPR64 register class and the output
//   values on the GPR32 register class.
// - The instruction needs to be in the MyTarget namespace.
// - The name of the opcode must start with YT (this is just to avoid name
//   collision with the solution.)

// LOADIMM32 opcode
// - Add an instruction that materializes a 32-bit immediate value
//   into a 32-bit register.
// - The input value will be an immediate operand and the output
//   value on a GPR32 register class.
// - The instruction needs to be in the MyTarget namespace.
// - The name of the opcode must start with YT (this is just to avoid name
//   collision with the solution.)
// - The instruction needs to be marked isAsCheapAsAMove.
//
// Hint for the immediate operand, look for the appropriate record name
// under the OPERAND_IMMEDIATE OperandType in llvm/include/llvm/Target/Target.td
