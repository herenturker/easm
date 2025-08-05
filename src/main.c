/*
    FILE: src/main.c
    AUTHOR: Habil Eren Turker
    ABOUT: Getting file contents from command line.

    Copyright (C) 2025  Habil Eren Turker

*/

// INCLUDE LIBRARIES HERE
#include <stdio.h>
#include <string.h>
#include "include\proggrlinfo.h"
#include "include\errors.h"
#include "include\lexer.h"

// DEFINITIONS HERE
#define MAX_LENGTH 256

int main(int argc, char *argv[])
{
    printf("%s Copyright (C) %d %s\n", progName, progYear, progAuthor);
    printf("This program comes with ABSOLUTELY NO WARRANTY;\nThis is free software, and you are welcome to redistribute it\nunder certain conditions.\n\n");

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror(ERROR_FILE_NOT_OPENED);
        return 1;
    }

    char line[MAX_LENGTH];
    int line_number = 1;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';

        char formatted_line[MAX_LENGTH + 20];
        snprintf(formatted_line, sizeof(formatted_line), "line%d~%s", line_number, line);

        // printf("%s\n", formatted_line);
        lexer_process_line(formatted_line, filename, &line_number);

        line_number++;
    }

    fclose(file);
    return 0;
}
