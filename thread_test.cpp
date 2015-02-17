// mutex::lock/unlock
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <random>
#include <ctime>

std::mutex mtx;           // mutex for critical section

void print_thread_id (int id) {
    // critical section (exclusive access to std::cout signaled by locking mtx):
    mtx.lock();
    std::cout << "thread " << id << '\n';
    mtx.unlock();

    int total = 0;
    if ((id % 2) == 0){
        int limit = ((rand() % 8) + 1) * 10000000;
        while(total < limit){
            total += rand() % 100;
        }
    }

    mtx.lock();
    std::cout << "total from thread " << id << " = " << total << '\n';
    mtx.unlock();

}

int main () {
    srand(time(NULL));
    std::thread threads[10];
    // spawn 10 threads:
    for (int i=0; i<10; ++i){
        threads[i] = std::thread(print_thread_id,i+1);
    }
    for (auto& th : threads){
        th.join();
    }

    std::cout << "All threads finished\n";
    return 0;

}
