# Concurrent Programming Challenge

Assume modern C++ (C++17/20): `std::thread`, `std::mutex`, `std::condition_variable`, `std::atomic`, and (optionally) `std::jthread`, `std::stop_token`.


## Level 0 — Warm-up: Basic Safety and Visibility

1. **Racey counter → correct counter**
    - Spec: N threads increment a shared counter M times; final result must be `N*M`.
    - Variants: fix with **mutex**, then with **atomic** (`fetch_add`).
    - Teaches: **data races**, **atomicity**, perf tradeoffs.

2. **Thread-safe logger**
    - Spec: Many threads write lines; output must have no interleaving within a line.
    - Bonus: minimize contention (buffer per thread + flush).
    - Teaches: **critical sections**, contention, I/O vs compute.

3. **Stop flag + clean shutdown**
    - Spec: workers run until stopped; must exit quickly and deterministically.
    - Variants: atomic flag + sleep; then condition-variable wakeups; then `std::jthread`/`stop_token`.
    - Teaches: **visibility**, clean termination, avoiding “hung join”.


## Level 1 — Classic Synchronization Problems

4. **Bounded blocking queue (SPSC)**
    - Spec: one producer, one consumer; fixed capacity; `push` blocks when full, `pop` blocks when empty.
    - Teaches: **condition variables**, avoiding busy-wait.

5. **Bounded blocking queue (MPMC)**
    - Spec: multiple producers/consumers; correctness under heavy contention.
    - Teaches: correct **notify_one/notify_all**, spurious wakeups, invariant design.

6. **Barrier (one-shot) → reusable barrier**
    - Spec: N threads call `arrive_and_wait()`; all proceed together; reusable over phases.
    - Teaches: phase counters, avoiding missed wakeups.

7. **Readers–Writers lock**
    - Spec: implement `lock_shared()/unlock_shared()` + `lock()/unlock()`.
    - Variants: reader-prefer, writer-prefer, then fair.
    - Teaches: **starvation**, fairness policies.

8. **Dining Philosophers**
    - Spec: avoid **deadlock** and **starvation**; measure fairness (everyone eats).
    - Teaches: resource ordering, try-lock patterns, fairness.


## Level 2 — “Library-grade” Building Blocks

9. **Thread pool (fixed size) + futures**
    - Spec: `submit(f)` returns `std::future<R>`; supports many producers.
    - Requirements: no lost tasks, graceful shutdown, no deadlock on destruction.
    - Teaches: work queues, lifetime rules, exception propagation.

10. **Timer queue / scheduled executor**
    - Spec: schedule tasks to run at/after a time point; thread-safe cancel.
    - Teaches: time + concurrency, CV wait-until, cancellation races.

11. **Rate limiter (token bucket)**
    - Spec: `acquire(k)` blocks until k tokens available; safe under many threads.
    - Teaches: shared state + time, correctness under contention.

12. **Single-flight (request coalescing)**
    - Spec: concurrent `get(key)` where misses cause only one compute; others wait and share result/exception.
    - Teaches: per-key coordination, avoiding thundering herd.


## Level 3 — Concurrent Data Structures (Locking First)

13. **Concurrent hash map with lock striping**
    - Spec: K bucket locks; `get/put/erase`; correctness under contention.
    - Bonus: dynamic rehashing safely.
    - Teaches: lock granularity, resizing hazards.

14. **LRU cache (thread-safe)**
    - Spec: `get/put` with LRU eviction; correctness + performance.
    - Variants: single lock; then split locks (map lock + list lock) and fix ordering.
    - Teaches: **deadlock** prevention, lock ordering, invariants across structures.

15. **Producer/consumer pipeline**
    - Spec: multi-stage pipeline with bounded queues and backpressure.
    - Teaches: bounded memory, throughput vs latency, shutdown propagation.


## Level 4 — Atomics and Memory Ordering

16. **Lock-free SPSC ring buffer**
    - Spec: non-blocking `push/pop` for single producer/consumer.
    - Use: atomics for head/tail with correct **acquire/release**.
    - Teaches: memory ordering, false sharing, cache friendliness.

17. **Seqlock (read-mostly structure)**
    - Spec: many readers read a snapshot; single writer updates; readers retry on inconsistency.
    - Teaches: optimistic concurrency, correctness criteria.

18. **Spinlock + backoff (educational)**
    - Spec: implement a simple spinlock; add exponential backoff/yield.
    - Teaches: why spinlocks can be terrible, scheduling interactions.


## Level 5 — Expert: Lock-free + Verification Mindset

19. **MPMC bounded queue (lock-free, stretch)**
    - Spec: multi-producer/multi-consumer ring buffer; linearizable `enqueue/dequeue`.
    - Teaches: CAS loops, ABA considerations, correctness proofs.

20. **Work-stealing deque (Chase–Lev style, stretch)**
    - Spec: each worker has deque; local pop/push fast; thieves steal from top.
    - Teaches: complex interleavings, performance vs correctness.

21. **Model-check style testing for one primitive**
    - Spec: pick your barrier/queue/RW lock; write:
        - randomized stress tests (millions of ops)
        - invariant checks (no lost tasks, monotonic counters, etc.)
        - “adversarial scheduling” (yields/sleeps in critical spots)
    - Teaches: finding rare bugs and building confidence.


## C++-specific tools 

- **ThreadSanitizer**: catches data races (compile with `-fsanitize=thread`).
- **Address/UB sanitizers**: catch memory/UB issues (`-fsanitize=address,undefined`).
- Optional: `perf`/VTune or just wall-clock benchmarks + `std::chrono`.
