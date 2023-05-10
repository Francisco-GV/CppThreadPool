#include <future>
#include <memory>
#include <mutex>
#include <vector>
#include <queue>

class ThreadPool
{
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();

    template <typename Func, typename... Args>
        auto enqueue(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        using returnType = decltype(func(args...));

        auto task = std::make_shared<std::packaged_task<returnType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args...))
        );

        std::future<returnType> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_all();

        return result;
    }


private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;

    void workerThread();
};