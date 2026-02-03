# Mechanism: Limited Direct Execution (LDE)

## Time-Sharing (CPU Multiplexing)
**Time-sharing**: run one process for a little while, then run another one, and so forth.

### Key challenges
1. **Performance**: minimum overhead  
2. **Control**: how does the OS retain control over the CPU?

Thus, the core problem is: **How to efficiently virtualize the CPU while retaining control.**


## Basic Technique: Limited Direct Execution
- **Direct execution**: simply run the user's code directly on the CPU → **fast**
- The **limited** part is hard:
  1. How can the OS ensure the program is legit (i.e., doesn’t do anything **illegal**)?
  2. How can the OS **stop** it and **switch** to another process to do time-sharing?


## Aside: Why Do System Calls Look Like Procedure Calls?
Because they **are** procedure calls — but with a **trap instruction**.

When you call `open()`, you are calling a **procedure** in the C library.
- The library follows a **calling convention** agreed upon with the kernel:
  - places the **arguments** and the **system-call number** in well-known locations
  - then executes a **trap** instruction


# Problem #1: Restricted Operations

## The Problem
How can a process perform **restricted operations** without being given complete control over the system?

## The Solution: Processor Modes
Hardware provides at least two modes:

- **User mode**: processes are restricted  
  - e.g., cannot issue **I/O requests**, cannot execute **privileged/restricted instructions**
- **Kernel mode**: the OS runs here and can do anything  
  - e.g., **privileged operations**, **I/O**, **restricted instructions**

## Carefully Exposing Functionality to User Programs
Expose only specific services via **system calls** (modern hardware supports this), such as:
- **I/O**
- **create/destroy processes**
- **IPC** (inter-process communication)
- **allocating memory**

## How a System Call Happens (Trap → Kernel → Return)
1. User program executes a **trap instruction**
2. CPU jumps into the kernel and raises privilege level to **kernel mode**
3. Kernel performs the requested work for the user process
4. When finished, OS executes a **return-from-trap** instruction
5. Control returns to the calling user program and privilege level drops back to **user mode**


## Aside: What Is a Kernel Stack?
Each process has **two stacks** conceptually:

- **User stack**
  - used in **user mode**
  - stores function call frames / local variables
  - lives in the process’s **user address space**
- **Kernel stack**
  - used in **kernel mode** (during system calls, interrupts, traps)
  - lives in **kernel memory**
  - **not accessible** to user code

## What Must the Hardware Do on a Trap?
Hardware must save enough state to allow returning to the caller later:
- save **context** to the **per-process kernel stack**
- `return-from-trap` will restore (pop) saved values and resume execution


## How Does the Trap Know Which Kernel Code to Run?
### Trap Table (Initialized at Boot)
At **boot time**:
- OS initializes a **trap table** (via **privileged instructions**)
- OS tells the hardware where the trap table is
- CPU remembers the trap table location
- Hardware uses it to know where to jump on:
  - **system calls**
  - **interrupts**
  - other traps/exceptions

### System-Call Numbers
- each system call has a **system-call number**
- user code must place the desired number somewhere according to convention
- in the trap handler, the OS:
  - verifies the number is valid
  - dispatches to the correct handler

This indirection is **protection**: user code cannot jump freely into the kernel; it must request service via a number.


## Aside: Be Wary of User Inputs in Secure Systems
At the system call boundary, the OS must carefully validate user-supplied arguments:
- ensure arguments are properly specified
- reject malformed/unsafe requests


## LDE Protocol: Two Main Phases

### 1) Boot Time
- initialize the **trap table** (using privileged instructions)
- CPU stores its location for later trap/interrupt handling

### 2) Running a Process
When starting and running a program:
1. OS sets up a few things (e.g., allocate a new node on the **process list**, etc.)
2. OS uses **return-from-trap** to start the new process execution
3. When the user program issues a **system call**:
   - trap back into OS
   - OS handles it
   - return-from-trap back to user code
4. When the user program is done:
   - it returns from `main()`
   - control returns into some stub code, which calls the `exit()` system call
   - it traps into the OS
   - OS cleans up and finishes


# Problem #2: Switching Between Processes
AKA: how does the OS **regain control of the CPU**?

## Cooperative Approach: Wait for System Calls
In cooperative scheduling:
- OS trusts the user process to behave properly
- processes voluntarily transfer CPU to OS via `yield` system calls
- OR the process traps due to illegal behavior, such as:
  - divide by 0
  - accessing memory it shouldn’t access
  - executing a trap instruction

## Non-Cooperative Approach: The OS Takes Control
How to regain control **without** cooperation?

### Timer Interrupt
A **timer device** raises interrupts frequently:
- the running process is halted
- an **interrupt handler** in the OS runs
- OS regains control of the CPU
- hardware is responsible for saving enough state


## Saving and Restoring Context
Once the OS regains control, the **scheduler** decides:
- keep running the current process, or
- switch to a different one

If switching, a **context switch** occurs:
1. save registers for the current process
2. restore registers for the next process


# Complete Timeline (Very Important)
Below is the full timeline of a timer interrupt causing a switch from process **A** to process **B**:

0. Process **A** is running  
1. (**HW**) **timer interrupt** occurs  
2. (**HW**) save registers of `p_a` to **kernel stack** of `p_a`  
   - this saves the **user-mode context frame** of process A (as it was running in user mode)  
3. (**HW**) switch to **kernel mode** and jump to the **trap handler**  
4. (**OS**) handle the trap  
5. (**OS**) call `switch()` routine  
6. (**OS**) save registers of `p_a` to `proc_t` of `p_a`  
   - this saves the **kernel context frame** of process A (as it was running inside the kernel)  
   - there are **two "contexts"** per user process:
     - a **user-mode** context
     - a **kernel-mode** context
   - i.e., “save the **kernel thread context** of process A while the CPU is executing kernel code on its behalf”
   - if a timer interrupt preempts the kernel while it’s handling process A’s system call, the OS must save A’s **kernel context** so that when A is scheduled again it can resume the system call exactly where it left off  
7. (**OS**) restore registers of `p_b` from `proc_t` of `p_b`  
   - restoring process B’s **kernel-mode context**  
   - when `switch()` returns, it returns in process B’s **kernel thread**  
8. (**OS**) switch to **kernel stack** of `p_b`  
   - change CPU stack pointer so subsequent kernel pushes use process B’s kernel stack  
   - each process has its own kernel-stack region in kernel virtual memory  
   - a context switch leaves A’s kernel stack untouched and simply loads B’s saved stack pointer into SP, so the kernel starts using B’s kernel stack  
9. (**OS**) kernel finishes process B’s system call (if any)  
10. (**OS**) **return-from-trap** into `p_b`  
11. (**HW**) restore registers of `p_b` from kernel stack of `p_b`  
12. (**HW**) drop privilege to **user mode**, jump to `p_b`’s PC  
13. Process **B** starts running  
