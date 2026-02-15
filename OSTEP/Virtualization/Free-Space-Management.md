# Free-Space Management

## Aside: Modern memory management
- Memory works like buying in bulk: the OS hands out memory in big, fixed-size **pages** (like **4KB** chunks), and the **`malloc`** library asks for a bunch of these pages at once using **`sbrk()`** or **`mmap()`**.
- Then the library cuts that big supply into smaller pieces, keeps track of what’s free, and quickly gives your program the exact sizes it needs—without calling the OS every time.

## The problem
- How to manage **free space** when satisfying **variable-sized requests**?
- What strategies can be used to minimize **fragmentation**?

## Assumptions
- Primarily concerned with **external fragmentation** (not **internal fragmentation**).
- Once memory is handed out to a **client**, it cannot be relocated to another location.
  - No **compaction** of free space!
- Allocator manages a **contiguous**, **fix-sized** region of bytes.

## Low-level mechanisms

### Splitting and Coalescing
- A **free list** describes the free space still remaining in the **heap**.
  - Example:
    - `head -> {addr:0, len:10} -> {addr:20, len:10} -> null`

- **Splitting**: find a free chunk of memory that can satisfy the request and split it.
  - Example:
    - `head -> {addr:0, len:10} -> {addr:21, len:9} -> null`

- **Coalescing**: if the newly freed space sits right next to one existing free chunk, merge them.
  - Example:
    - `head -> {addr:10, len:10} -> {addr:0, len:10} -> {addr:20, len:10} -> null`
  - Example:
    - `head -> {addr:0, len:30} -> null`

### Tracking the size of allocated regions
- Store a little bit of extra information in a **header block**, before the handed-out memory.
- `header_t *hptr = (header_t*)ptr - 1;`

### Embedding a free list
- List nodes are embedded before free memory chunks, with **`size`** and **`next`** pointer.
- **`next`** pointer points to the node of the next free memory chunk, creating a **linked list**.
- Linked list can be referenced by **`head`** and **`tail`** pointers.

### Growing the heap
- Start with a small **heap**.
- **`sbrk()`**: OS finds free **pages**, maps them into the address space of the process.
- A larger **heap** is available.

## Basic Strategies

### Best fit
- Search through the **free list** and find chunks that are as big or bigger and return the smallest one.
- Tries to reduce wasted space.
- Exhaustive search cause **performance penalty**.

### Worst fit
- Opposite of **best fit**: always use (split) the largest chunk.
- Tries to leave big chunks free instead of lots of small chunks.
- Exhaustive search cause **performance penalty** AND performs badly!

### First fit
- Finds the first block that is big enough.
- Speed: no exhaustive search.
- Pollutes the beginning of the **free list** with small objects.
- **Address-based ordering**: keeping the list ordered by the address of the free space.

### Next fit
- Use an extra pointer to locate where in the list one was looking last.
- Spread the searches for free space more uniformly.
- Performance same as **first fit**.

## Other approaches

### Segregated lists
- If a particular application has one popular-sized request, keep a separate list just to manage objects of that size; all other requests are forwarded to a more general allocator.
- Less fragmentation due to **fixed-size**.
- Allocation and free can be quick for that **fix-sized** requests.

#### Slab allocator
- When boot, allocates a number of object caches for frequent kernel objects.
- Object caches are each **segregated lists** of a given size.
- When a cache is low on free space, requests some **slabs** of memory from a general memory allocator.
- When the reference counts of the objects within a given slab go to zero, general allocator can reclaim them from the specialized allocator.
- Keeping free objects on the list in a pre-initialized state (not really required).

### Binary Buddy Allocator
- Designed to make **coalescing** simple.
- Only allowed to give out **power-of-two-sized** blocks.
- Suffer from **internal fragmentation**.
- Recursively coalesce by checking if the "**buddy**" is free.

### Other ideas
- Major problem with above approaches: **scaling**.
- Use more complex data structures to address these costs.
