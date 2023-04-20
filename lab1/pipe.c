#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

  int fd = open("my_new_file.txt", O_APPEND | O_WRONLY | O_CREAT); // opens a new file
  // fd points to the txt file now
  printf("%d", fd);
  if (fd < 0)
    printf("Error opening the file\n");

  dup2(fd, STDOUT_FILENO); // now the STDOUT_FILENO points to "my_new_file.txt"
  printf("This message will be printed to the my_new_file.txt, instead of the "
         "console.\n");

  return 0;
}
