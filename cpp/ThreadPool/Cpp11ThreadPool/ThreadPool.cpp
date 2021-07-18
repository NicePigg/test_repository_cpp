#include "ThreadPool.h"

ThreadPool::ThreadPool(int min, int max) : finished_(false), min_(min), max_(max)
{
    for (int i = 0; i < min_; i++) {
        AddThread();
    }

    manageThread_ = std::thread([this]() {
        while (!finished_) {
            if ((taskQue_.GetSize() > 2 * threads_.size()) && (threads_.size() < max_)) {
                AddThread();
            }
            else {
                int cnt = 0;
                for (auto& t : threads_) {
                    if (t.second->GetState() == WorkThread::STATE_WAIT) {
                        ++cnt;
                    }
                }
                if ((cnt > 2 * taskQue_.GetSize()) && (threads_.size() > min_)) {
                    DelThread();
                }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        });
}

ThreadPool::~ThreadPool()
{
    if (manageThread_.joinable()) {
        manageThread_.join();
    }
}


void ThreadPool::AddTask(TaskPtr task)
{
    printf("-- add task: [%s]\n", task->GetName().c_str());
    taskQue_.AddTask(task);
}

void ThreadPool::Finish()
{
    finished_ = true;
    for (auto& t : threads_) {
        t.second->Finish();
    }
}

void ThreadPool::AddThread()
{
    printf("-- add thread\n");
    auto tdPtr = std::make_shared<WorkThread>(taskQue_);
    threads_[tdPtr->GetId()] = tdPtr;
}

void ThreadPool::DelThread()
{
    printf("-- del thread\n");
    std::thread::id tid;
    for (auto& t : threads_) {
        if (t.second->GetState() == WorkThread::STATE_WAIT) {
            t.second->Finish();
            tid = t.first;
            break;
        }
    }
    threads_.erase(tid);
}
