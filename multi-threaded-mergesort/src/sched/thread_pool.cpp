#include "thread_pool.hpp"

#include <cassert>

ThreadPool::ThreadPool(size_t threads) : workers_number_(threads) {}

ThreadPool::~ThreadPool() {
  assert(workers_.empty());
}

void ThreadPool::Start() {
  workers_.reserve(workers_number_);
  while (workers_.size() < workers_number_) {
    workers_.emplace_back([this]() {
      cur_scheduler = this;
      WorkerRoutine();
    });
  }
}

void ThreadPool::Submit(TaskBase* task) {
  task_queue_.Push(task);
}

void ThreadPool::Stop() {
  task_queue_.Close();
  for (auto& t : workers_) {
    t.join();
  }
  workers_.clear();
}

void ThreadPool::WorkerRoutine() {
  while (auto task = task_queue_.Pop()) {
    task->Run();
  }
}