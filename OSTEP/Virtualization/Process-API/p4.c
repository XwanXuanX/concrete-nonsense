#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int rc = fork();

    if (rc == 0) {
        // I'm a child process now!
        // child: redirect standard output to a file
        close(STDOUT_FILENO);
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        // open file descriptors are kept open across the exec() call
        // so the file p4.output will stay open for `wc` program.

        // WHY THE REDIRECTION WORKS???
        // because of how OS manages file descriptors
        // Unix systems start looking for free file descriptors at zero.
        // and it will find STDOUT_FILENO and thus get assigned whe open() is
        // called.

        // the point of this example is that it shows the power of having fork()
        // and exec() separately for example, you can setup the environment for
        // the program that's about to be run (such as redirecting its output)

        // now exec "wc"...
        char *myargs[3];
        myargs[0] = strdup("wc");   // program: "wc" (word count)
        myargs[1] = strdup("p4.c"); // argument: file to count
        myargs[2] = NULL;           // marks end of array
        execvp(myargs[0], myargs);  // runs word count
    } else {
        // parent goes down this path (original process)
        // I'm the parent, I'm waiting for my child to terminate
        int wc = wait(NULL);
        assert(wc >= 0);
    }
    return 0;
}