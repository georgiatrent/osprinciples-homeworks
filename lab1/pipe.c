#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if(argc <= 1)
  {
    exit(EINVAL); //just ./pipe
  }
  else if(argc == 2)
  {
    if(execlp(argv[1], argv[1], NULL) == -1) //only two arguments, ./pipe and something else
    {
      exit(errno);
    }
  }

  int ret = 0;
  int fds[2];
  int in = STDIN_FILENO;

  for(int i = 1; i < argc; i++)
  {
    if (pipe(fds) == -1)
    {
      exit(errno);
    }
    ret = fork();
    if(ret == 0)
    {
      //child process

      if(close(fds[0])) //close read end
      {
        exit(errno);
      }
      if(dup2(in, STDIN_FILENO) == -1) //set in to input
      {
        exit(errno);
      } //change where you get input
      if(argv[i + 1] != NULL) //only change where output goes if
      {//there's another argument, otherwise send to terminal
        if(dup2(fds[1], STDOUT_FILENO) == -1) //change output to write end
        {
          exit(errno);
        }
      }
      if(execlp(argv[i], argv[i], NULL) == -1)
      {
        exit(errno);

      };
    }
    else if(ret > 0)
    {
      if(close(fds[1]))
      {
        exit(errno);
      }
      if(close(in))
      {
        exit(errno);
      }
      in = fds[0];

      int pid = ret;
      int status = 0;
      waitpid(pid, &status, 0);

      if(WIFEXITED(status))
      {
        int getOut = WEXITSTATUS(status);
        if(getOut != 0)
        {
          exit(getOut);
        }
      }

    }
    else
    {
      exit(errno);
    }
  }

  return 0;
}

