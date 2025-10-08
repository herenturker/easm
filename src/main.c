/*
    EASM, Eren's Educational Assembler Project
    Copyright (C) 2025 Habil Eren Türker

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
