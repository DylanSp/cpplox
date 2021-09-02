#include "compiler.hpp"
#include "scanner.hpp"

using lox::Compiler;

void Compiler::compile(std::string_view source) {
  auto scanner = Scanner(source);

  auto line = -1;

  for (;;) {
    auto token = scanner.scanToken();
  }
}