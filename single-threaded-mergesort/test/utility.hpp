#pragma once

#include <cassert>
#include <chrono>
#include <functional>
#include <type_traits>
#include <vector>

template <typename ForwardIt, typename Compare>
void Check(ForwardIt begin, ForwardIt end, Compare cmp) {
  using Data = std::remove_reference_t<decltype(*begin)>;

  if (begin == end) return;

  auto equiv = [&cmp](const Data& lhs, const Data& rhs) {
    return !cmp(lhs, rhs) && !cmp(rhs, lhs);
  };

  auto it = begin;
  auto prev_it = it++;

  while (it != end) {
    assert(cmp(*prev_it, *it) || equiv(*prev_it, *it));
    prev_it = it++;
  }
}

template <typename ForwardIt>
void Check(ForwardIt begin, ForwardIt end) {
  using Data = std::remove_reference_t<decltype(*begin)>;

  Check(begin, end, std::less<Data>());
}

std::vector<int> GetRandomVector(size_t N) {
  std::vector<int> res;
  res.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    res.push_back(std::rand() % 1'000'000 - 500'000);
  }

  return res;
}

class Timer {
  using Clock = std::chrono::steady_clock;

 public:
  Timer() : begin_(Clock::now()) {}

  template <typename Duration>
  Duration Spent() const {
    return std::chrono::duration_cast<Duration>(Clock::now() - begin_);
  }

 private:
  decltype(Clock::now()) begin_;
};
