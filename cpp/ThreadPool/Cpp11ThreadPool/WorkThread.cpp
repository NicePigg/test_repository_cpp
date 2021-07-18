#include "WorkThread.h"
#include <chrono>
#include <cstdio>
#include "ThreadUtils.h"

WorkThread::WorkThread(TaskQueue& taskQue) : taskQue_(taskQue), state_(STATE_WAIT), finished_(false)
{
    thread_ = std::thread([this]() {
        while (!finished_) {
            state_ = STATE_WAIT;
            while (!finished_ && taskQue_.IsEmpty()) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
            if (finished_) {
                break;
            }
            TaskPtr task = taskQue_.GetTask();
            if (task != nullptr) {
                state_ = STATE_WORK;
                printf("-- thread [%3d]: run task [%20s] begin\n", GetThreadId(), task->GetName().c_str());
                task->Run();
                printf("-- thread [%3d]: run task [%20s] end\n", GetThreadId(), task->GetName().c_str());
            }
        }
        state_ = STATE_EXIT;
        });
    printf("-- thread [%3d]: started\n", GetThreadId(thread_));
}

WorkThread::~WorkThread()
{
    printf("-- thread [%3d]: finished\n", GetThreadId(thread_));
    if (thread_.joinable()) {
        thread_.join();
    }
}