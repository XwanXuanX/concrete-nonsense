#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
The output is like:
```
hello world (pid:7959)
hello, I am parent of 7960 (pid:7959)
hello, I am child (pid:7960)
```
*/
int main(int argc, char *argv[]) {
    // this is from the main
    // getpid() get the PID (process identifier of the current process, which is
    // the parent)
    printf("hello world (pid:%d)\n", (int)getpid());

    // fork it!
    // create a new process which is exact copy of the calling process
    // OS sees two p1 processes and both of them are about to return from
    // `fork()`
    int rc = fork();
    // the new processes doesn't start from `main()`, but started from `fork()`
    // the only thing that's different: the return value of `fork()`:
    //  * parent receives the PID of the child
    //  * child receives 0

    // thus we have two different cases to handle for each individually:

    if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int)getpid());
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
    }

    // the output order is not deterministic tho
    // the child and the parent can both be run and print first, depending on
    // scheduler's decision

    return 0;
}
