#include "vm.hpp"
#include "chunk.hpp"
#include <iostream>

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

// prints stack from bottom to top
void VM::printStackContents() {
  std::cout << "          ";
  std::vector<Value> tempStack;

  while (!stack.empty()) {
    tempStack.push_back(stack.top());
    stack.pop();
  }

  for (auto it = tempStack.rbegin(); it != tempStack.rend(); it++) {
    std::cout << "[ ";
    printValue(*it);
    std::cout << " ]";
  }
  std::cout << "\n";

  for (auto elem : tempStack) {
    stack.push(elem);
  }
}

lox::InterpretResult VM::run() {
  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    codeChunk.disassembleInstruction(instructionPointer);
    printStackContents();
#endif
    switch (readByte()) {
    case OpCode::OP_RETURN: {
      auto topOfStack = stack.top();
      stack.pop();

      std::cout << "top of stack:"
                << "\n";
      printValue(topOfStack);
      std::cout << "\n";

      return InterpretResult::OK;
    }
    case OpCode::OP_CONSTANT: {
      auto constantValue = readConstant();
      stack.push(constantValue);
      break;
    }
    }
  }
}