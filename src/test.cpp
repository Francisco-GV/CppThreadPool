#include "thread_pool.hpp"
#include <iostream>

int main()
{
    static ThreadPool pool{5};

    static std::mutex mutex;

    for (int n{1}; n <= 5; n++)
    {
        pool.enqueue([n](std::mutex& mutex) {
            for (int i{0}; i < 1000; i++)
            {
                std::unique_lock<std::mutex> lock(mutex);
                std::cout << "Thread " << n << " active (" << i << ")" << std::endl;
            }

            {
                std::unique_lock<std::mutex> lock(mutex);
                std::cout << "thread " << n << " ended." << std::endl;
            }
        }, std::ref(mutex));
    }

    {
        std::unique_lock<std::mutex> lock(mutex);
        std::cout << "Main thread ended.\n"; 
    }
}