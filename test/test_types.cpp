#include "smartit/types.hpp"
#include "smartit/test.hpp"


template<class XYZ>
void set_xyz_and_compare(XYZ& v) {

  v.x() = 1;
  v.y() = 1;
  v.z() = 1;

  SMARTIT_TEST_ASSERT(v.x, 1);
  SMARTIT_TEST_ASSERT(v.y, 1);
  SMARTIT_TEST_ASSERT(v.z, 1);
}

template<class Type>
void test_point_3d() {

  smit::point_3d<Type> t;
  set_xyz_and_compare(t);
}

template<class Type>
void test_point_with_vector_3d() {

  smit::point_with_vector_3d<Type> t;

  set_xyz_and_compare(t.point());
  //set_xyz_and_compare(t.vector());
}

int main() {

  smit::core::test_collector coll("set-and-compare");

  // Set X Y Z for smit::point_3d
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_3d<int>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_3d<float>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_3d<double>);
/*
  // Set the point and vector and compare for smit::point_with_vector_3d
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_with_vector_3d<int>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_with_vector_3d<float>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_with_vector_3d<double>);
*/
  return coll.status();
}
