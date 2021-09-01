#pragma once

#include <functional>
#include <iostream>
#include <variant>

namespace lox {
using Value = std::variant<double>;

void printValue(Value val);
Value negateValue(Value val);

std::function<Value(Value, Value)>
createArithmeticBinaryOp(const std::function<double(double, double)> &);
Value addValues(Value lhs, Value rhs);
Value subtractValues(Value lhs, Value rhs);
Value multiplyValues(Value lhs, Value rhs);
Value divideValues(Value lhs, Value rhs);
} // namespace lox