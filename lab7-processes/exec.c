#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    puts("Start");

    pid_t id = fork();

    puts("Middle");
    if (id == 0) {
        puts("This is the child.");
        char *argv[] = {"/usr/bin/ls", NULL};
        char *envp[] = {NULL};
        execve(argv[0], argv, envp);
        puts("Child is over.");
    }

    wait(NULL);
    puts("End");
    return 0;
}