#pragma once

#include "value.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace lox {

class OpCode {
public:
  static constexpr uint8_t OP_CONSTANT = 0;
  static constexpr uint8_t OP_RETURN = 1;
};

class Chunk {
private:
  std::vector<int>
      lineNumbers; // nth entry of this is the line number for nth byte of
                   // this.code; stored as a separate array to avoid messing
                   // with CPU cache of bytecode data

public:
  std::vector<uint8_t> code; // stores opcodes AND operands
  std::vector<Value> constantPool;

  void write(uint8_t byte, int lineNumber);
  int addConstant(Value constant);

  // debugging functionality
  void disassemble(const std::string &chunkName);
  int disassembleInstruction(int offset);
  int disassembleSimpleInstruction(const std::string &name, int offset);
  int disassembleConstantInstruction(const std::string &name, int offset);
};

} // namespace lox