#include <c10/core/thread_pool.h>
#include <iostream>
#include <omp.h>

namespace c10 {

ThreadPool::ThreadPool(
    int pool_size,
    int numa_node_id,
    std::function<void()> init_thread)
    : threads_(pool_size < 0 ? defaultNumThreads() : pool_size),
      condition_(threads_.size()),
      running_(true),
      complete_(true),
      available_(threads_.size()),
      total_(threads_.size()),
      numa_node_id_(numa_node_id) {
  tasks_.resize(threads_.size());
  for (std::size_t i = 0; i < threads_.size(); ++i) {
    threads_[i] = std::thread([this, i, init_thread]() {
      if (init_thread) {
        init_thread();
      }
      std::cerr << omp_get_num_places() << std::endl;
      this->main_loop(i);
    });
  }
}

ThreadPool::~ThreadPool() {
  // Set running flag to false then notify all threads.
  {
    std::unique_lock<std::mutex> lock(mutex_);
    running_ = false;
    for (auto &cv : condition_) cv.notify_one();
  }

  for (auto& t : threads_) {
    try {
      t.join();
    } catch (const std::exception&) {
    }
  }
}

size_t ThreadPool::size() const {
  return threads_.size();
}

size_t ThreadPool::numAvailable() const {
  std::unique_lock<std::mutex> lock(mutex_);
  return available_;
}

bool ThreadPool::inThreadPool() const {
  for (auto& thread : threads_) {
    if (thread.get_id() == std::this_thread::get_id()) {
      return true;
    }
  }
  return false;
}

void ThreadPool::run(std::function<void()> func) {
  if (threads_.size() == 0) {
    throw std::runtime_error("No threads to run a task");
  }
  const auto cpu_id = sched_getcpu();
  const auto qid = cpu_id<48 ? total_/2+cpu_id/2 : (cpu_id-48)/2;
  //std::cerr << "launch by " << cpu_id << " use queue " << qid << std::endl;
  std::unique_lock<std::mutex> lock(mutex_);

  // Set task and signal condition variable so that a worker thread will
  // wake up and use the task.
  tasks_[qid].emplace(std::move(func));
  complete_ = false;
  condition_[qid].notify_one();
}

void ThreadPool::waitWorkComplete() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (!complete_) {
    completed_.wait(lock);
  }
}

void ThreadPool::main_loop(std::size_t index) {
  const auto cpu_id = sched_getcpu();
  const auto qid = cpu_id<48 ? cpu_id/2 : total_/2+(cpu_id-48)/2;
  std::cerr << "thread " << index << " bind to " << cpu_id << " use queue " << qid << std::endl;
  std::unique_lock<std::mutex> lock(mutex_);
  auto &taskq = tasks_[qid];
  auto &cv = condition_[qid];
  while (running_) {
    // Wait on condition variable while the task is empty and
    // the pool is still running.
    while (taskq.empty() && running_) {
      cv.wait(lock);
    }
    // If pool is no longer running, break out of loop.
    if (!running_) {
      break;
    }
    //std::cerr << index << " runs on " << sched_getcpu() << std::endl;

    // Copy task locally and remove from the queue.  This is
    // done within its own scope so that the task object is
    // destructed immediately after running the task.  This is
    // useful in the event that the function contains
    // shared_ptr arguments bound via bind.
    {
      task_element_t tasks = std::move(taskq.front());
      taskq.pop();
      // Decrement count, indicating thread is no longer available.
      --available_;

      lock.unlock();

      // Run the task.
      try {
        if (tasks.run_with_id) {
          tasks.with_id(index);
        } else {
          tasks.no_id();
        }
      } catch (const std::exception& e) {
        LOG(ERROR) << "Exception in thread pool task: " << e.what();
      } catch (...) {
        LOG(ERROR) << "Exception in thread pool task: unknown";
      }

      // Destruct tasks before taking the lock.  As tasks
      // are user provided std::function, they can run
      // arbitrary code during destruction, including code
      // that can reentrantly call into ThreadPool (which would
      // cause a deadlock if we were holding the lock).
    }

    // Update status of empty, maybe
    // Need to recover the lock first
    lock.lock();

    // Increment count, indicating thread is available.
    ++available_;
    if (taskq.empty() && available_ == total_) {
      complete_ = true;
      completed_.notify_one();
    }

    // Deliberately hold the lock on the backedge, so this thread has an
    // opportunity to acquire a new task before another thread acquires
    // the lock.
  } // while running_
}

C10_DEFINE_SHARED_REGISTRY(
    ThreadPoolRegistry,
    TaskThreadPoolBase,
    int,
    int,
    bool);
} // namespace c10
