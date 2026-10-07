#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<int> counter = 0;
int normal = 0;

void worker() {
    for(int i = 0; i < 100000; ++i) {
        {
            int expected = counter.load();
            int desired = 0;
            do {
                desired = expected + 1;
            } while(!counter.compare_exchange_weak(expected, desired));
        }
        {
            // The above CAS is equal to this line.
            // counter.fetch_add(1);
        }
        normal++;
    }
}

int main() {
    std::vector<std::thread> ts;
    for(int i = 0; i < 100; ++i) {
        ts.push_back(std::thread(worker));
    }
    for(int i = 0; i < 100; ++i) {
        ts[i].join();
    }
    std::cout << counter.load() << '\n';
    std::cout << normal << '\n';
}