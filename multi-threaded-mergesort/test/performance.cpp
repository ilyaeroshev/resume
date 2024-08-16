#include <iostream>
#include <src/merge_sort.hpp>

#include "utility.hpp"

#define all(obj) obj.begin(), obj.end()

void Test1() {
  size_t seed = std::time(nullptr);
  std::cerr << "Test #1 seed = " << seed << '\n';
  std::srand(seed);

  size_t workers = std::max(1u, std::thread::hardware_concurrency());
  std::cerr << "Test #1 workers = " << workers << '\n';
  ThreadPool tp(workers);

  tp.Start();

  const size_t TESTS_NUMBER = 5;

  std::chrono::milliseconds all_time{0};

  for (size_t test = 0; test < TESTS_NUMBER; ++test) {
    const size_t N = 10'000'000;

    auto data = GetRandomVector(N);

    Timer timer;
    MergeSort(all(data), static_cast<IScheduler&>(tp));
    all_time += timer.Spent<std::chrono::milliseconds>();
  }

  tp.Stop();

  std::cerr << "Test #1 was completed in " << all_time << std::endl;
}

void Test2() {
  size_t seed = std::time(nullptr);
  std::cerr << "Test #2 seed = " << seed << '\n';
  std::srand(seed);

  size_t workers = std::max(1u, std::thread::hardware_concurrency());
  std::cerr << "Test #2 workers = " << workers << '\n';
  ThreadPool tp(workers);

  tp.Start();

  const size_t TESTS_NUMBER = 5;

  std::chrono::milliseconds all_time{0};

  for (size_t test = 0; test < TESTS_NUMBER; ++test) {
    const size_t VSZ = 1'000'000;
    const size_t SSZ = 1000;

    auto data = GetRandomStringVector(VSZ, SSZ);

    Timer timer;
    MergeSort(all(data), static_cast<IScheduler&>(tp));
    all_time += timer.Spent<std::chrono::milliseconds>();
  }

  tp.Stop();

  std::cerr << "Test #2 was completed in " << all_time << std::endl;
}

int main() {
  Test1();
  Test2();

  return 0;
}