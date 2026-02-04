# Scheduling: The Multi-level Feedback Queue

## Overview
**Multi-level Feedback Queue (MLFQ)** is one of the most well-known approaches to scheduling.

MLFQ tries to address two problems:
1. Optimize **turnaround time**
2. Minimize **response time**

## The core problem
How to schedule **without perfect knowledge** (e.g., job length)?


## MLFQ: Basic rules
MLFQ:
- Has a number of distinct queues, each assigned a different **priority level**
- Ensures a job that is ready to run is on **a single queue**
- Uses priorities to decide which job should run (**higher priority** runs sooner)
- If multiple jobs are in one queue (same priority), use **Round Robin (RR)**
- Varies the priority of a job based on its **observed behavior** (learn from history to predict the future)

Thus, the rules:
1. If `priority(A) > priority(B)`, then **A runs** and **B doesn't**
2. If `priority(A) == priority(B)`, then **A and B run in RR**


## Attempt #1: How to change priority
### Workload assumption
- The workload is a mix of:
  - **Interactive jobs** that are short-running
  - Long-running **CPU-bound** jobs (interactive time is not important for them)

### Definitions
- Define **Job's allotment**: the amount of time a job can spend at a given priority level before the scheduler reduces its priority
- Assume allotment is equal to a single **time slice**

### Proposed rules
3. When a job enters, place it at the **highest priority queue**
4. If it uses up its allotment while running, **reduce priority** (move one queue down)
5. If it gives up the CPU (I/O) before allotment is up, it **stays at the same priority level** (allotment is reset)

### Intuition behind the rules
- The scheduler doesn't know whether a job will be **short** or **long**
- It first assumes the job might be short, thus gives it **high priority**
- If short, then good: we guessed correctly
- If not, then it will slowly move down the queues, thus soon prove itself to be a **long-running job**
- MLFQ approximates **SJF**

### Problems
- **Starvation**: if there are too many interactive jobs, they may combine to consume all CPU time, and long-running jobs will starve
- Susceptible to attacks that **game** the scheduler to get more than fair-share of resources
- A program may change its behavior over time; a CPU-bound program might change to interactive


## Attempt #2: The priority boost
### New rule
6. After some time **S**, move **all the jobs** in the system to the **topmost queue**

### What it solves
This solves two problems:
1. Processes are guaranteed not to **starve**
2. If a CPU-bound job becomes interactive, the scheduler treats it properly

### Open question
But what should **S** be set to? Guess it!


## Attempt #3: Better Accounting
### Goal
How to prevent **gaming** of the scheduler?

### Motivation
- Intuitively, the problem comes from the rule that lets a job retain its priority by relinquishing the CPU before its allotment expires

### Rewrite of rules 4 and 5
Rewrite rule 4 and 5 to the following:
6. Once a job uses up its allotment at a given level, its priority is reduced, **regardless of how many times it has given up the CPU**


## Tuning MLFQ and other issues
- How to parameterize such a scheduler?
- No easy answer: you can only do experiments on workloads to find out


## Summary: Refined set of MLFQ rules
1. If `priority(A) > priority(B)`, then **A runs** and **B doesn't**
2. If `priority(A) == priority(B)`, then **A and B run in RR** using the time slice of the given queue
3. When a job enters, place it at the **highest priority queue**
4. Once a job uses up its allotment at a given level, its priority is reduced, **regardless of how many times it has given up the CPU**
5. After some time **S**, move **all the jobs** in the system to the **topmost queue**
