# ccds - Concurrent C Data Structures

ccds is a C library that allows thread safe data structures.

## Instilation
Downnlaod, or fork this repository.
Navigate to this directory, then run the make command.

```bash
$ make
```
This will generate `libccds.so` inside of `bin/` link files with GCC like so:
```bash
$ gcc -L./bin -lccds your_file.c
```
## Usage

Creating an array that has space for 10 elements, with no error reporing and no user defined memory allocation.

```c
array * a = array_new(10, NULL, NULL);
if(a == NULL) {
    printf("Error failed to allocate the array\n");
    exit(1);
}
```

Adding 10 integer pointers to the array:
```c
for(int i = 0; i < 10; i ++) {
    int * x = malloc(sizeof(int));
    if(x == NULL) {
        printf("Error, memory allocation failed for: %d\n", i);
        exit(1);
    }
    *x = i;
    if(!array_add_tail(a, x, NULL)) {
        printf("Error failed to add element: %d\n", i);
    }
}
```

## Goal:

Provide a library of thread safe data structures written in C.
    Make the library API simple and consistent. Maintain clean, 
    simple to understand code, while providing protability and
    the user's ability to decide how they are allocating memory.



