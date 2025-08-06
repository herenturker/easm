/*
    FILE: src/main.c
    AUTHOR: Habil Eren Turker
    ABOUT: Getting file contents from command line.

    Copyright (C) 2025  Habil Eren Turker
*/

// INCLUDE LIBRARIES HERE
#include <stdio.h>
#include <string.h>
#include "include/proggrlinfo.h"
#include "include/errors.h"
#include "include/lexer.h"

// DEFINITIONS HERE
#define MAX_LENGTH 256

/**
 * @brief Entry point of the assembler program.
 *
 * This function reads an input file line-by-line, formats each line,
 * and passes it to the lexer for tokenization.
 *
 * @param argc Argument count.
 * @param argv Argument vector. The second argument (argv[1]) should be the input file name.
 * @return int Returns 0 on success, non-zero on error.
 */
int main(int argc, char *argv[])
{
    // Print copyright and licensing information
    printf("%s Copyright (C) %d %s\n", progName, progYear, progAuthor);
    printf("This program comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and you are welcome to redistribute it\nunder certain conditions.\n\n");

    // Ensure filename is provided
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    // Handle file open failure
    if (file == NULL)
    {
        perror(ERROR_FILE_NOT_OPENED);
        return 1;
    }

    char line[MAX_LENGTH];
    int line_number = 1;

    // Process each line in the input file
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline character if present
        line[strcspn(line, "\n")] = '\0';

        lexer_process_line(line, filename, &line_number);
    }

    fclose(file);
    return 0;
}
