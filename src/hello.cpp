#include "hello.hpp"
#include <iostream>

void hello::hello_world()
{
#ifdef DEBUG
  std::cout << "DEBUG defined!\n";
#else
  std::cout << "DEBUG undefined!\n";
#endif

  std::cout << "Hello!\n";
}

int main()
{
  hello::hello_world();

  return 0;
}