// Shows all the fancy ways to increment a counter LOL

#include "argy.hh"
#include "timer.hh"
#include <iostream>
#include <print>
#include <mutex>
#include <thread>
#include <vector>
#include <cassert>
#include <semaphore>
#include <atomic>

void mutex(const int inc, const int nth) {
    int counter = 0; // the shared state
    std::mutex mut;

    auto worker = [inc, &counter, &mut]() -> void {
        for (int i = 0; i < inc; ++i) {
            std::scoped_lock lock(mut);
            counter++;
        }
    };

    std::vector<std::thread> ths;
    for (int i = 0; i < nth; ++i) {
        ths.emplace_back(worker);
    }
    for (auto &&t : ths) {
        t.join();
    }

    std::println("counter = {}, expected = {}", counter, nth * inc);
    assert(counter == nth * inc);
}

void atomic(const int inc, const int nth) {
    std::atomic_int counter = 0; // the shared state

    auto worker = [inc, &counter]() -> void {
        for (int i = 0; i < inc; ++i) {
            counter++;
        }
    };

    std::vector<std::thread> ths;
    for (int i = 0; i < nth; ++i) {
        ths.emplace_back(worker);
    }
    for (auto &&t : ths) {
        t.join();
    }

    const int cnt = counter.load();
    std::println("counter = {}, expected = {}", cnt, nth * inc);
    assert(counter == nth * inc);
}

void cas(const int inc, const int nth) {
    std::atomic_int counter = 0; // the shared state

    auto cas_inc = [](std::atomic_int &n) {
        // work is only committed if the expected value is up to date
        // otherwise, keep trying
        int expected = n.load();
        int desired = 0;
        do {
            desired = expected + 1;
        } while (!n.compare_exchange_weak(expected, desired));
    };

    auto worker = [inc, &counter, &cas_inc]() -> void {
        for (int i = 0; i < inc; ++i) {
            cas_inc(counter);
        }
    };

    std::vector<std::thread> ths;
    for (int i = 0; i < nth; ++i) {
        ths.emplace_back(worker);
    }
    for (auto &&t : ths) {
        t.join();
    }

    const int cnt = counter.load();
    std::println("counter = {}, expected = {}", cnt, nth * inc);
    assert(counter == nth * inc);
}

int main(int argc, char *argv[]) {
    Argy::CliParser cli(argc, argv);
    try {
        cli.addInt(
            "inc",
            "The number of times each thread should increment the counter");
        cli.addInt("nth",
                   "The number of threads used to increment the counter");
        cli.parse();
    } catch (const std::exception &ex) {
        std::println("Wrong usage!");
        std::exit(-1);
    }
    const int inc = cli.getInt("inc"), nth = cli.getInt("nth");

    {
        Timer t;
        mutex(inc, nth);
    }

    {
        Timer t;
        atomic(inc, nth);
    }

    {
        Timer t;
        cas(inc, nth);
    }
}
