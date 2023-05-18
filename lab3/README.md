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
In the `hash_table_v1_add_entry` function, I added the mutex at the very beginning of the function and at the very end of the function, checking that the mutex function did not return with an error. This guarantees correctness because all calls modifying the state of global values of type `list_entry` are part of the critical section, and these calls occur throughout the function.

### Performance
```
>./hash_table_tester -t 8 -s 50000
#Generation: 105,372 usec
#Hash table base: 3,109,089 usec
#  - 0 missing
#Hash table v1: 3,759,556 usec
#  - 0 missing
```

Version 1 is slower than the base version. This is because the entirety of the `hash_table_add_entry` function is locked whenever a new entry is added to the hash table. If another thread is also trying to add an entry, it must wait for the thread currently executing the function before it can run. Since the base version runs in serial, it never has to wait before using the `hash_table_add_entry` function and it will finish faster.

## Second Implementation
In the `hash_table_v2_add_entry` function, xxx

### Performance


This time the speed up is xxx

## Cleaning up
```
> make clean
```
