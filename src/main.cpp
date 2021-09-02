#include "vm.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

void repl() {
  lox::VM vm;

  std::cout << "> ";
  std::string inputLine;
  while (std::getline(std::cin, inputLine)) {
    vm.interpret(inputLine);
    std::cout << "> ";
  }

  std::cout << "\n";
}

std::string readFile(const std::string &filename) {
  std::ifstream readStream(filename);
  std::stringstream buffer;
  buffer << readStream.rdbuf();
  return buffer.str();
}

void runFile(const std::string &filename) {
  auto source = readFile(filename);

  lox::VM vm;
  auto result = vm.interpret(source);

  if (result == lox::InterpretResult::COMPILE_ERROR) {
    std::exit(65);
  }

  if (result == lox::InterpretResult::RUNTIME_ERROR) {
    std::exit(70);
  }
}

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    std::cerr << "Usage: clox [path]\n";
    exit(64);
  }

  return 0;
}
