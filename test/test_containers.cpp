#include "smartit/array.hpp"
#include "smartit/test.hpp"
#include "smartit/types.hpp"
#include "smartit/vector.hpp"

template <class Type, class Container>
void test_container(Container &a, size_t initial_size) {

  SMARTIT_TEST_ASSERT(a.size, initial_size);

  smit::test::single_value<Type> v{0};

  for (auto it = a.begin(); it != a.end(); ++it)
    it->value() = v.value();

  for (auto it = a.begin(); it != a.end(); ++it)
    SMARTIT_TEST_ASSERT(it->value, 0);

  for (auto it = a.cbegin(); it != a.cend(); ++it)
    SMARTIT_TEST_ASSERT(it->value, 0);

  SMARTIT_TEST_ASSERT(a.begin, a.begin());

  auto distance = [&a]() { return a.begin() - a.begin(); };

  SMARTIT_TEST_ASSERT(distance, 0);

  a.at(0);

  a[0];
}

template <typename Type> void test_array() {

  smit::array<smit::test::single_value<Type>, 10> a;

  test_container<Type>(a, 10);
}

template <typename Type> void test_vector() {

  smit::vector<smit::test::single_value<Type>> a(10);

  test_container<Type>(a, 10);

  a.resize(20);
  SMARTIT_TEST_ASSERT(a.size, 20);

  a.reserve(40);
  SMARTIT_TEST_ASSERT(a.size, 20);
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

  return smit::test::combined_status(vcoll.status(), vcoll.status());
}
