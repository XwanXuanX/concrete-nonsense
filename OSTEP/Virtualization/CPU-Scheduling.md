# Scheduling: Introduction

## The problem
How to develop a scheduling policy.


## Workload assumptions
* Understanding the workload is critical for building scheduling policies.
* Assumptions about processes (jobs):
  1. Each job runs for the same amount of time.
  2. All jobs arrive at the same time.
  3. Once started, each job runs to completion.
  4. All jobs only use the CPU (no I/O activity).
  5. The runtime of each job is known.


## Scheduling metrics
* **Turnaround time**:  
  $$T_{\text{turnaround}} = T_{\text{completion}} - T_{\text{arrival}}$$
* By assumption 2, $T_{\text{arrival}} = 0$, thus:  
  $$T_{\text{turnaround}} = T_{\text{completion}}$$
* Turnaround time is a **performance** metric; there is also a **fairness** metric.


## FIFO (First-In, First-Out / First-Come, First-Serve)
* Simple and easy to implement.
* If jobs are the same length, FIFO performs well.
* With different job lengths, FIFO can perform poorly:
  * If the first arriving task is **VERY VERY LONG**,
  * it delays the execution of other tasks,
  * causing the **convoy effect**.


## SJF (Shortest Job First)
* **Non-preemptive**.
* Runs the shortest job first, then the second shortest, and so on.
* Given the assumption that all jobs arrive at the same time, SJF is the **optimal** scheduling policy.
* If jobs arrive at different times, SJF can perform poorly:
  * A **VERY VERY LONG** job A arrives first and starts executing.
  * Even if shorter jobs B and C arrive later, they must still wait until job A finishes.
  * This creates the same **convoy problem**.

### Aside: preemptive schedulers
* **Non-preemptive schedulers**: run each job to completion before considering the next.
* **Preemptive schedulers** (all modern schedulers): stop one process in order to run another.


## STCF (Shortest Time-to-Completion First)
* Preemptive version of SJF, also known as **preemptive shortest job first (PSJF)**.
* Relaxes assumption 3 by allowing preemption.
* With timer interrupts and context switching, the scheduler can preempt job A and run another job.
* Whenever a new job enters the queue, the scheduler selects the job with the **least time remaining** and runs it.
* Under the assumptions, STCF is **provably optimal**.


## New metric: Response time
* Time from when the job arrives to the **first time it is scheduled**.
* **Response time**:  
  $$T_{\text{response}} = T_{\text{first\_run}} - T_{\text{arrival}}$$
* STCF is not good for response time and **interactivity**.


## Round Robin (RR)
* Instead of running to completion, runs a job for a **time slice** (also called a **scheduling quantum**).
* Also called **time slicing**.
* The length of the time slice is important:
  * Shorter slices -> better response time under RR, but increases overhead due to frequent context switching.
  * Context switching is expensive: machine state must be saved (flushed out) and new state restored (brought in), creating significant overhead.
* RR is the **worst policy for turnaround time**.
* More generally, any policy that is **fair** tends to perform poorly on turnaround time.
* **Response time vs. turnaround time** is a key trade-off.


## Incorporating I/O
* Relaxes assumption 4: processes can perform I/O now.
* When a process issues an I/O request, it becomes **blocked**, and the scheduler should let others run.
* When the I/O completes, an interrupt is raised -> the OS must decide which job to schedule next.

### A common approach
* For an **I/O-intensive job** (alternating CPU <-> I/O), treat each CPU sub-job as an independent job.
* STCF will choose the shorter CPU sub-job.
* When the sub-job issues an I/O request and blocks, other CPU-intensive jobs can run.
* When the I/O finishes, a new sub-job is submitted and can **preempt** the currently running CPU-intensive job.
* This approach enables overlap of I/O and CPU utilization.


## No More Oracle
* The OS usually knows very little about the length of each job.
* More on this later!
