#include <stdio.h>
#include <stdlib.h>
#include "include/errors.h"

void occur_error(const char* error_name, int* line_number, const char* file){
    printf("%s - File: %s, Line: %d\n", error_name, file, *line_number);
}

void fatal_error(const char *msg) {
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(1);
}