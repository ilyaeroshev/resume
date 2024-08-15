#include <functional>
#include <iostream>
#include <list>
#include <src/merge_sort.hpp>
#include <vector>

#include "utility.hpp"

#define all(obj) obj.begin(), obj.end()

void Test1() {
  std::vector data{5, 7, 4, 2, 8, 6, 1, 7, 0, 3};

  MergeSort(all(data));

  Check(all(data));

  std::cerr << "Test #1 is passed!" << std::endl;
}

void Test2() {
  std::vector<int> data{};

  MergeSort(all(data));

  Check(all(data));

  std::cerr << "Test #2 is passed!" << std::endl;
}

void Test3() {
  std::vector data{0};

  MergeSort(all(data));

  Check(all(data));

  std::cerr << "Test #3 is passed!" << std::endl;
}

void Test4() {
  std::list data{5, 7, 4, 2, 8, 6, 1, 7, 0, 3};

  MergeSort(all(data));

  Check(all(data));

  std::cerr << "Test #4 is passed!" << std::endl;
}

void Test5() {
  std::vector data{5, 7, 4, 2, 8, 6, 1, 7, 0, 3};

  MergeSort(all(data), std::greater<>());

  Check(all(data), std::greater<>());

  std::cerr << "Test #5 is passed!" << std::endl;
}

void Test6() {
  std::vector data{5, 7, 4, 2, 8, 6, 1, 7, 0, 3};

  ThreadPool tp(4);

  tp.Start();

  MergeSort(all(data), static_cast<IScheduler&>(tp));

  tp.Stop();

  Check(all(data));

  std::cerr << "Test #6 is passed!" << std::endl;
}

void Test7() {
  std::vector data{5, 7, 4, 2, 8, 6, 1, 7, 0, 3};

  ThreadPool tp(4);

  tp.Start();

  MergeSort(all(data), std::greater<>(), tp);

  tp.Stop();

  Check(all(data), std::greater<>());

  std::cerr << "Test #7 is passed!" << std::endl;
}

int main() {
  Test1();
  Test2();
  Test3();
  Test4();
  Test5();
  Test6();
  Test7();

  return 0;
}