#include "chunk.hpp"
#include <iomanip>
#include <iostream>
#include <string>

using lox::Chunk;

void Chunk::write(OpCode opcode) {
  this->code.push_back(static_cast<uint8_t>(opcode));
}

void Chunk::write(uint8_t byte) { this->code.push_back(byte); }

// returns the index of the added constant, so we can look it up later
// note: this may indicate we want a different data structure
int Chunk::addConstant(Value constant) {
  this->constantPool.push_back(constant);
  return this->constantPool.size() - 1;
}

void Chunk::disassemble(std::string chunkName) {
  std::cout << "== " << chunkName << " =="
            << "\n";

  // instructions can have different sizes, so we update offset with the size of
  // the last instruction disassembled note: not married to this
  for (auto offset = 0; offset < this->code.size();) {
    offset = disassembleInstruction(offset);
  }
}

int Chunk::disassembleInstruction(int offset) {
  std::ios_base::fmtflags f(std::cout.flags());

  // byte offset of the instruction within the chunk
  std::cout << std::setfill('0') << std::setw(4) << offset << " ";

  std::cout.flags(f);

  auto instruction = this->code.at(offset);
  switch (instruction) {
  case static_cast<uint8_t>(OpCode::OP_CONSTANT):
    return disassembleConstantInstruction("OP_CONSTANT", offset);
  case static_cast<uint8_t>(OpCode::OP_RETURN):
    return disassembleSimpleInstruction("OP_RETURN", offset);
  default:
    std::cout << "Unknown opcode " << static_cast<uint8_t>(instruction) << "\n";
    return offset + 1;
  }
}

// for disassembling zero-operand (one-byte) simple instructions
int Chunk::disassembleSimpleInstruction(std::string name, int offset) {
  std::cout << name << "\n";
  return offset + 1;
}

// for disassembling one-operand (two-byte) constant instructions
int Chunk::disassembleConstantInstruction(std::string name, int offset) {
  auto constantIndex = code.at(offset + 1);
  std::cout << name << "@ "
            << static_cast<int>(
                   constantIndex) // static_cast is necessary to avoid treating
                                  // constantIndex like an ASCII char
            << " value: ";
  printValue(constantPool.at(constantIndex));
  std::cout << "\n";
  return offset + 2;
}