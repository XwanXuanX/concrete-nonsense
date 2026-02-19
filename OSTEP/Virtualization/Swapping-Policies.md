# Beyond Physical Memory: Policies

## The problem
- How to decide which page to evict


## Cache management
- Main memory is the cache for **virtual memory pages**
- Goal: minimize the number of cache misses (or maximize cache hit)

### Average Memory Access Time (AMAT)
- **AMAT**:  
  $$AMAT = T_M + (P_{miss} \cdot T_D)$$
  - $T_M$: cost of accessing **memory**
  - $T_D$: cost of accessing **disk**
  - $P_{miss}$: probability of a **cache miss**


## The optimal replacement policy
- Evict the page that is needed the **furthest from now**
- **Optimal**, but requires knowing the **future**  
  - Not practical; only serves as a **comparison**

### Aside: types of cache misses
- **Compulsory**: cache is empty to begin with, and this is the first reference to the item
- **Capacity**: cache ran out of space and had to evict an item
- **Conflict**: because of limits on where an item can be placed in hardware cache due to **set-associativity**


## A simple policy: FIFO
- **FIFO**: **first in, first out**


## Another simple policy: Random
- **Random**: picks a **random page** to replace


## Using history: LRU
- **Recency**: if a page is accessed recently, it is likely accessed in the near future

### History-based policies
- **LFU (Least-frequently-used)**
- **LRU (Least-recently-used)**
- **MFU (Most-frequently-used)**: ignores **locality**, **bad**
- **MRU (Most-recently-used)**: ignores **locality**, **bad**


## Approximating LRU
- Implementing a perfect **LRU** is very expensive and not practically possible
- Problem: can we approximate it in some way and still obtain the desired behavior?

### Hardware support
- A **use bit** per page
- When a page is referenced, the **use bit** is set by hardware to **1**
  - Hardware never clears the bit; the **OS** needs to do that

### Clock algorithm
- **Clock algorithm**: read [this](https://web.stanford.edu/class/archive/cs/cs111/cs111.1232/lectures/25/Lecture25.pdf)


## Considering dirty pages
- Small modification to **clock algorithm**: also consider whether a page has been modified or not while in memory

### Dirty vs. clean pages
- If a page is **dirty**, it must be written back to disk to evict it (**expensive**)
- If a page is **clean**, eviction is **free**
  - Do not need to write back to disk; simply overwrite it
- Prefer to evict **clean pages** over **dirty pages**

### Hardware requirement
- Hardware must include a **dirty bit**
- The **dirty bit** is set any time a page is **written**


## Other VM policies
- Page replacement is not the only policy the **VM subsystem** employs

### Examples
- **Page selection policy** (e.g., **prefetching**: prefetch about-to-be-used pages proactively)
- How the **OS** writes pages out to disk: **clustering/grouping** of writes


## Thrashing
### What is thrashing?
- **Thrashing**: what should the OS do when memory is **oversubscribed** and the memory demands exceeds the available physical memory?
  - Will constantly be **paging**

### Responses in OS design
- Earlier OS: **Admission control**
  - Decide not to run a subset of processes
  - Hope that the reduced set of processes' **working sets** fit in memory
- Modern OS: **out-of-memory killer**
  - The daemon chooses a **memory-intensive process** and kills it
