#pragma once

#include "chunk.hpp"
#include "compiler.hpp"
#include "value.hpp"
#include <functional>
#include <stack>
#include <string>

namespace lox {

enum class InterpretResult { OK, COMPILE_ERROR, RUNTIME_ERROR };

class VM {
private:
  lox::Compiler compiler;

  lox::Chunk codeChunk;

  // points to the instruction _about to be executed_
  // address of an instruction in codeChunk.code
  size_t instructionPointer; // raw C pointer in Crafting Interpreters; may
                             // need to change this later?
  std::stack<Value> stack;

  InterpretResult run();
  uint8_t readByte();
  Value readConstant();

  void assembleBinaryOperation(
      std::function<lox::Value(lox::Value, lox::Value)>);

  // debugging functionality
  void printStackContents();

public:
  InterpretResult interpret(const std::string &source);
};

} // namespace lox