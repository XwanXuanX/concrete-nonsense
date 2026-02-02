#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc = fork();
    assert(rc >= 0);

    // same situation as p1.c
    // thus we have two different cases to handle for each individually:

    if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int)getpid());
        sleep(1);
    } else {
        // parent goes down this path (original process)
        // the wait system call will block the parent process and wait until the
        // child process has completed

        // parent is blocked
        int wc = wait(NULL);
        // parent is unblocked

        // now the output is proved to be deterministic:
        // if child runs first, then good
        // if parent runs first, then it will wait until child finishes -> child
        // runs first, then good

        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc,
               (int)getpid());
    }
    return 0;
}