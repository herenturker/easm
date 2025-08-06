#include <stdio.h>
#include <stdlib.h>
#include "include/errors.h"

/**
 * @brief Prints a non-fatal error message with file and line context.
 * 
 * @param error_name A descriptive error message.
 * @param line_number Pointer to the current line number where the error occurred.
 * @param file The filename where the error occurred.
 */
void occur_error(const char* error_name, int* line_number, const char* file){
    printf("%s - File: %s, Line: %d\n", error_name, file, *line_number);
}

/**
 * @brief Prints a fatal error message and terminates the program.
 * 
 * This function does not return.
 * 
 * @param msg The fatal error message to display.
 */
void fatal_error(const char *msg) {
    fprintf(stderr, "Fatal error: %s\n", msg);
    exit(1);
}
