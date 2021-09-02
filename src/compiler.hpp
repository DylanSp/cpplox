#pragma once

#include <string_view>

namespace lox {
class Compiler {
private:
public:
  void compile(std::string_view source);
};

} // namespace lox
