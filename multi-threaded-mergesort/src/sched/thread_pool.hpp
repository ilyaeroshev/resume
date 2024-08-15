#pragma once

#include <src/utility/intrusuve_mpmc_queue.hpp>
#include <thread>
#include <vector>

#include "scheduler.hpp"

class ThreadPool : public IScheduler {
 public:
  explicit ThreadPool(size_t threads);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Non-movable
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  void Start();

  // IScheduler
  void Submit(TaskBase*) override;

  void Stop();

 private:
  void WorkerRoutine();

  IntrusiveMPMCQueue<TaskBase> task_queue_;
  std::vector<std::thread> workers_;
  const size_t workers_number_;
};