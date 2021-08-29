#include "hello.hpp"
#include <iostream>
#include <vector>

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

  int arr[] = {1, 2, 3};

  for (int i = 0; i < 3; i++)
  {
    std::cout << arr[i];
  }

  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  for (int i = 0; i < v.size(); i++)
  {
    std::cout << v[i];
  }

  return 0;
}