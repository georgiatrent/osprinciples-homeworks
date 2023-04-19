#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int fds[2];
  pipe(fds); // create a pipe of two ends

  int ret = fork();
  if (ret == 0) {
    // child says hello to parents
    // write(fd, char* the content to write, int bytes to write)
    sleep(5);
    write(fds[1], "hello, parent!", 14);
    //sleep(5);
    write(fds[1], "just one more thing!", 21);
  } else if (ret > 0) {
    char buffer[4096];
    // read(fd, char* place to dump read, int bytes to read)
    read(fds[0], buffer, 35);
    printf("%s\n", buffer);
  }
}
