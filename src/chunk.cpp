#include "chunk.hpp"
#include <iostream>
#include <iomanip>
#include <string>

using lox::Chunk;

void Chunk::write(OpCode opcode)
{
  this->code.push_back(opcode);
}

void Chunk::disassemble(std::string chunkName)
{
  std::cout << "== " << chunkName << " =="
            << "\n";

  // instructions can have different sizes, so we update offset with the size of the last instruction disassembled
  // note: not married to this
  for (auto offset = 0; offset < this->code.size();)
  {
    offset = disassembleInstruction(offset);
  }
}

// for disassembling one-byte simple instructions
int simpleInstruction(std::string name, int offset)
{
  std::cout << name << "\n";
  return offset + 1;
}

int Chunk::disassembleInstruction(int offset)
{
  std::ios_base::fmtflags f(std::cout.flags());

  // byte offset of the instruction within the chunk
  std::cout << std::setfill('0') << std::setw(4) << offset << " ";

  std::cout.flags(f);

  auto instruction = this->code.at(offset);
  switch (instruction)
  {
  case OpCode::OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  default:
    std::cout << "Unknown opcode " << static_cast<uint8_t>(instruction) << "\n";
    return offset + 1;
  }
}
