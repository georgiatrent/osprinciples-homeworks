#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int return_code = fork();
  if (return_code == 0) {
    printf("This is the child process!\n");
    execlp("ls", "ls", NULL);
    exit(0);
  } else if (return_code > 0) {
    printf("I am lazy parent, letting my child to ls the directory\n");
    printf("I will just wait for their report\n");
    int pid = return_code;
    int status = 0;
    waitpid(pid, &status, 0);
    printf("Child process exits with code: %d\n", WEXITSTATUS(status));
  } else {
    printf("Child process creation error! \n");
  }
  printf("They finished; Done!\n");
  return 0;
}

