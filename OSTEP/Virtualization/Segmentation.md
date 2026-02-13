# Segmentation

## The Problem

How to support a **large address space** with a lot of **free space** between **stack** and **heap**.

## Segmentation: Generalized Base/Bounds

- instead of having just one **base** and **bounds** pair in the **MMU**, have a **base** and **bounds** pair per **logical segment** of the address space
- **segment**: contiguous portion of the address space (**code**, **stack**, **heap**)
- allows the **OS** to place each segment in different parts of **physical memory**
- avoid filling physical memory with unused **virtual address space**
- **MMU requirement**: a set of **3 base and bounds register pairs**
- the **bounds register** holds the **size** for each segment
  - tells the hardware exactly how many **bytes** are valid in this segment for **illegal access detection**
- thus the origin of the famous **"segmentation fault"**

## Which Segment Are We Referring To?

- How does the hardware know the **offset** into a segment, and to which segment an address refers?

### Explicit Approach

- chop up the address space into segments based on the **top few bits** of the **virtual address**
- hardware examines the **top few bits** to understand which segment the offset is referring to
  - and uses the corresponding **base** and **bounds** pair to relocate the virtual address

#### Issue: Limits the Use of the Virtual Address Space

- limits the use of the virtual address space
  - each segment is limited a **maximum size** (**16KB/4 = 4KB per segment**)
  - cannot grow a segment beyond that maximum

### Implicit Approach

- hardware determines the segment by noticing how the address was formed
  - e.g. if the address generated from **PC** -> must be **code segment**

## What About the Stack?

- critical difference: **stack grows backwards** — translation must proceed differently
- **MMU** also needs to know which way each segment grows (**add a bit**)

## Support for Sharing

- useful to share certain memory segments between address spaces (**code sharing** is common and still in use today)
- **MMU** also needs to track the **protection bits**
  - adds a few bits per segment (whether or not a program can **read** or **write** or **execute** a segment)
  - setting the **code segment** to **read-only** -> same code can be shared across multiple processes
    - each process still thinks it is accessing its own **private memory**
- same **physical segment** in memory could be mapped into multiple **virtual address spaces**
- the hardware has to check whether a particular access is **permissible**

## Fine-grained vs. Coarse-grained Segmentation

- **coarse-grained**: chops up the address space into large coarse chunks
- **fine-grained**: allowed for address space to consist of a large number of smaller segments
- **fine-grained segmentation** uses **hardware segment table**
  - supports a very large number of segments
  - more **flexible**

## OS Support

### Some New Issues

1. what should the OS do on a **context switch**?
   - save and restore the **segment registers**

2. **OS interaction** when segments grow
   - memory-allocation library use a **system call** to grow the heap (`sbrk()`)
   - OS provides more space:
     - if there are empty space adjacent, simply update the **segment size register** to the bigger size
     - otherwise, find a bigger empty slot and copy everything over! (and adjust the **base** and **bounds** registers)
   - the library can then allocate space for the new object
   - **NOTE**: OS could reject the request if no more **physical memory** is available

3. managing **free space** in physical memory
   - **external fragmentation**: physical memory becomes full of little holds of free space

   - **Solution1**: compact physical memory by rearranging the existing segments (**expensive**)
     1. OS stop running process
     2. copy data to one contiguous region of memory
     3. change their **segment register** values to point to the new physical locations

   - **Solution2**: **free-list management algorithm**
     - **best-fit**
     - **worst-fit**
     - **first-fit**
     - **buddy algorithm**

   - **external fragmentation** will always exist
