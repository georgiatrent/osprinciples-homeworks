# Hash Hash Hash
Georgia Trentalange, UID:105812649
This lab implements two different strategies of using multiple threads while manipulating a hash table.

## Building
```
> make
```
    This creates the executable.
    It may be well-advised to preface `make` with `make clean` to ensure that there are no other lingering executable files.

## Running
```
>./hash-table-tester -t 8 -s 50000
#Generation: *variable number 1*
#Hash table base: *variable number 2* usec
#    - 0 missing
#Hash table v1: *variable number 3, > vn2* usec
#    - 0 missing
#Hash table v2: *variable number 4, < vn2 by criteria* usec
#    - 0 missing
```

## First Implementation
In the `hash_table_v1_add_entry` function, I added the mutex at the very beginning of the function and at the very end of the function, checking that the mutex function did not return with an error. This guarantees correctness because all calls modifying the state of global values such as the type `hash_table_entry` are part of the critical section, and once one thread locks the critical section no other threads can change the state of these entries.

### Performance
```
>./hash_table_tester -t 8 -s 50000
#Generation: 105,372 usec
#Hash table base: 3,109,089 usec
#  - 0 missing
#Hash table v1: 3,759,556 usec
#  - 0 missing
```

Version 1 is slower than the base version. This is because the entirety of the `hash_table_add_entry` function is locked using the same mutex whenever a new entry is added to the hash table. If another thread is also trying to add an entry, it must wait for the thread currently executing the function before it can run. Since the base version runs in serial, it never has to wait before using the `hash_table_add_entry` function and it will finish faster.

## Second Implementation
In the `hash_table_v2_add_entry` function, I made sure to still place my lock around the areas which modify the global types. However, this implementation differs from v1 as each hash_table_entry type has its own mutex. This still guarantees correctness because it ensures that other threads won't attempt to add to the same entry at the same time, but it also allows multiple threads to use the function at the same time as most often the entries each thread attempts to add to will be different from each other and will thus have different mutexes and different critical sections.

### Performance
This speeds up performance significantly because it allows for the multiple threads to actually be utilized. Because each hash_table_entry has its own mutex, when a  hash_table_entry gets a new element, a thread will be able to use the function at the same time a different hash_table_entry gets a new element.

This time the speed up is:
Generation: 72,661 usec
Hash table base: 1,103,766 usec
  - 0 missing
Hash table v1: 1,435,161 usec
  - 0 missing
Hash table v2: 384,271 usec
  - 0 missing
## Cleaning up
```
> make clean
```
