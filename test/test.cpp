#include "basic_types.hpp"
#include "vector.hpp"
#include <iostream>

int main() {

  smit::point_3d<double> a{1, 2, 3};

  std::cout << a.x() << std::endl;

  smit::vector<smit::point_3d<double>> va(10);

  std::cout << va.size() << std::endl;

  auto it = va.begin();
  auto end = va.end();

  std::cout << "Should be equal" << std::endl;
  it->x() = 1000;
  (*it).x() = 1000;
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

  std::cout << c.x() << " " << c.y() << " " << c.z() << std::endl;

  std::cout << smit::dot(c, c) << std::endl;

  auto r = smit::cross(c, c);

  std::cout << r.x() << " " << r.y() << " " << r.z() << std::endl;

  smit::point_with_vector_3d<float> pv;

  pv.point().x() = 2347;

  std::cout << pv.point().x() << std::endl;

  smit::vector<smit::point_with_vector_3d<float>> v_pv(10);

  smit::vector<smit::point_with_vector_3d<float>>::iterator i;
  i = v_pv.begin();

  auto& cont = v_pv.begin()->point();
  std::cout << "HERE" << std::endl;
  cont.x() = 10;
  *(std::get<0>(cont.m_iter)) = 10;
  std::cout << *(std::get<0>(cont.m_iter)) << std::endl;
  std::cout << cont.x() << std::endl;

  return 0;

  for ( auto it = v_pv.begin(); it != v_pv.end(); ++it )
    it->point().x() = 11;

  for ( auto it = v_pv.begin(); it != v_pv.end(); ++it )
    std::cout << it->point().x() << std::endl;

  return 0;
}
