#pragma once
#include <iostream>
#include <variant>

namespace lox {
using Value = std::variant<double>;

void printValue(Value val);
} // namespace lox