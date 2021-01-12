#include <chrono>
#include <iomanip>
#include <iostream>

#include "smartit/array.hpp"
#include "smartit/test.hpp"
#include "smartit/types.hpp"
#include "smartit/vector.hpp"

struct iterate {
  template <class... Iterator> static void call_std_raw(Iterator &... it) {
    (++it, ...);
  }
  template <class Iterator> static void call_std_obj(Iterator &it) { ++it; }

  template <class Iterator> static void call_smit(Iterator &it) { ++it; }
};

struct reset {
  template <class... Iterator> static void call_std_raw(Iterator &... it) {
    (reset_it(it++), ...);
  }
  template <class Iterator> static void call_std_obj(Iterator &it) {
    reset_it(it++);
  }

  template <class Iterator> static void call_smit(Iterator &it) {
    reset_it(it++);
  }
  template <class Iterator> static void reset_it(Iterator it) { *it = *it; }
};

template <class Action, size_t N, size_t R> void test_array() {

  std::cout << "Measuring time for " << N << " entries and " << R
            << " repetitions" << std::endl;

  const auto start_std_raw = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    std::array<float, N> vx, vy, vz;

    auto it_x = vx.begin();
    auto it_y = vy.begin();
    auto it_z = vz.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_std_raw(it_x, it_y, it_z);
  }
  const auto end_std_raw = std::chrono::high_resolution_clock::now();

  const auto start_std_obj = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    std::array<smit::point_3d<float>, N> v;

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_std_obj(it);
  }
  const auto end_std_obj = std::chrono::high_resolution_clock::now();

  auto start_smit = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    smit::array<smit::point_3d<float>, N> v;

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_smit(it);
  }
  const auto end_smit = std::chrono::high_resolution_clock::now();

  // Time using the standard library
  auto std_raw_t = (end_std_raw - start_std_raw).count();
  std::cout << "- std (raw): " << std_raw_t << std::endl;
  // Time using the standard library with the value type
  auto std_obj_t = (end_std_obj - start_std_obj).count();
  std::cout << "- std (obj): " << std_obj_t << std::endl;
  // Time using SmartIt
  auto smit_t = (end_smit - start_smit).count();
  std::cout << "- smit:      " << smit_t << std::endl;
  // Ratio
  std::cout << "- ratio:     " << std::fixed << std::setprecision(2)
            << (std_raw_t < std_obj_t ? std_raw_t : std_obj_t) << std::endl;
}

template <class Action, size_t N, size_t R> void test_vector() {

  std::cout << "Start measuring time for " << N << " entries and " << R
            << " repetitions" << std::endl;

  const auto start_std_raw = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    std::vector<float> vx(N), vy(N), vz(N);

    auto it_x = vx.begin();
    auto it_y = vy.begin();
    auto it_z = vz.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_std_raw(it_x, it_y, it_z);
  }
  const auto end_std_raw = std::chrono::high_resolution_clock::now();

  const auto start_std_obj = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    std::vector<smit::point_3d<float>> v(N);

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_std_obj(it);
  }
  const auto end_std_obj = std::chrono::high_resolution_clock::now();

  auto start_smit = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    smit::vector<smit::point_3d<float>> v(N);

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      Action::call_smit(it);
  }
  const auto end_smit = std::chrono::high_resolution_clock::now();

  // Time using the standard library
  auto std_raw_t = (end_std_raw - start_std_raw).count();
  std::cout << "- std (raw): " << std_raw_t << std::endl;
  // Time using the standard library with the value type
  auto std_obj_t = (end_std_obj - start_std_obj).count();
  std::cout << "- std (obj): " << std_obj_t << std::endl;
  // Time using SmartIt
  auto smit_t = (end_smit - start_smit).count();
  std::cout << "- smit:      " << smit_t << std::endl;
  // Ratio
  std::cout << "- ratio:     " << std::fixed << std::setprecision(2)
            << (smit_t * 1. / (std_raw_t < std_obj_t ? std_raw_t : std_obj_t))
            << std::endl;
}

int main() {

  smit::test::test_collector coll_iterate("test-timing-iterate");

  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_vector<iterate, 100, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_vector<iterate, 1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_vector<iterate, 10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_vector<iterate, 100000, 10000>));

  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_array<iterate, 1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_array<iterate, 10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_array<iterate, 100000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_iterate,
                              (&test_array<iterate, 1000000, 10000>));

  smit::test::test_collector coll_reset("test-timing-reset");

  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset, (&test_vector<reset, 100, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset, (&test_vector<reset, 1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset, (&test_vector<reset, 10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset, (&test_vector<reset, 100000, 10000>));

  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset, (&test_array<reset, 1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset, (&test_array<reset, 10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset, (&test_array<reset, 100000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll_reset, (&test_array<reset, 1000000, 10000>));

  return 0;
}
