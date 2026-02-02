# The Abstraction: Processes

## 1) Core Concept
- **Process** = a running program


## 2) The Big Problem: How to Provide the Illusion of Many CPUs?
- Provide the illusion of many CPUs by **virtualizing the CPU** through **time-sharing**.

### Aside: Mechanisms vs. Policies
- **Mechanisms** = low-level methods or protocols that implement a needed piece of functionality  
  - Example: **context switch**
- **Policies** = algorithms for making some kind of decision within the OS  
  - Example: a **scheduling policy**


## 3) The Abstraction: The Process (Machine State)
A process’s **machine state** describes what parts of the machine are important to the execution of the program:

1. **Memory** (its **address space**)
2. **Registers**
3. **Special registers**
   - **Program counter**
   - **Stack pointer**
   - **Frame pointer**
4. **I/O information**
   - e.g., a list of files the process currently has open


## 4) Process APIs
Process-related operations (calls programs can make):

1. **Create** a new process
2. **Destroy** existing processes
3. **Wait** for a process to stop running
4. **Misc control**
   - e.g., suspend and resume a process
5. **Status**
   - query status information of a process


## 5) Process Creation (How Programs Are Transformed into Processes)
Steps (high-level):

1. **OS loads the program’s code and static data** from disk into the program’s address space
   - In early OS, all data and code is loaded **eagerly**
   - In modern OS, data and code is loaded only when needed (**lazily**)
   - Need to understand **paging** and **swapping** (more on this later)

2. **Allocate the program’s run-time stack** and give it to the process
   - Probably fill in the parameters to the `main()` function (`argc`, and `argv`)

3. **Allocate the program’s heap**
   - Heap is small at first
   - As more request for memory via `malloc()`, OS may get involved and allocate more heap memory

4. **Do some initialization tasks**
   - Something related to I/O (3 open file descriptors by default: **std out**, **std in**, **std error**)

5. **Start the process running** at `main()`


## 6) Process States
Common states:

- **Running**: executing instructions
- **Ready**: ready to be run, but OS choose not to run it for now
- **Blocked**: has performed some operation that makes it not ready to run until some other event takes place  
  - Example: initiate an **I/O request**


## 7) Data Structures (OS Bookkeeping)
What the OS needs to keep track of processes:

- For each process, need to save its **register context** when **context switching**
- **Process list**: OS needs to track each process and its states

### Aside: Process List and PCB
- The **process list** (or **task list**) holds all the running programs
- **PCB (process control block)** (or **process descriptor**): individual structure that stores info about a process


## 8) Key Process Terms
1. **Process** = major OS abstraction of a running program  
2. **Process APIs** = calls programs can make related to processes  
3. **Process states** = includes running, ready, blocked, and others  
4. **Process list and PCB** = described above  
