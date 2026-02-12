#include "argy.hh"
#include <print>
#include <thread>
#include <vector>
#include <format>
#include <mutex>

int main(int argc, char *argv[]) {
    Argy::CliParser cli(argc, argv);
    try {
        cli.addInt("nth",
                   "The number of threads used to increment the counter");
        cli.parse();
    } catch (const std::exception &ex) {
        std::println("Wrong usage!");
        std::exit(-1);
    }
    const int nth = cli.getInt("nth");

    std::mutex mut;

    auto work = [&mut]() -> void {
        const std::__thread_id id = std::this_thread::get_id();
        for (;;) {
            // std::println("Hello from thread {}", id);
            // DON'T use `std::println()` since it's thread safe
            {
                std::scoped_lock lock(mut);
                std::cout << std::format("Hello from thread {}", id)
                          << std::endl;
            }
            std::this_thread::sleep_for(
                std::chrono::nanoseconds(static_cast<int>(1e6)));
        }
    };

    std::vector<std::thread> ths;
    for (int i = 0; i < nth; ++i) {
        ths.emplace_back(work);
    }
    for (auto &&t : ths) {
        t.join();
    }
}
