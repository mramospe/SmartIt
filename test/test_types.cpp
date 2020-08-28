#include "smartit/test.hpp"
#include "smartit/types.hpp"

/// Test function for "single_value"
template <class Type> void test_single_value() {

  smit::test::single_value<Type> v;
  v.value() = 10;
  SMARTIT_TEST_ASSERT(v.value, 10);
  auto &_v = v.value();
  _v = 0;
  SMARTIT_TEST_ASSERT(v.value, 0);
}

/// Test function for "two_single_values"
template <class Type> void test_two_single_values() {

  smit::test::two_single_values<Type> tv;
  tv.first().value() = +10;
  tv.second().value() = -10;

  auto &f = tv.first();
  auto &s = tv.second();

  SMARTIT_TEST_ASSERT(f.value, +10);
  SMARTIT_TEST_ASSERT(s.value, -10);
}

template <class XYZ> void set_xyz_and_compare(XYZ &v) {

  v.x() = 1;
  v.y() = 1;
  v.z() = 1;

  SMARTIT_TEST_ASSERT(v.x, 1);
  SMARTIT_TEST_ASSERT(v.y, 1);
  SMARTIT_TEST_ASSERT(v.z, 1);
}

template <class Type> void test_point_3d() {

  smit::point_3d<Type> t;
  set_xyz_and_compare(t);

  smit::point_3d<Type> a{1, 0, 0};
  smit::point_3d<Type> b{0, 1, 0};

  const auto c = smit::cross(a, b);

  SMARTIT_TEST_ASSERT(c.x, 0);
  SMARTIT_TEST_ASSERT(c.y, 0);
  SMARTIT_TEST_ASSERT(c.z, 1);
}

template <class Type> void test_point_with_vector_3d() {

  smit::point_with_vector_3d<Type> t;

  set_xyz_and_compare(t.point());
  // set_xyz_and_compare(t.vector());
}

int main() {

  smit::test::test_collector coll("set-and-compare");

  // Test the "single_value" class
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_single_value<int>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_single_value<float>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_single_value<double>);

  // Test the "two_single_values" class
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_two_single_values<int>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_two_single_values<float>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_two_single_values<double>);

  // Set X Y Z for smit::point_3d
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_3d<int>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_3d<float>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_3d<double>);

  // Set the point and vector and compare for smit::point_with_vector_3d
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_with_vector_3d<int>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_with_vector_3d<float>);
  SMARTIT_TEST_SCOPE_FUNCTION(coll, &test_point_with_vector_3d<double>);

  return coll.status();
}
