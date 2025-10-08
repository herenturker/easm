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
