#include "basic_types.hpp"
#include "vector.hpp"
#include <iostream>

int main() {

  si::point3d<double> a{1, 2, 3};

  std::cout << a.x() << std::endl;

  si::vector<si::point3d<double>> va(100);

  std::cout << va.size() << std::endl;

  auto it = va.begin();
  auto end = va.end();

  it->x() = 1000;

  std::cout << it->x() << std::endl;

  ++it;
  --it;
  it++;
  it--;

  it == end;

  for ( auto it = va.begin(); it != va.end(); ++it );

  return 0;
}
