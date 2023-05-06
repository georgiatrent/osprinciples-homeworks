## UID: 105812649
(IMPORTANT: Only replace the above numbers with your true UID, do not modify spacing and newlines, otherwise your tarfile might not be created correctly)

## You Spin Me Round Robin

This program takes in a text file in a specified format containing information about various processes, and  outputs the average waiting time and the average response time expected under a kernel implementing a simple round robin scheduling system.

## Building

Build the program using: make

## Running

Run the program using:
./rr processes.txt n,
Where processes.txt can be any text file following the format:
4
1, 0, 7 //process ID, arrival time, time to complete process
2, 3, 4
3, 0, 1
4, 6, 4
...
and n can be any number (this will be the quantum number for RR implementation).

## Cleaning up

Clean up the extra files by running: make clean
