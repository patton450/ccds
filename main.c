#include "src/array.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t cap = 100;
    array * a = array_new(cap, NULL, NULL);
    void ** data;
    int i = 0;
    ARRAY_FOREACH(a, data, *data = malloc(sizeof(int)); *((int *)(*data)) = i; printf("%p: %d\n", *data, i); i++;);

    i = 0;
    ARRAY_FOREACH(a, data, printf("Expected %d : Got %p %d\n", i, *data,*((int *)(*data))); free(*data); i++;);
    array_free(a, NULL);
}
