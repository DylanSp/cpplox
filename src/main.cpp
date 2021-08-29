#include "chunk.hpp"

int main(int argc, const char *argv[])
{
  lox::Chunk chunk;
  chunk.write(lox::OpCode::OP_RETURN);
  chunk.disassemble("test chunk");

  return 0;
}