#include <iostream>
#include <src/merge_sort.hpp>

#define all(obj) obj.begin(), obj.end()

int main() {
  std::vector<int> v{7, 6, 5, 4, 3, 2, 1};

  MergeSort(all(v));

  for (auto& t : v) {
    std::cout << t << ' ';
  }
  std::cout << '\n';

  return 0;
}