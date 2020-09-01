#include "smartit/array.hpp"
#include "smartit/test.hpp"
#include "smartit/types.hpp"
#include "smartit/vector.hpp"

template <typename Type> void test_array() {

  smit::array<smit::test::single_value<Type>, 10> a;

  SMARTIT_TEST_ASSERT(a.size, 10);

  smit::test::single_value<Type> v{0};

  for (auto it = a.begin(); it != a.end(); ++it)
    it->value() = v.value();

  for (auto it = a.begin(); it != a.end(); ++it)
    SMARTIT_TEST_ASSERT(it->value, 0);
}

template <typename Type> void test_vector() {

  smit::vector<smit::test::single_value<Type>> a(10);

  SMARTIT_TEST_ASSERT(a.size, 10);

  smit::test::single_value<Type> v{0};

  for (auto it = a.begin(); it != a.end(); ++it)
    it->value() = v.value();

  for (auto it = a.begin(); it != a.end(); ++it)
    SMARTIT_TEST_ASSERT(it->value, 0);
}

int main() {

  smit::test::test_collector acoll("test-array");
  SMARTIT_TEST_SCOPE_FUNCTION(acoll, &test_array<int>);
  SMARTIT_TEST_SCOPE_FUNCTION(acoll, &test_array<float>);
  SMARTIT_TEST_SCOPE_FUNCTION(acoll, &test_array<double>);

  smit::test::test_collector vcoll("test-vector");
  SMARTIT_TEST_SCOPE_FUNCTION(vcoll, &test_vector<int>);
  SMARTIT_TEST_SCOPE_FUNCTION(vcoll, &test_vector<float>);
  SMARTIT_TEST_SCOPE_FUNCTION(vcoll, &test_vector<double>);

  return vcoll.status() || vcoll.status();
}
