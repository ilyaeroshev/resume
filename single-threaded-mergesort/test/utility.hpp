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
  res.resize(N);

  for (int& t: res) {
    t = std::rand() % 1'000'000 - 500'000;
  }

  return res;
}

std::string GetRandomString(size_t N) {
  std::string res;
  res.resize(N);

  for(char& c: res) {
    c = 'a' + std::rand() % 26;
  }

  return res;
}

std::vector<std::string> GetRandomStringVector(size_t VSZ, size_t SSZ) {
  std::vector<std::string> res(VSZ);

  const size_t RANDOM_STRINGS = 1000;

  for(size_t i = 0; i < std::min(RANDOM_STRINGS, VSZ); ++i) {
    res[i] = GetRandomString(SSZ);
  }

  for(size_t i = RANDOM_STRINGS; i < VSZ; ++i) {
    res[i] = res[std::rand() % RANDOM_STRINGS];
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
