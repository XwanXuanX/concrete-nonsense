# Interlude: Memory API

## Problem

- How to allocate and manage **memory**

## Type of Memory

- **Stack**
  - Managed implicitly by the **compiler** (aka **automatic memory**)
- **Heap**
  - **Long-lived memory**, managed explicitly
  - Example:
    - `int *x = (int *)malloc(sizeof(int));`
  - Note: both **stack** and **heap** allocations happens on this line

## Common Errors

1. Forgetting to allocate **memory**
2. Not allocating enough **memory** (**buffer overflow**)
3. Forgetting to **initialize** allocated memory
4. Forgetting to **free** memory (**memory leak**)
   - Still a problem with modern languages with **garbage collector**
   - But if your program is **short-lived**, you can not call `free()` since the **OS** will free it for you
5. Freeing memory before you are done with it (**dangling pointer**)
6. Freeing memory repeatedly (**double free**): **undefined**
7. Calling `free()` incorrectly (**invalid frees**)

## Underlying OS Support

- `malloc()` and `free()` are **not system calls**, but rather **library calls**
- Built on top of some **system calls**:
  - **`brk`**
    - Change the location of the program's **`break`** — the location of the end of the **heap**
    - Takes one argument (the address of the new **break**) and increase or decrease the **break**
  - **`sbrk`**
    - Similar to **`brk`**, but is passed an **increment**
  - **`mmap()`**
    - Create an **anonymous memory region** within your program — a region which is not associated with any **file** but rather with **swap space**
    - Can then also be treated like a **heap** and managed as such

## Other Calls

- **`calloc()`**: initialize with **zeros**
- **`realloc()`**: makes a new larger region of **memory**, copies the old region into it, and return the pointer to the new region
