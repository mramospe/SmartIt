#include <chrono>
#include <iomanip>
#include <iostream>

#include "smartit/array.hpp"
#include "smartit/test.hpp"
#include "smartit/types.hpp"
#include "smartit/vector.hpp"

template <size_t N, size_t R> void test_array() {

  std::cout << "Measuring time for " << N << " entries and " << R
            << " repetitions" << std::endl;

  const auto start_std = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    std::array<float, N> vx, vy, vz;

    auto it_x = vx.begin();
    auto it_y = vy.begin();
    auto it_z = vz.begin();

    for (size_t j = 0; j < N; ++j) {
      ++it_x;
      ++it_y;
      ++it_z;
    }
  }
  const auto end_std = std::chrono::high_resolution_clock::now();

  auto start_smit = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    smit::array<smit::point_3d<float>, N> v;

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      ++it;
  }
  const auto end_smit = std::chrono::high_resolution_clock::now();

  // Time using the standard library
  auto std_t = (end_std - start_std).count();
  std::cout << "- std:   " << std_t << std::endl;
  // Time using SmartIt
  auto smit_t = (end_smit - start_smit).count();
  std::cout << "- smit:  " << smit_t << std::endl;
  // Ratio
  std::cout << "- ratio: " << std::fixed << std::setprecision(2)
            << (smit_t * 1. / std_t) << std::endl;
}

template <size_t N, size_t R> void test_vector() {

  std::cout << "Start measuring time for " << N << " entries and " << R
            << " repetitions" << std::endl;

  const auto start_std = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    std::vector<float> vx(N), vy(N), vz(N);

    auto it_x = vx.begin();
    auto it_y = vy.begin();
    auto it_z = vz.begin();

    for (size_t j = 0; j < N; ++j) {
      ++it_x;
      ++it_y;
      ++it_z;
    }
  }
  const auto end_std = std::chrono::high_resolution_clock::now();

  auto start_smit = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < R; ++i) {

    smit::vector<smit::point_3d<float>> v(N);

    auto it = v.begin();

    for (size_t j = 0; j < N; ++j)
      ++it;
  }
  const auto end_smit = std::chrono::high_resolution_clock::now();

  // Time using the standard library
  auto std_t = (end_std - start_std).count();
  std::cout << "- std:   " << std_t << std::endl;
  // Time using SmartIt
  auto smit_t = (end_smit - start_smit).count();
  std::cout << "- smit:  " << smit_t << std::endl;
  // Ratio
  std::cout << "- ratio: " << std::fixed << std::setprecision(2)
            << (smit_t * 1. / std_t) << std::endl;
}

int main() {

  smit::test::test_collector coll("test-timing");

  SMARTIT_TEST_SCOPE_FUNCTION(coll, (&test_vector<100, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll, (&test_vector<1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll, (&test_vector<10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll, (&test_vector<100000, 10000>));

  SMARTIT_TEST_SCOPE_FUNCTION(coll, (&test_array<100, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll, (&test_array<1000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll, (&test_array<10000, 10000>));
  SMARTIT_TEST_SCOPE_FUNCTION(coll, (&test_array<100000, 10000>));

  return 0;
}
