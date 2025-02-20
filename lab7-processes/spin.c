
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    printf("Start\n");

    pid_t pid = fork();

    if (pid > 0) {
        printf("This is the parent process, getpid: %d, pid: %d\n", getpid(), pid);
    }
    else if (pid == 0) {
        printf("This is the child process, getpid: %d, pid: %d\n", getpid(), pid);
    }
    else {
        printf("Fork failed\n");
    }

    wait(NULL);
    printf("End\n");
    return 0;
}