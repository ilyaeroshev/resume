#pragma once

#include <condition_variable>
#include <mutex>
#include <src/utility/intrusive_forward_list.hpp>

// Intrusive Multi Producer Multi Consumer Queue

template <typename T>
class IntrusiveMPMCQueue {
 public:
  void Push(T* node) {
    std::lock_guard guard(mutex_);
    list_.PushBack(node);
    if (list_.Size() == 1) {
      not_empty_.notify_all();
    }
  }

  T* Pop() {
    std::unique_lock lock(mutex_);
    while (list_.IsEmpty() && is_open_) {
      not_empty_.wait(lock);
    }

    if (list_.IsEmpty()) {
      return nullptr;
    }

    return list_.PopFront();
  }

  void Close() {
    std::lock_guard guard(mutex_);
    is_open_ = false;
    if (list_.IsEmpty()) {
      not_empty_.notify_all();
    }
  }

 private:
  IntrusiveForwardList<T> list_;  // Guarded by mutex_
  bool is_open_{true};            // Guarded by mutex_
  std::mutex mutex_;
  std::condition_variable not_empty_;
};