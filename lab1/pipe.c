#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("I am a lazy program, letting other program do work.\n");
	printf("I will just wait for their report\n\n");
	execlp("ls", "ls", "-a", "-l", NULL);
	printf("They finished; Done!\n");
	return 0;
}

