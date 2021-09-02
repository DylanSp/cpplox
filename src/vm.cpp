#include "vm.hpp"
#include "chunk.hpp"
#include <deque>
#include <iostream>
#include <string>

using lox::VM;

uint8_t VM::readByte() {
  auto executingInstruction = codeChunk.code.at(instructionPointer);
  instructionPointer++;
  return executingInstruction;
}

lox::Value VM::readConstant() { return codeChunk.constantPool.at(readByte()); }

lox::InterpretResult VM::interpret(std::string source) {
  compiler.compile(source);
  return InterpretResult::OK;

  /*
  codeChunk = incomingChunk;
  instructionPointer = 0;

  return run();
   */
}

// prints stack from bottom to top
void VM::printStackContents() {
  std::cout << "          ";
  std::deque<Value> tempStack;

  while (!stack.empty()) {
    tempStack.push_front(stack.top());
    stack.pop();
  }

  for (auto &it : tempStack) {
    std::cout << "[ ";
    printValue(it);
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
    case OpCode::OP_NEGATE: {
      auto top = stack.top();
      stack.pop();
      stack.push(negateValue(top));
      break;
    }
    case OpCode::OP_ADD: {
      assembleBinaryOperation(addValues);
      break;
    }
    case OpCode::OP_SUBTRACT: {
      assembleBinaryOperation(subtractValues);
      break;
    }
    case OpCode::OP_MULTIPLY: {
      assembleBinaryOperation(multiplyValues);
      break;
    }
    case OpCode::OP_DIVIDE: {
      assembleBinaryOperation(divideValues);
      break;
    }
    }
  }
}

void VM::assembleBinaryOperation(
    std::function<lox::Value(lox::Value, lox::Value)> binaryOp) {
  auto rhs = stack.top();
  stack.pop();
  auto lhs = stack.top();
  stack.pop();
  auto result = binaryOp(lhs, rhs);
  stack.push(result);
}