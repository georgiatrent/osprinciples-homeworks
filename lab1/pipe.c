#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if(argv[1] != NULL){
    char* argument = argv[1];
    execlp(argument, argument, NULL);
  }

  return 0;
}
