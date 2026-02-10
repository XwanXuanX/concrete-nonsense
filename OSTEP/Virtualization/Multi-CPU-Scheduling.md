# Multiprocessor Scheduling (Advanced)

## The problem
- **How to schedule jobs on multiple CPUs**


## Background: Multiprocessor Architecture
- Difference between **single-CPU hardware** and **multi-CPU hardware** (mostly use of hardware **caches**)
  - Caches are based on **locality**
    - **Temporal locality**
    - **Spatial locality**
  - What happens if you have multiple processors with a single shared **main memory**?
    - **MUCH more complicated!**
  - Problem of **cache coherence**, basic solution provided by the hardware:
    1. **Bus snooping**: each cache pays attention to memory updates by observing the **bus** that connects them to main memory.
    2. When a CPU sees an update for a data it holds in its cache, it will notice the change and **invalidate** or **update** its copy.


## Don't forget synchronization
- When accessing shared data items across CPUs, a **mutex** should be use to guarantee **correctness**
- Leads to **performance problems**:
  - As the number of CPU grows, access to a **synchronized shared data structure** is quite slow.


## One Final Issue: cache affinity
- A process when run on a particular CPU, builds up a fair bit of state in the **caches** and **TLBs**
- The next time the process runs, it's more advantageous to run it on the **same CPU**
- If the process is run on different CPUs every time, the performance will be worse
- A multiprocessor scheduler should preferring to keep a process on the same CPU if possible (**cache affinity**)


## Single-Queue scheduling
- **Single-queue multiprocessor scheduling (SQMS)**: put all jobs into a **single queue**
- Advantage:
  - **Simplicity** — take an existing **single-processor scheduling policy** and adapt it
- Disadvantages:
  1. **Lack of scalability**: has to use **locks** → reduce performance
  2. **Cache affinity**: globally-shared queue → each job **bounce around** from CPU to CPU


## Multi-queue scheduling
- **Multi-queue multiprocessor scheduling (MQMS)**: one queue per CPU
- Each queue will follow one scheduling policy (e.g. **RR**)
- Job is placed on exactly one scheduling queue (according to some **heuristic**)
- Then scheduled independently
- Advantages:
  1. More **scalable**: avoid need for **synchronization**
  2. Better **cache affinity**
- Disadvantage:
  - **Load imbalance**


## How to deal with load imbalance
- **Continuous migration**: move a job from one CPU to another continuously to achieve overall **load balance**
- How can the system decide what and when to migrate?
  - **Work stealing**:
    1. A queue that's low on jobs will **peek** at another queue to see how full it is
    2. If the other queue is more full than itself, then it will **steal** one or more jobs to help balance load


## Linux Multiprocessor Schedulers
- **O(1)**: **multi-queues**, **priority based**, similar to **MLFQ**
- **CFS**: **multi-queues**, **deterministic proportional-share**
- **BFS**: **single queue**, **proportional-share**, based on **Earliest Eligible Virtual Deadline First (EEVDF)**
