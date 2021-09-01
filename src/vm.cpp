#include "vm.hpp"
#include "chunk.hpp"
#include <iostream>
#include <vector>

using lox::VM;

uint8_t VM::readByte() {
  auto executingInstruction = codeChunk.code.at(instructionPointer);
  instructionPointer++;
  return executingInstruction;
}

lox::Value VM::readConstant() { return codeChunk.constantPool.at(readByte()); }

lox::InterpretResult VM::interpret(Chunk incomingChunk) {
  codeChunk = incomingChunk;
  instructionPointer = 0;

  return run();
}

lox::InterpretResult VM::run() {
  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    codeChunk.disassembleInstruction(instructionPointer);
#endif

    switch (readByte()) {
    case OpCode::OP_RETURN: {
      return InterpretResult::OK;
    }
    case OpCode::OP_CONSTANT: {
      auto constantValue = readConstant();
      printValue(constantValue);
      std::cout << "\n";
      break;
    }
    }
  }
}