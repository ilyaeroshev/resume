#include <iostream>
#include <src/merge_sort.hpp>
#include <vector>

#include "utility.hpp"

#define all(obj) obj.begin(), obj.end()

void Test1() {
  size_t seed = std::time(nullptr);
  std::cerr << "Test #1 seed = " << seed << '\n';
  std::srand(seed);

  ThreadPool tp(3);

  tp.Start();

  const size_t TESTS_NUMBER = 5;

  for (size_t test = 0; test < TESTS_NUMBER; ++test) {
    const size_t N = 1'000'000;

    auto data = GetRandomVector(N);

    MergeSort(all(data), static_cast<IScheduler&>(tp));

    Check(all(data));
  }

  tp.Stop();

  std::cerr << "Test #1 is passed!" << std::endl;
}

int main() {
  Test1();

  return 0;
}