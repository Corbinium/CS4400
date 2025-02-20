#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  int firstchild = fork();

  if( firstchild == 0)
  {
    // Do work here to fork to create target pstree diagram

    // fork();

    // if (fork()) {
    //     fork();
    // }

    // if (!(fork())) {
    //     if (fork()) {
    //         fork();
    //     }
    // }

    // int i = 0;
    // for (i = 0; i < 3; i++) {
    //     fork();
    // }

    int i = 0;
    for (i = 0; i < 3; i++) {
        if (!(fork()) && i == 2) {
            fork();
        }
    }

    // Put all calls to fork() above this line, and don't modify below.

    sleep(2); /* required so that execve will run before the process quits, don't do this sort of thing in real code */
    return 0;
  }
  else 
  {
    sleep(1); /* make it so child finishes forking before calling execve */


    // parent process calls execve on pstree

    char pid[20]; // This is where we will put the pid argument to the pstree execution
    char* args[] = {"/usr/bin/pstree", "-p" , pid, NULL};
    char* environ[] = {NULL};

    sprintf(pid, "%u", firstchild);
    execve( args[0], args, environ);
  }
}
