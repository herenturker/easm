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

// strops.c

#include <ctype.h>
#include "include/strops.h"

/**
 * @brief Converts all characters in a string to uppercase.
 *
 * This function modifies the input string in-place, converting each character
 * to its uppercase equivalent using the C standard library function `toupper`.
 *
 * @param s Pointer to the null-terminated string to be converted.
 */
void str_to_upper(char *s) {
    for (; *s; s++) {
        *s = (char)toupper((unsigned char)*s);
    }
}

/**
 * @brief Converts all characters in a string to lowercase.
 *
 * This function modifies the input string in-place, converting each character
 * to its lowercase equivalent using the C standard library function `tolower`.
 *
 * @param s Pointer to the null-terminated string to be converted.
 */
void str_to_lower(char *s) {
    for (; *s; s++) {
        *s = (char)tolower((unsigned char)*s);
    }
}
