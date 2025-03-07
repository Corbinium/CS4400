#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  if (argc < 4) {
    fputs("[-] ERROR: Not enough arguments\n", stderr);
    return EXIT_FAILURE;
  }
  char* writer_args[] = {argv[1], NULL};
  char* reader_args[] = {argv[2], NULL};
  char* third_args[] = {argv[3], NULL};

  int fds[2];
  pipe(fds);
  int fds2[2];
  pipe(fds2);
 
  // First child
  if (fork() == 0) {
    dup2(fds[1], STDOUT_FILENO);
    close(fds[0]);
    close(fds[1]);
    close(fds2[0]);
    close(fds2[1]);
    if(execv(writer_args[0], writer_args) == -1)
      exit(1);
  }
  // Second child
  if (fork() == 0) {
    dup2(fds[0], STDIN_FILENO);
    dup2(fds2[1], STDOUT_FILENO);
    close(fds[0]);
    close(fds[1]);
    close(fds2[0]);
    close(fds2[1]);
    if(execv(reader_args[0], reader_args) == -1)
      exit(1);
  }
  // Third child
  if (fork() == 0) {
    dup2(fds2[0], STDIN_FILENO);
    close(fds[0]);
    close(fds[1]);
    close(fds2[0]);
    close(fds2[1]);
    if(execv(third_args[0], third_args) == -1)
      exit(1);
  }

  // Parent
  close(fds[0]);
  close(fds[1]);
  close(fds2[0]);
  close(fds2[1]);
  int status;
  wait(&status);
  wait(&status);
  wait(&status);
}

