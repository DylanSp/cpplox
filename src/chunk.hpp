#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace lox
{
  enum class OpCode : uint8_t
  {
    OP_RETURN
  };

  class Chunk
  {
  private:
    std::vector<OpCode> code;

  public:
    void write(OpCode opcode);

    // debugging functionality
    void disassemble(std::string chunkName);
    int disassembleInstruction(int offset);
  };

}