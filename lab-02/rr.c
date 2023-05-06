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
  u32 waiting_time;
  bool baby_has_run;

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
  struct process *running_baby = NULL;
  u32 theTime = 0;
  u32 babies_to_run = size; //number of processes
  u32 active_baby_counter;
  total_waiting_time = 0;
  total_response_time = 0;
  u32 whats_babay_time = 0;

  while(babies_to_run)
  {
    //printf("Time: %i", theTime);
    //Add babies to queue if they have arrived
    for(u32 i = 0; i < size; i++)
    {
      current_baby = &data[i];
      if(current_baby->arrival_time == theTime)
      {
        //printf(" A new baby has arrived.");
        current_baby->baby_has_run = false;
        struct process *baby = current_baby;
        TAILQ_INSERT_TAIL(&list, baby, pointers);
        baby->remaining_time = baby->burst_time;
      }
    }
    //for each clock tick, check if:
    //  (1) A baby is running
    //      (a) Baby has reached end of its time
    //  (2) Time is divisible by quantum time
    //  (3)
    if(!baby_running)
    {
      if(!TAILQ_EMPTY(&list))
      {
        struct process *hi_baby = TAILQ_FIRST(&list);
        running_baby = hi_baby;
        TAILQ_REMOVE(&list, hi_baby, pointers); //bye baby
        baby_running = running_baby->pid;
        active_baby_counter = 0;
        //printf("It's baby %i's turn.", baby_running);
      }
      else
      {
        theTime++;
        continue;
      }
    }

    if(theTime==0)
    {
      theTime++;
      continue;
    }
    // if(running_baby->remaining_time == 0)
    // {
    //   babies_to_run--;
    //   baby_running = 0;
    // }

    if(baby_running)
    {

      active_baby_counter++;
      running_baby->remaining_time--;
      if(!running_baby->baby_has_run)
      {
        total_response_time+= (theTime - running_baby->arrival_time - 1);
        running_baby->baby_has_run = true;
        //printf(" response time is %i after process no. %i", (theTime - running_baby->arrival_time), running_baby->pid);
      }
      //running_baby->baby_has_run = true;
      whats_babay_time++;
      if(running_baby->remaining_time == 0)
      {
        //printf(" This baby has run to completion.");
        total_waiting_time+=(theTime - running_baby->burst_time - running_baby->arrival_time);
        babies_to_run--;
        baby_running = 0;
        whats_babay_time = 0;
      }
      else if(whats_babay_time != quantum_length && running_baby->remaining_time > 0)
      {
        //printf("Keep going baby no. %i .\n", running_baby->pid);
      }

      if(whats_babay_time == quantum_length)
      {
        //printf("It's not your turn anymore, baby no. %i, remain: %i", running_baby->pid, running_baby->remaining_time);
        whats_babay_time = 0;
        TAILQ_INSERT_TAIL(&list, running_baby, pointers);
        baby_running = 0;
      }
    }
    //printf("\n");
    theTime++;
  }

  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
