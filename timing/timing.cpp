#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "smartit/all.hpp"

template <size_t N, size_t R> void _run_array(std::ofstream &file) {

  std::cout << "- " << N << std::endl;

  float total_std = 0, total_smit = 0, total_std_2 = 0, total_smit_2 = 0;

  // STD
  {
    std::array<smit::point_3d<float>, N> a;
    for (size_t i = 0; i < R; ++i) {

      const auto start = std::clock();

      const auto e = a.end();

      for (auto it = a.begin(); it != e; ++it) {
        it->x() = 1.f;
        it->y() = 1.f;
        it->z() = 1.f;
      }

      const auto end = std::clock();

      const auto t = (end - start) * 1.f / CLOCKS_PER_SEC;

      total_std += t;
      total_std_2 += t * t;
    }
  }

  // SMIT
  {
    smit::array<smit::point_3d<float>, N> a;
    for (size_t i = 0; i < R; ++i) {

      const auto start = std::clock();

      const auto e = a.end();

      for (auto it = a.begin(); it != e; ++it) {
        it->x() = 1.f;
        it->y() = 1.f;
        it->z() = 1.f;
      }

      const auto end = std::clock();

      const auto t = (end - start) * 1.f / CLOCKS_PER_SEC;

      total_smit += t;
      total_smit_2 += t * t;
    }
  }

  float mean_std = total_std * 1.f / R;
  float var_std =
      (total_std_2 + R * mean_std * mean_std - 2.f * mean_std * total_std) *
      1.f / (R * (R - 1));
  float mean_smit = total_smit * 1.f / R;
  float var_smit = (total_smit_2 + R * mean_smit * mean_smit -
                    2.f * mean_smit * total_smit) *
                   1.f / (R * (R - 1));

  file << N << " " << mean_std << " " << var_std << " " << mean_smit << " "
       << var_smit << std::endl;
}

void run_array(std::string path) {

  std::ofstream file;

  file.open(path + "/array.txt");

  _run_array<100, 1000>(file);
  _run_array<1000, 1000>(file);
  _run_array<10000, 1000>(file);
  _run_array<100000, 1000>(file);

  file.close();
}

template <size_t N, size_t R> void _run_vector(std::ofstream &file) {

  std::cout << "- " << N << std::endl;

  float total_std = 0, total_smit = 0, total_std_2 = 0, total_smit_2 = 0;

  // STD
  {
    std::vector<smit::point_3d<float>> a(N);
    for (size_t i = 0; i < R; ++i) {

      const auto start = std::clock();

      const auto e = a.end();

      for (auto it = a.begin(); it != e; ++it) {
        it->x() = 1.f;
        it->y() = 1.f;
        it->z() = 1.f;
      }

      const auto end = std::clock();

      const auto t = (end - start) * 1.f / CLOCKS_PER_SEC;

      total_std += t;
      total_std_2 += t * t;
    }
  }

  // SMIT
  {
    smit::vector<smit::point_3d<float>> a(N);
    for (size_t i = 0; i < R; ++i) {

      const auto start = std::clock();

      const auto e = a.end();

      for (auto it = a.begin(); it != e; ++it) {
        it->x() = 1.f;
        it->y() = 1.f;
        it->z() = 1.f;
      }

      const auto end = std::clock();

      const auto t = (end - start) * 1.f / CLOCKS_PER_SEC;

      total_smit += t;
      total_smit_2 += t * t;
    }
  }

  float mean_std = total_std * 1.f / R;
  float var_std =
      (total_std_2 + R * mean_std * mean_std - 2.f * mean_std * total_std) *
      1.f / (R * (R - 1));
  float mean_smit = total_smit * 1.f / R;
  float var_smit = (total_smit_2 + R * mean_smit * mean_smit -
                    2.f * mean_smit * total_smit) *
                   1.f / (R * (R - 1));

  file << N << " " << mean_std << " " << var_std << " " << mean_smit << " "
       << var_smit << std::endl;
}

void run_vector(std::string path) {

  std::ofstream file;

  file.open(path + "/vector.txt");

  _run_vector<100, 1000>(file);
  _run_vector<1000, 1000>(file);
  _run_vector<10000, 1000>(file);
  _run_vector<100000, 1000>(file);
  _run_vector<1000000, 1000>(file);

  file.close();
}

int main(int argc, char *argv[]) {

  if (argc > 2) {
    std::cerr << "Only one argument is allowed (output directory)" << std::endl;
    return 1;
  }

  std::string path = argc == 2 ? argv[1] : "./";

  std::cout << "Run for array" << std::endl;
  run_array(path);
  std::cout << "Run for vector" << std::endl;
  run_vector(path);

  return 0;
}
