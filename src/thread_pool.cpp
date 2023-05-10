#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t num_threads) : threads{num_threads}
{
    for (std::thread& thread : threads)
    {
        thread = std::thread(&ThreadPool::workerThread, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread& thread : threads)
    {
        thread.join();
    }
}

void ThreadPool::workerThread()
{
    while (true)
    {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            condition.wait(lock, [this] { stop || !tasks.empty();});

            if (stop && tasks.empty())
            {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}