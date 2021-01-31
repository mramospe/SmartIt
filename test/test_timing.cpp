#include <chrono>
#include <iomanip>
#include <iostream>

#include "smartit/array.hpp"
#include "smartit/test.hpp"
#include "smartit/types.hpp"
#include "smartit/vector.hpp"

struct iterate {
  template <class Iterator> static void call_std(Iterator &it) { ++it; }

  template <class Iterator> static void call_smit(Iterator &it) { ++it; }
};

struct reset {
  template <class Iterator> static void call_std(Iterator &it) {
    reset_it(it++);
  }

  template <class Iterator> static void call_smit(Iterator &it) {
    reset_it(it++);
  }
  template <class Iterator> static void reset_it(Iterator it) { *it = *it; }
};

template <class Type, class Action, size_t N, size_t R> void test_array() {

  std::cout << "Measuring time for " << N << " entries and " << R
            << " repetitions" << std::endl;

  const auto start_std = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    std::array<Type, N> v;

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_std(it);
  }
  const auto end_std = std::chrono::high_resolution_clock::now();

  auto start_smit = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    smit::array<Type, N> v;

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_smit(it);
  }
  const auto end_smit = std::chrono::high_resolution_clock::now();

  // Time using the standard library with the value type
  auto std_t = (end_std - start_std).count();
  std::cout << "- std:   " << std_t << std::endl;
  // Time using SmartIt
  auto smit_t = (end_smit - start_smit).count();
  std::cout << "- smit:  " << smit_t << std::endl;
  // Ratio
  std::cout << "- ratio: " << std::fixed << std::setprecision(2)
            << smit_t * 1. / std_t << std::endl;
}

template <class Type, class Action, size_t N, size_t R> void test_vector() {

  std::cout << "Start measuring time for " << N << " entries and " << R
            << " repetitions" << std::endl;

  const auto start_std = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    std::vector<Type> v(N);

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_std(it);
  }
  const auto end_std = std::chrono::high_resolution_clock::now();

  auto start_smit = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    smit::vector<Type> v(N);

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_smit(it);
  }
  const auto end_smit = std::chrono::high_resolution_clock::now();

  // Time using the standard library with the value type
  auto std_t = (end_std - start_std).count();
  std::cout << "- std:   " << std_t << std::endl;
  // Time using SmartIt
  auto smit_t = (end_smit - start_smit).count();
  std::cout << "- smit:  " << smit_t << std::endl;
  // Ratio
  std::cout << "- ratio: " << std::fixed << std::setprecision(2)
            << smit_t * 1. / std_t << std::endl;
}

template <class Type> void test_timing(std::string const &name) {

  smit::test::test_collector coll_iterate(std::string{"iterate-"} + name);

  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_vector<Type, iterate, 100, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_vector<Type, iterate, 1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_vector<Type, iterate, 10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_vector<Type, iterate, 100000, 10000>));

  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_array<Type, iterate, 1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_array<Type, iterate, 10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_array<Type, iterate, 100000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_array<Type, iterate, 1000000, 10000>));

  smit::test::test_collector coll_reset(std::string{"reset-"} + name);

  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset,
                              (&test_vector<Type, reset, 100, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset,
                              (&test_vector<Type, reset, 1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset,
                              (&test_vector<Type, reset, 10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset,
                              (&test_vector<Type, reset, 100000, 10000>));

  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset,
                              (&test_array<Type, reset, 1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset,
                              (&test_array<Type, reset, 10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset,
                              (&test_array<Type, reset, 100000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset,
                              (&test_array<Type, reset, 1000000, 10000>));
}

int main() {

  test_timing<smit::point_3d<float>>("point-3d");
  test_timing<smit::test::two_single_values<float>>("two-single-values");
  test_timing<smit::point_with_vector_3d<float>>("point-with-vector-3d");

  return 0;
}
