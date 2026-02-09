# Scheduling: Proportional Share

This chapter is about **proportional-share scheduling**, a.k.a. **fair-share scheduling**.

## The Problem

How to share the CPU **proportionally**?

## Basic Concept: Tickets Represent Your Share

- **Tickets**: represent the share of a resource that a process should receive.

### How a Lottery Is Held

1. The scheduler knows how many tickets there are (e.g., **100**).
2. Each process gets assigned a number of tickets proportional to its share (e.g., **A: 75**, **B: 25**).
3. The scheduler picks a winning ticket (e.g., **0 to 99**).
4. Whichever process holds the winning ticket is the process that runs.

- **Randomness** → **probabilistic correctness**.

## Ticket Mechanisms

Mechanisms to manipulate **tickets**:

1. **Ticket currency**: currency allows a user with a set of tickets to allocate tickets among their own jobs. The system then converts said currency into the correct **global value**.
2. **Ticket transfer**: a process can temporarily hand off its tickets to another process. Useful in a **client/server** setting:
   1. Client asks a server to do some task on the client's behalf.
   2. Client hands off its tickets to the server to speed up.
   3. Task done.
   4. Server returns tickets to the client.
3. **Ticket inflation**: a process can temporarily raise or lower the number of tickets it owns. Useful in an environment where a group of processes **trust one another**.

## Implementation

### Data

1. A list of **processes**.
2. Each process gets assigned a **FIXED** number of **tickets**.

### Algorithm

1. Initialize a **counter** = 0.
2. Generate a random number `winner` (a.k.a. the **winning ticket**).
3. Walk through the process list.
4. Add each ticket value to `counter`, until the value exceeds `winner`.
5. Once that's the case, the current list element is the winner (and gets **scheduled**).

### Best Practice

- Organize the list in **sorted order**, from **highest** number of tickets to the **lowest**.
- Reduce number of list iterations.

## How to Assign Tickets

Open problem with **no definite answer**...

## Stride Scheduling

A **deterministic** fair-share scheduler.

### How It Works

1. Each job has a **stride**, inverse in proportion to the number of **tickets** it has.
2. Every time a process runs, we increment a counter for it (i.e., **pass value**) by its stride to track **global progress**.
3. The scheduler picks the process to run that has the **lowest pass value** so far.

### Advantage of Lottery Scheduling Over Stride Scheduling

- No **global state**.
- Easier to incorporate **new processes** in a sensible manner.

## The Linux Completely Fair Scheduler (CFS)

- Too much info, just read [this](https://github.com/Chaoses-Ib/Linux/blob/main/Kernel/Processes/Scheduling/Completely%20Fair%20Scheduler.md) instead (XD)

## Summary

- Proportional-share scheduling approaches: **lottery**, **stride**, and **CFS**.
- **CFS** is the most widely used fair-share scheduler in existence today.
- No scheduler is a panacea. Fair-share scheduler problems:
  1. Cannot deal with **I/O processes** very well.
  2. The open problem is still unsolved: how do you know how many **tickets** a process should be allocated, or to what **nice value** to set another process? Other schedulers (such as **MLFQ**) do so automatically.
- Where proportional-share scheduler is used:
  1. **Virtualized data center** (or **cloud**).
  2. Extended to other resources (e.g., proportionally share **memory**).
