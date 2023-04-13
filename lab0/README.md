## UID: 105812649

(IMPORTANT: Only replace the above numbers with your true UID, do not modify spacing and newlines, otherwise your tarfile might not be created correctly)

# A Kernel Seedling

My kernel module is able to start, create a file inside /proc which returns the number of processes, and then be removed.

## Building

Running the following command builds my kernel module:
"make". Running "sudo insmod proc_count.ko" inserts my kernel module into the kernel.

## Running

The following command inserts my kernel module into the kernel:
"sudo insmod proc_count.ko". The results can be observed by running the command "cat /proc/count", which returns a single integer describing the number of current running processes.



## Cleaning Up

Running "make clean" followed by "sudo rmmod proc_count" cleans up the files and then removes the kernel module.

## Testing

I tested my kernel module on 5.14.8-arch1-1.

