#pragma once

#include <atomic>
#include <iterator>
#include <src/sched/thread_pool.hpp>
#include <src/sync/wait_group.hpp>
#include <src/utility/defer.hpp>
#include <src/utility/segment.hpp>

template <typename MergeTask>
struct MergeWaiter {
  void Done() {
    if (merge_counter_.fetch_add(1 /* , std::memory_order */) == 1) {
      // all (two) tasks are completed
      left_task->left_seg.end = left_task->right_seg.end;
      left_task->right_seg.begin = right_task->left_seg.begin;
      left_task->right_seg.end = right_task->right_seg.end;

      left_task->right_waiter = right_task->right_waiter;
      if (left_task->right_waiter) {
        left_task->right_waiter->left_task = left_task;
      }

      IScheduler::Current()->Submit(left_task);
    }
  }

  MergeTask *left_task{nullptr}, *right_task{nullptr};
  std::atomic<int> merge_counter_{0};
};

template <typename ForwardIt, typename Compare>
struct MergeTask : public TaskBase {
  using Data = std::remove_reference_t<decltype(*ForwardIt())>;
  using MergeWaiter = MergeWaiter<MergeTask>;

  void Run() noexcept override {
    // Merge left_seg and right_seg

    if (right_seg.NonEmpty()) {
      // Merge to buffer
      auto lhs_it = left_seg.begin;
      auto rhs_it = right_seg.begin;
      Data* buf_ptr = buffer;

      while (lhs_it != left_seg.end && rhs_it != right_seg.end) {
        if (st->cmp(*rhs_it, *lhs_it)) {
          new (buf_ptr++) Data(std::move(*rhs_it++));
        } else {
          new (buf_ptr++) Data(std::move(*lhs_it++));
        }
      }

      while (lhs_it != left_seg.end) {
        new (buf_ptr++) Data(std::move(*lhs_it++));
      }

      while (rhs_it != right_seg.end) {
        new (buf_ptr++) Data(std::move(*rhs_it++));
      }

      // Move from buffer
      buf_ptr = buffer;
      for (auto it = left_seg.begin; it != right_seg.end; ++it, ++buf_ptr) {
        *it = std::move(*buf_ptr);
        (*buf_ptr).~Data();
      }
    }

    // Define merge-waiter
    MergeWaiter* waiter;

    if (number % 2) {
      if (right_waiter) {
        waiter = right_waiter;
      } else {
        while (number % 2 && number != 1) {
          RecalcNumber();
        }
        waiter = left_waiter;
      }
    } else {
      assert(left_waiter);
      waiter = left_waiter;
    }

    RecalcNumber();

    if (!waiter) {  // nullptr -> sorting is complite
      // One call for sorting
      st->wg.Done();
      return;
    }

    Defer d([waiter]() { waiter->Done(); });  // this task can be reused
  }

  Segment<ForwardIt> left_seg, right_seg;

  struct SortingTools {
    Compare& cmp;
    WaitGroup& wg;
  };

  SortingTools* st;

  Data* buffer;

  MergeWaiter *left_waiter{nullptr}, *right_waiter{nullptr};
  size_t number;

  void RecalcNumber() { number = (number + 1) >> 1; }
};

template <typename ForwardIt, typename Compare>
void MergeSort(ForwardIt begin, ForwardIt end, Compare cmp, IScheduler& sched) {
  using Data = std::remove_reference_t<decltype(*begin)>;
  using MergeTask = MergeTask<ForwardIt, Compare>;
  using MergeWaiter = MergeWaiter<MergeTask>;

  size_t n = std::distance(begin, end);

  if (n < 2) return;

  Data* buffer =
      reinterpret_cast<Data*>(::operator new(n * sizeof(Data)));  // TODO
  Defer d([buffer]() { ::operator delete((void*)buffer); });

  WaitGroup wg;
  wg.Add(1);

  typename MergeTask::SortingTools st{cmp, wg};

  std::vector<MergeTask> tasks((n + 1) / 2);
  std::vector<MergeWaiter> waiters((n - 1) / 2);

  auto it = begin;
  for (size_t i = 0; i < tasks.size(); ++i) {
    tasks[i].st = &st;
    tasks[i].buffer = buffer + 2 * i;
    tasks[i].number = i + 1;

    tasks[i].left_seg.begin = it;
    tasks[i].left_seg.end = ++it;
    tasks[i].right_seg.begin = it;

    tasks[i].right_seg.end = (it != end ? ++it : it);
  }

  for (size_t i = 0; i < waiters.size(); ++i) {
    tasks[i].right_waiter = tasks[i + 1].left_waiter = &waiters[i];
    waiters[i].left_task = &tasks[i];
    waiters[i].right_task = &tasks[i + 1];
  }

  for (auto& t : tasks) {
    sched.Submit(&t);
  }

  wg.Wait();  // wait for sorting
}

template <typename ForwardIt, typename Compare>
void MergeSort(ForwardIt begin, ForwardIt end, Compare cmp) {
  size_t workers = std::max(1u, std::thread::hardware_concurrency());

  ThreadPool tp(workers);

  tp.Start();

  MergeSort(begin, end, cmp, tp);

  tp.Stop();
}

template <typename ForwardIt>
void MergeSort(ForwardIt begin, ForwardIt end, IScheduler& sched) {
  using Data = std::remove_reference_t<decltype(*begin)>;

  MergeSort(begin, end, std::less<Data>(), sched);
}

template <typename ForwardIt>
void MergeSort(ForwardIt begin, ForwardIt end) {
  using Data = std::remove_reference_t<decltype(*begin)>;

  MergeSort(begin, end, std::less<Data>());
}