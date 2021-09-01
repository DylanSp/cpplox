#include "value.hpp"
#include <functional>

// taken from https://www.cppstories.com/2018/09/visit-variants/ to allow using
// lambdas inside std::visit calls
template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts> overload(Ts...) -> overload<Ts...>;

void lox::printValue(Value val) {
  std::visit(overload{[](double d) { std::cout << d; }}, val);
}

lox::Value lox::negateValue(Value val) {
  return std::visit(overload{[](double d) { return -1 * d; }}, val);
};

std::function<lox::Value(lox::Value, lox::Value)>
lox::createArithmeticBinaryOp(const std::function<double(double, double)> &op) {
  return [=](Value lhs, Value rhs) {
    return std::visit(overload{[=](double lhDouble, double rhDouble) {
                        return op(lhDouble, rhDouble);
                      }},
                      lhs, rhs);
  };
};

lox::Value lox::addValues(Value lhs, Value rhs) {
  auto func = createArithmeticBinaryOp(
      [](double lhs, double rhs) { return lhs + rhs; });
  return func(lhs, rhs);
}

lox::Value lox::subtractValues(Value lhs, Value rhs) {
  auto func = createArithmeticBinaryOp(
      [](double lhs, double rhs) { return lhs - rhs; });
  return func(lhs, rhs);
}

lox::Value lox::multiplyValues(Value lhs, Value rhs) {
  auto func = createArithmeticBinaryOp(
      [](double lhs, double rhs) { return lhs * rhs; });
  return func(lhs, rhs);
}

lox::Value lox::divideValues(Value lhs, Value rhs) {
  auto func = createArithmeticBinaryOp(
      [](double lhs, double rhs) { return lhs / rhs; });
  return func(lhs, rhs);
}
