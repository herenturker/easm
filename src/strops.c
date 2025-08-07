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
