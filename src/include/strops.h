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

// String operations

#ifndef STROPS_H
#define STROPS_H

#include <ctype.h>

/**
 * @brief Converts a string to uppercase.
 *
 * This function modifies the input string in place by converting all
 * lowercase alphabetic characters to their uppercase equivalents.
 *
 * @param s Pointer to the string to be converted.
 */
void str_to_upper(char *s);

/**
 * @brief Converts a string to lowercase.
 *
 * This function modifies the input string in place by converting all
 * uppercase alphabetic characters to their lowercase equivalents.
 *
 * @param s Pointer to the string to be converted.
 */
void str_to_lower(char *s);

#endif // STROPS_H
