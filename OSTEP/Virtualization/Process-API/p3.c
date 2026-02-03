#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * what does the `exec()` system call do:
 * given the name of an exe and arguments, it loads code and static data from
 * that exe and overwrite its current code and current static data segment. heap
 * and stack and other memory space are re-initialized. then the OS runs that
 * program.
 *
 * it does not create new processes, it transforms current process.
 */

/*
Output may look like:
```
hello world (pid:12571)
hello, I am child (pid:12575)
      29     120     969 p3.c
hello, I am parent of 12575 (wc:12575) (pid:12571)
```
*/
int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc = fork();
    assert(rc >= 0);

    // again, handle 2 cases

    if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        char *myargs[3];
        myargs[0] = strdup("wc");   // program: "wc" (word count)
        myargs[1] = strdup("p3.c"); // argument: file to count
        myargs[2] = NULL;           // marks end of array
        execvp(myargs[0], myargs);  // runs word count
        // exec NEVER returns!
        printf("this shouldn't print out");
    } else {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc,
               (int)getpid());
    }
    return 0;
}