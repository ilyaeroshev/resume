#pragma once

#include <src/utility/intrusive_forward_list.hpp>

struct ITask {
  virtual void Run() noexcept = 0;

 protected:
  ~ITask() = default;
};

// Intrusive task
struct TaskBase : ITask, 
                  IntrusiveForwardListNode<TaskBase> {
  virtual ~TaskBase() = default;
};