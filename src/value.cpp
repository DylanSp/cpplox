#include "value.hpp"

struct ValuePrinter {
  void operator()(double d) { std::cout << d; }
};

void lox::printValue(Value val) { std::visit(ValuePrinter{}, val); }