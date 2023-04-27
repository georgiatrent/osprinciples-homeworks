## UID: XXX
(IMPORTANT: Only replace the above numbers with your true UID, do not modify spacing and newlines, otherwise your tarfile might not be created correctly)

## Pipe Up

This program takes in a variable amount of arguments, taking the first argument following the executable and running it as if it were a shell command. The output of this command is then used as the input of the second argument. The last argument in the string outputs to the terminal once executed.

## Building

Build the program using: make

## Running

Run the program using:
./pipe arg1 arg2 arg3 ...
Without any arguments following ./pipe, the program will exit.
./pipe ls wc
This will use the output of ls (which outputs a list of the contents in the current directory) as the input of wc (which counts the words in its input and outputs the number). Your output should be number of lines, words, and characters in ls's output.

## Cleaning up

Clean up the extra files by running: make clean
