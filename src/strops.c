#include <ctype.h>
#include "include/strops.h"

// FUNCTION DECLARATIONS
void str_to_upper(char *s) {
    for (; *s; s++) {
        *s = toupper((unsigned char)*s);
    }
}


void str_to_lower(char *s) {
    for (; *s; s++) {
        *s = tolower((unsigned char)*s);
    }
}