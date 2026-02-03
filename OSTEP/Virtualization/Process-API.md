# Process APIs

## 1) The Problem
- How to **create** and **control** processes  
- Refer to this chapter’s code files for more detailed notes


## 2) Key Process API Terms
1. A process’s name = **PID**
2. `fork()` system call is used to create a new process  
   - creator is called the **parent**  
   - the new process is the **child**
3. `wait()` system call allows a parent to **block** until the child’s termination
4. `exec()` system call **transforms a process** to another and runs it freshly
5. A Unix shell commonly uses `fork()`, `wait()`, and `exec()` to launch user commands  
   - the separation between `fork()` and `exec()` enables features like **I/O redirection** without changing anything about the programs being run
6. Process control is available in the form of **signals**
7. The OS allows multiple **users** onto the system  
   - a user can only control their own processes
8. The **superuser** can control all processes (**root**)
