#pragma once

#include <utility>

#include "task.hpp"

struct IScheduler {
  virtual ~IScheduler() = default;

  static IScheduler* Current();

  virtual void Submit(TaskBase*) = 0;

 protected:
  static thread_local IScheduler* cur_scheduler;
};

template <typename F>
void Submit(IScheduler& scheduler, F fun) {
  struct Task : TaskBase {
    explicit Task(F fun) : fun(std::move(fun)) {}

    void Run() noexcept override {
      fun();
      delete this;
    }

    F fun;
  };

  Task* task_ptr = new Task(std::move(fun));

  scheduler.Submit(task_ptr);
}