AUTHOR: Sean Tronsen
DATE: December 3rd, 2021

FILES INCLUDED
./scheduler.c
./structures.c
./structures.h

COMPILE INSTRUCTIONS
use the following command within a directory containing the above files
this assumes that you have the gcc compiler readily availible for use

gcc scheduler.c structures.c structures.h


USAGE PATTERN
./a.out <scheduler_type [FCFS | RR]> <[optional_quantum_as_int]>

NOTE THAT QUANTUM IS REQUIRED FOR THE RR ALGORITHM
IT IS ALSO COMPLETELY IGNORED FOR THE FCFS ALGORITHM

The program will then output further usage  instructions related to using stdin

KNOWN ERRORS
Currently none.