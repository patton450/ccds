# ccds - Concurrent C Data Structures

ccds is a C library that allows thread safe data structures.

## Instilation
Downnlaod, fork, or brach this repo.
Navigate to the directory and run

```bash
$make
```
link files with GCC like so:

```bash
$gcc -Lpath_to_libccds.so -lccds your_file.c
```
using any other gcc flags you need as well.

## usage

Creating an array that has space for 10 elements, with no error reporing and no user defined memory allocation.

```c
array * a = array_new(10, NULL, NULL);
```

## Goal:
    Provide a library of thread safe data structures written in C.
    Make the library API simple and consistent. Maintain clean, 
    simple to understand code, while providing protability and
    the user's ability to decide how they are allocating memory.



