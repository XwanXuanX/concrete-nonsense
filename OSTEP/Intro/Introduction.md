# Introduction to Operating Systems


## 1) What an Operating System Does
An **Operating System (OS)** acts as a **resource manager**:
- Allocates and coordinates hardware resources (**CPU**, **memory**, **disk**, **I/O devices**) among many programs.
- Provides useful **abstractions** so programs can use hardware without dealing with low-level details.


## 2) Virtualizing the CPU
### Core Idea
The OS makes it *seem* like many programs run at the same time on one CPU by **time-sharing**.

### Scheduling: Who Runs Next?
Two key concepts:
- **Mechanisms**: the *capabilities* that make multiplexing possible  
  - e.g., context switching, timers/interrupts
- **Policies**: the *decision rules* for choosing what to do  
  - e.g., deciding **which process to run next** (**scheduling**)


## 3) Virtualizing Memory
### Core Idea
Each process thinks it has its own private memory called a **virtual address space** (or just **address space**).

### How It Works (High-Level)
- A process uses **virtual addresses**
- The OS (with hardware support) maps these to **physical memory**
- This mapping makes memory look “magical” and private per process


## 4) Concurrency
### Why Concurrency Causes “Weird Outcomes”
Multi-threaded programs can behave unexpectedly because the CPU executes **one instruction at a time**, and operations you *assume* are single steps often are not.

### Example: Incrementing a Shared Counter Is Not Atomic
Incrementing typically expands into multiple instructions:
1. **load** value from memory into a register  
2. **increment** the value in the register  
3. **store** the value back to memory  

Since these steps are not **atomic**, interleavings between threads can cause lost updates (race conditions).


## 5) Persistence (Storage)
### Core Idea
The OS provides long-term storage via the **file system** (a “disk manager”).

### OS as a Standard Interface to Devices
- Programs interact with devices through OS-provided interfaces
- Hardware details are handled by **device drivers**

### Ensuring Correctness on Disk (Write Protocols)
Common techniques for safer updates:
- **journaling**
- **copy-on-write (CoW)**


## 6) OS Design Goals
1. **Abstraction**: make the system easy to use  
2. **High performance**: minimize overhead introduced by the OS  
3. **Protection / Isolation**: prevent processes from interfering with each other  
4. **Reliability**: OS failure can crash everything running on it  
5. **Energy efficiency**: reduce power use  
6. **Security**: resist attacks and prevent unauthorized access  
7. **Mobility / Portability**: run across devices of different sizes and capabilities
