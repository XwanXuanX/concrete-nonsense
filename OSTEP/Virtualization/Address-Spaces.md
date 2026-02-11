# The Abstraction: Address Spaces

## The Address Space

- An easy-to-use abstraction of **physical memory**
- Contains all of the **memory state** of the program:
  - **code**
  - **stack**
  - **heap**

### Address Space Structure

1. **Code** is static, and lives at the **top**
2. **Heap** and **stack** may grow
   - Placed at opposite ends and grow towards each other
   - The placement of **stack** and **heap** is just a convention

- When multiple **threads** co-exist, no nice way to divide the **address space**

## The Problem

- How to virtualize a single, **physical memory**

## Goals of a VM System

- **Transparency**: **VM** should be implemented such that it's invisible to the running program
- **Efficiency** in both time and space: need **hardware support** (**TLBs**)
- **Protection**: protect **processes** from one another and the **OS** itself — **isolation**
