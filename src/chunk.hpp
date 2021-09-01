#pragma once
#include "value.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace lox {
enum class OpCode : uint8_t { OP_CONSTANT, OP_RETURN };

class Chunk {
private:
  std::vector<uint8_t>
      code; // stores opcodes AND operands; unsure what type this should be
  std::vector<Value> constantPool;
  std::vector<int>
      lineNumbers; // nth entry of this is the line number for nth byte of
                   // this.code; stored as a separate array to avoid messing
                   // with CPU cache of bytecode data

public:
  void write(OpCode opcode, int lineNumber);
  void write(uint8_t byte, int lineNumber);
  int addConstant(Value constant);

  // debugging functionality
  void disassemble(std::string chunkName);
  int disassembleInstruction(int offset);
  int disassembleSimpleInstruction(std::string name, int offset);
  int disassembleConstantInstruction(std::string name, int offset);
};

} // namespace lox