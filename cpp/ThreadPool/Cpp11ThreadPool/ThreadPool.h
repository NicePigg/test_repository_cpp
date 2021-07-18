#pragma once
#include <unordered_map>
#include "TaskQueue.h"
#include "WorkThread.h"
#include "ThreadUtils.h"

class ThreadPool {
public:
    ThreadPool(int min = 10, int max = 50);
    ~ThreadPool();

    void AddTask(TaskPtr task);

    void Finish();

private:
    void AddThread();
    void DelThread();

    std::atomic_bool finished_;
    std::unordered_map<std::thread::id, ThreadPtr> threads_;
    std::thread manageThread_;
    TaskQueue taskQue_;

    int min_;
    int max_;
};