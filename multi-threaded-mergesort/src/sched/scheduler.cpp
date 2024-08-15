#include "scheduler.hpp"

thread_local IScheduler* IScheduler::cur_scheduler{nullptr};

IScheduler* IScheduler::Current() {
  return cur_scheduler;
}