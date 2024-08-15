#include <iostream>
#include <src/merge_sort.hpp>
#include <vector>

#include "utility.hpp"

#define all(obj) obj.begin(), obj.end()

void Test1() {
  size_t seed = std::time(nullptr);
  std::cerr << "Test #1 seed = " << seed << '\n';
  std::srand(seed);

  size_t workers = std::max(1u, std::thread::hardware_concurrency());
  ThreadPool tp(workers);

  tp.Start();

  for (size_t n = 0; n <= 256; ++n) {
    auto data = GetRandomVector(n);

    MergeSort(all(data), static_cast<IScheduler&>(tp));

    Check(all(data));
  }

  tp.Stop();

  std::cerr << "Test #1 is passed!" << std::endl;
}

void Test2() {
  size_t seed = std::time(nullptr);
  std::cerr << "Test #2 seed = " << seed << '\n';
  std::srand(seed);

  size_t workers = std::max(1u, std::thread::hardware_concurrency());
  ThreadPool tp(workers);

  tp.Start();

  const size_t TESTS_NUMBER = 100;

  for (size_t test = 0; test < TESTS_NUMBER; ++test) {
    const size_t N = std::rand() % 10'000;

    auto data = GetRandomVector(N);

    MergeSort(all(data), static_cast<IScheduler&>(tp));

    Check(all(data));
  }

  tp.Stop();

  std::cerr << "Test #2 is passed!" << std::endl;
}

int main() {
  Test1();
  Test2();

  return 0;
}