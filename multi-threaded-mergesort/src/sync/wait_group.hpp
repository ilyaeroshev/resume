#pragma once

#include <condition_variable>
#include <mutex>

class WaitGroup {
 public:
  void Add(size_t count) {
    std::lock_guard guard(mutex_);
    task_counter_ += count;
  }

  void Done() {
    std::lock_guard guard(mutex_);
    if (--task_counter_ == 0 && waiting_counter_ != 0) {
      zero_task_counter_.notify_all();
    }
  }

  void Wait() {
    std::unique_lock lock(mutex_);
    ++waiting_counter_;
    while (task_counter_) {
      zero_task_counter_.wait(lock);
    }
    --waiting_counter_;
  }

 private:
  size_t task_counter_{0};     // Guarded by mutex_
  size_t waiting_counter_{0};  // Guarded by mutex_
  std::mutex mutex_;
  std::condition_variable zero_task_counter_;
};
