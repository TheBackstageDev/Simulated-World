#include "../src/Headers/System/SystemBackend/ThreadPool.hpp"

namespace System_Inner
{
    ThreadPool::ThreadPool(size_t threads) : stop(false)
    {
        workers.resize(threads);
        for (size_t i = 0; i < threads; ++i)
        {
            workers.emplace_back([this]{ this->worker(); });
        }
    }

    ThreadPool::~ThreadPool() 
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();

        for (std::thread& thread : workers)
        {
            if (thread.joinable())
                thread.join();
        }
    }

    void ThreadPool::enqueue(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            tasks.emplace(std::move(task));
        }
        condition.notify_one();
    }

    void ThreadPool::enqueue(std::vector<std::function<void()>> tasks)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);

            for (auto& task : tasks)
            {
                this->tasks.emplace(std::move(task));
            }
        }
        condition.notify_all();
    }

    void ThreadPool::worker()
    {
        while (true)
        {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this]
                               { return stop || !tasks.empty(); });

                if (stop && tasks.empty())
                    return;

                if (!tasks.empty())
                {
                    task = std::move(tasks.front());
                    tasks.pop();
                }
            }

            if (task)
            {
                task();
            }
        }
    }

} // namespace System_Inner