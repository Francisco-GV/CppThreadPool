#include "thread_pool.hpp"
#include <iostream>

int main()
{
    ThreadPool pool{5};

    for (int n{1}; n <= 5; n++)
    {
        pool.enqueue([n]() {
            for (int i{0}; i < 10; i++)
            {
                std::cout << "Thread " << n << " active (" << n << ")\n";
            }

            std::cout << "thread " << n << " ended.\n"; 
    });
    }

    std::cout << "Main thread ended.\n"; 
}