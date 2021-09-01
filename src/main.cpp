#include "chunk.hpp"

int main(int argc, const char *argv[]) {
  lox::Chunk chunk;

  auto constantAddress = chunk.addConstant(1.2);
  chunk.write(lox::OpCode::OP_CONSTANT);
  chunk.write(constantAddress);

  chunk.write(lox::OpCode::OP_RETURN);
  
  chunk.disassemble("test chunk");

  return 0;
}