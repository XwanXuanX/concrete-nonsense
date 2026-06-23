# Synchronization & Data Transfer

## Problem

How to pass information between physical operators?

## Bad Solutions

- Temporary files
  - Need entire previous output to start executing the next node
  - Limited parallelization

- IPC 
  - Each operator as a process
  - OS scheduling and IPC cost

- Tree-structured plan $\Rightarrow$ iterative program with nested loops
  - Rule-base translation 
  - Complex

## Good Solution

[Volcano Model / Iterator Model](/architecture/iterator-model.md)

