#include "chunk.hpp"
#include "vm.hpp"

int main(int argc, const char *argv[]) {
  lox::VM vm;

  lox::Chunk chunk;

  auto sampleLineNumber = 123;

  auto constantAddress12 = chunk.addConstant(1.2);
  chunk.write(lox::OpCode::OP_CONSTANT, sampleLineNumber);
  chunk.write(constantAddress12, sampleLineNumber);

  auto constantAddress34 = chunk.addConstant(3.4);
  chunk.write(lox::OpCode::OP_CONSTANT, sampleLineNumber);
  chunk.write(constantAddress34, sampleLineNumber);

  chunk.write(lox::OpCode::OP_ADD, sampleLineNumber);

  auto constantAddress56 = chunk.addConstant(5.6);
  chunk.write(lox::OpCode::OP_CONSTANT, sampleLineNumber);
  chunk.write(constantAddress56, sampleLineNumber);

  chunk.write(lox::OpCode::OP_DIVIDE, sampleLineNumber);

  chunk.write(lox::OpCode::OP_NEGATE, sampleLineNumber);

  chunk.write(lox::OpCode::OP_RETURN, sampleLineNumber);

  chunk.disassemble("test chunk");

  vm.interpret(chunk);

  return 0;
}