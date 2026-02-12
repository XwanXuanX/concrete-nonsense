// This program demonstrates multiple ways to send a signal to a thread to make
// it shut down

#include <print>
#include <thread>
#include <stop_token>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <cassert>

void atomic_bool() {
    auto worker = [](const std::atomic_bool &flag) {
        std::println("worker start working");
        for (;;) {
            if (flag.load())
                break;
        }
        std::println("worker exits on request");
    };

    std::atomic_bool flag = false;
    // Note that the line below will not work:
    // std::thread t(worker, flag);
    // This is because std::thread copies the arguments by default, but the copy
    // constructor of a atomic type is explicitly deleted! You need to use
    // std::ref() to explicitly tell std::thread to pass by reference
    std::jthread t(worker, std::ref(flag));
    // or
    // std::thread t(worker, std::reference_wrapper(flag));

    // The main thread sleeps for 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // stops the worker thread
    std::println("requesting stop worker thread");
    flag.store(true);
}

void atomic_flag() {
    auto worker = [](const std::atomic_flag &flag) {
        std::println("worker starts waiting on flag");
        // wait while the flag's value is the old value, which is `false`
        // in the main thread, it should set the value of flag to true
        // and wake up the worker thread to check the value again
        flag.wait(false);
        std::println("worker exits on request");
    };

    std::atomic_flag flag = false;
    std::jthread t(worker, std::reference_wrapper(flag));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::println("requesting stop worker thread");

    // WRONG version:
    // without setting the new value of the flag, even if you wake up the worker
    // thread it will just wake up and immediately wait again.
    flag.notify_all();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // CORRECT version:
    // First atomically set the new value and then wake up the worker thread
    flag.test_and_set();
    flag.notify_all();
}

void condition_variable() {
    // This example is bad, since using a condition variable to send a signal to
    // a thread is over engineered, but I'll show it anyways.

    auto worker = [](const bool &flag, std::mutex &m,
                     std::condition_variable &cv) {
        std::println("worker starts waiting on flag");
        {
            // First acquire the lock to access the flag
            std::unique_lock lk(m);
            while (!flag) {
                // If flag is not set, sleep and wait and drop the lock
                // so that the main thread can acquire it
                cv.wait(lk);
                // As soon as we are notified, we immediately acquire the lock
                // and check again
            }
        }
        std::println("worker exits on request");
    };

    bool flag = false;
    std::mutex m; // The mutex is used to protect the flag
    std::condition_variable cv;

    std::jthread t(worker, std::ref(flag), std::ref(m), std::ref(cv));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::println("requesting stop worker thread");

    // atomically set the flag
    {
        std::scoped_lock lk(m);
        flag = true;
    }
    // notify the worker thread that is waiting on the flag
    cv.notify_all();
}

void stop_token() {
    // The most modern C++ approach

    // A shared queue
    std::queue<int> q;
    std::mutex m;
    std::condition_variable cv;

    auto worker = [&q, &m, &cv](std::stop_token st) {
        std::stop_callback cb(st, [&] {
            std::println("stop requested! Waking up...");
            cv.notify_one();
        });

        int stop = false;
        for (;;) {
            const auto head = [&]() {
                int ret = 0;
                {
                    // acquire the lock to access the queue
                    std::unique_lock lk(m);
                    while (q.empty()) {
                        // atomically drop the lock and sleep
                        // immediately grab the lock and check again
                        cv.wait(lk);
                        // when we wake up, where are two possible causes:
                        // 1. due to producer notify
                        // 2. due to stop request
                        // if it's due to a stop request, abort
                        if (st.stop_requested()) {
                            stop = true;
                            break;
                        }
                    }
                    if (!stop) {
                        ret = q.front();
                        q.pop();
                    }
                }
                // if stop requested, ret will be 0
                return ret;
            }();

            if (stop) {
                break;
            }

            // head is guaranteed to be a valid value > 0
            assert(head > 0);
            std::println("get = {}", head);
        }

        std::println("worker thread exiting due to request");
    };

    std::stop_source src;
    std::thread t(worker, src.get_token());

    {
        std::scoped_lock lk(m);
        q.push(42);
        std::println("put = 42");
    }
    cv.notify_one();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        std::scoped_lock lk(m);
        q.push(67);
        std::println("put = 67");
    }
    cv.notify_one();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::println("requesting stop worker thread");
    src.request_stop();

    t.join();
}

int main() {
#if false
    {
        atomic_bool();
        std::println();
    }
    {
        atomic_flag();
        std::println();
    }
    {
        condition_variable();
        std::println();
    }
#endif
    {
        stop_token();
        std::println();
    }
}
