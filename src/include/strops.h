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
