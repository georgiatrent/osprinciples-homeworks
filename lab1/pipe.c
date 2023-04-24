#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  int to_shell[2];
  pipe(to_shell);


  int ret = fork();
  if(ret == 0)
  {
    char buffer[4096];
    close(to_shell[1]);
    close(0);
    dup(to_shell[0]);
    close(to_shell[0]);
    // read(0, buffer, 300);
    // printf(buffer);
    execlp(argv[2], argv[2], NULL);
  }
  else if (ret > 0){
    close(to_shell[0]);
    close(1);
    dup(to_shell[1]);
    close(to_shell[1]);
    execlp(argv[1], argv[1], NULL);
    //write(1, "ls", 10);
    int pid = ret;
    int status = 0;
    waitpid(pid, &status, 0);
    //printf("hi baby");

  }
  else{
    printf("Error forking");
  }
  return 0;
}

