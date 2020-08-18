#include "basic_types.hpp"
#include "vector.hpp"
#include <iostream>

int main() {

  si::point3d<double> a{1, 2, 3};

  std::cout << a.x() << std::endl;

  si::vector<si::point3d<double>> va(10);

  std::cout << va.size() << std::endl;

  auto it = va.begin();
  auto end = va.end();

  it->x() = 1000;
  std::cout << "Should be equal" << std::endl;
  std::cout << it->x() << std::endl;
  std::cout << va.begin()->x() << std::endl;
  std::cout << "end check" << std::endl;

  (++it)->x() = 99;
  --it;
  it++;
  it--;

  it == end;

  std::cout << "Start printing" << std::endl;
  for ( auto it = va.begin(); it != va.end(); ++it )
    std::cout << it->x() << std::endl;
  std::cout << "End printing" << std::endl;

  auto c = *it;

  std::cout << c.y() << std::endl;

  return 0;
}
