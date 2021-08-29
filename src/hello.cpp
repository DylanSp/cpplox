#include "hello.hpp"
#include <iostream>

void hello::hello_world()
{
  std::cout << "Hello!\n";
}

int main()
{
  hello::hello_world();

  return 0;
}