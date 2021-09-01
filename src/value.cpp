#include "value.hpp"
#include <functional>

template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts> overload(Ts...) -> overload<Ts...>;

struct ValuePrinter {
  void operator()(double d) { std::cout << d; }
};

void lox::printValue(Value val) { std::visit(ValuePrinter{}, val); }

struct ValueNegator {
  double operator()(double d) { return -1 * d; }
};

lox::Value lox::negateValue(Value val) {
  return std::visit(ValueNegator{}, val);
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
