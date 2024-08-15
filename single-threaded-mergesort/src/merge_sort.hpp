#pragma once

#include <functional>
#include <queue>
#include <vector>

template <typename ForwardIt, typename Compare>
void MergeSort(ForwardIt begin, ForwardIt end, Compare cmp) {
  using Data = std::remove_reference_t<decltype(*begin)>;

  if (begin == end) return;

  size_t sz = 0;

  std::queue<std::pair<ForwardIt, ForwardIt>> proc;  // tasks of the same size

  for (auto it = begin; it != end;) {
    auto prev = it++;
    proc.emplace(prev, it);
    ++sz;
  }

  std::vector<Data> buffer;
  buffer.reserve(sz);

  while (proc.size() > 1) {
    std::queue<std::pair<ForwardIt, ForwardIt>> next_proc;
    while (proc.size() > 1) {
      // Get two segmets
      auto [lhs_begin, lhs_end] = proc.front();
      proc.pop();
      auto [rhs_begin, rhs_end] = proc.front();
      proc.pop();

      // Merge to buffer
      {
        auto lhs_it = lhs_begin;
        auto rhs_it = rhs_begin;

        while (lhs_it != lhs_end && rhs_it != rhs_end) {
          buffer.push_back(
              std::move(cmp(*rhs_it, *lhs_it) ? *rhs_it++ : *lhs_it++));
        }

        while (lhs_it != lhs_end) {
          buffer.push_back(std::move(*lhs_it++));
        }

        while (rhs_it != rhs_end) {
          buffer.push_back(std::move(*rhs_it++));
        }
      }

      // Move from buffer
      {
        auto it_buffer = buffer.begin();
        auto it_sequence = lhs_begin;

        while (it_buffer != buffer.end()) {
          *it_sequence = std::move(*it_buffer);
          ++it_buffer;
          ++it_sequence;
        }
        buffer.clear();
      }

      next_proc.emplace(lhs_begin, rhs_end);
    }

    if (!proc.empty()) {
      next_proc.push(proc.front());
    }

    proc = std::move(next_proc);
  }
}

template <typename ForwardIt>
void MergeSort(ForwardIt begin, ForwardIt end) {
  using Data = std::remove_reference_t<decltype(*begin)>;

  MergeSort(begin, end, std::less<Data>());
}