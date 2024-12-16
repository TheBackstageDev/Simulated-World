#pragma once

#include <vector>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace System_Inner
{
    class ThreadPool
    {
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::condition_variable condition;
        std::mutex queueMutex;
        bool stop;

        void worker();
    public:
        ThreadPool(size_t threads);
        ~ThreadPool();

        void enqueue(std::function<void()> task);
        void enqueue(std::vector<std::function<void()>> tasks);
    };
} // namespace System_Inner
     