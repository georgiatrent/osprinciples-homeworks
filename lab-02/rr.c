#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
  u32 remaining_time;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */

  if(quantum_length<=0)
  {
    exit(EINVAL);
  }
  //** check if burst time is less than 0 for each process

  //&data - array of processes from the text file
  //      pid, arrival_time, burst_time
  //size - number of processes in the text file

  //DO NOT JUDGE ME, I HAVE RENAMED PROCESSES TO BE BABIES
  //BECAUSE I AM TRYING TO ENTERTAIN MYSELF
  u32 baby_running = 0;

  struct process *current_baby;
  struct process *running_baby;
  u32 theTime = 0;
  u32 babies_to_run = size + 30; //number of processes
  while(babies_to_run)
  {
    //Add babies to queue if they have arrived
    for(u32 i = 0; i < size; i++)
    {
      current_baby = &data[i];
      if(current_baby->arrival_time == theTime)
      {
        struct process *baby = current_baby;
        TAILQ_INSERT_TAIL(&list, baby, pointers);
        baby->remaining_time = baby->burst_time;
      }
    }
    if(!baby_running)
    {
      if(!TAILQ_EMPTY(&list))
      {
        running_baby = TAILQ_FIRST(&list);
        baby_running = 1;
        running_baby->remaining_time--;
        printf("This baby's remaining time is %i and pid is %i \n", running_baby->remaining_time, running_baby->pid);
        if(running_baby->remaining_time == 0)
        {
          baby_running = 0;
          TAILQ_REMOVE(&list, running_baby, pointers);
        }
      }
      else
      {
        printf("The queue is empty");
      }

    }




    babies_to_run--;
    theTime++;
  }



  //sort based on arrival time








  while(!TAILQ_EMPTY(&list))
  {
    printf("%i", TAILQ_FIRST(&list)->burst_time);
    TAILQ_REMOVE(&list, TAILQ_FIRST(&list), pointers);
  }


  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
