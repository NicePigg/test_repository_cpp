#include <cstdio>
#include "ThreadPool.h"

class MyTask : public Task {
public:
    MyTask(int i) : Task("MyTask" + std::to_string(i)) {}
    void Run()
    {
        for (int i = 0; i < 5; i++) {
            printf("-- thread [%3d]: task_name=[%20s], out=%d\n", GetThreadId(), GetName().c_str(), i);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main(int argc, char** argv) {
    ThreadPool pool;
    for (int i = 0; i < 50; i++) {
        pool.AddTask(std::make_shared<MyTask>(i));
    }
    int i = 100;
    while (i < 150) {
        pool.AddTask(std::make_shared<MyTask>(i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
    }
    pool.Finish();
    return 0;
}
