#ifndef SWIFT_CPP_THREAD_UTILS_H
#define SWIFT_CPP_THREAD_UTILS_H

#include <cstddef>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include <glog/logging.h>

#include "common.h"

namespace swiftcpp {
namespace thread {

// Cocurrent blocking queue.
template <typename T> class TaskQueue {
public:
  TaskQueue() = default;
  ~TaskQueue() = default;

  void Push(T &&v);

  void Push(const T &v);

  bool Pop(T *rv);

  // Tell all blocking pop will return false.
  void SignalForKill();

  size_t Size() const;

private:
  std::mutex mut_;
  std::condition_variable cv_;
  std::atomic<bool> killed_{false};
  // std::atomic<int> num_pending_tasks_;
  std::queue<T> queue_;

  DISALLOW_COPY_AND_ASSIGN(TaskQueue);
};

class ThreadPool {
public:
  typedef std::function<void()> task_t;
  ThreadPool(int n, const task_t &task) : num_workers_(n), task_(task) {
    for (int i = 0; i < n; i++) {
      workers_.emplace_back(task);
      DLOG(WARNING) << "new thread " << workers_.back().get_id();
    }
  }

  size_t Size() const { return workers_.size(); }

  ~ThreadPool() {
    for (auto &t : workers_) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

private:
  int num_workers_{0};
  std::vector<std::thread> workers_;
  task_t task_;

  ThreadPool() = delete;
  DISALLOW_COPY_AND_ASSIGN(ThreadPool);
};

template <typename T> void TaskQueue<T>::Push(T &&v) {
  std::lock_guard<std::mutex> l(mut_);
  queue_.emplace(std::forward<T>(v));
  cv_.notify_one();
}

template <typename T> void TaskQueue<T>::Push(const T &v) {
  std::lock_guard<std::mutex> l(mut_);
  queue_.emplace(v);
  cv_.notify_one();
}

template <typename T> bool TaskQueue<T>::Pop(T *cv) {
  std::unique_lock<std::mutex> l(mut_);
  // ++num_pending_tasks_;
  cv_.wait(l, [this] { return !queue_.empty() || killed_.load(); });
  if (killed_.load())
    return false;
  *cv = std::move(queue_.front());
  queue_.pop();
  return true;
}

template <typename T> size_t TaskQueue<T>::Size() const {
  return queue_.size();
}

template <typename T> void TaskQueue<T>::SignalForKill() {
  killed_ = true;
  DLOG(INFO) << "queue singal for kill...";
  cv_.notify_all();
}

} // namespace thread
} // namespace swiftcpp

#endif
