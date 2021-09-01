#include "chunk.hpp"
#include "vm.hpp"

int main(int argc, const char *argv[]) {
  lox::VM vm;

  lox::Chunk chunk;

  auto constantAddress = chunk.addConstant(1.2);
  auto sampleLineNumber = 123;
  chunk.write(lox::OpCode::OP_CONSTANT, sampleLineNumber);
  chunk.write(constantAddress, sampleLineNumber);

  chunk.write(lox::OpCode::OP_RETURN, sampleLineNumber);

  chunk.disassemble("test chunk");

  vm.interpret(chunk);

  return 0;
}