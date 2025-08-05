// Informations about errors.

#ifndef ERRORS_H
#define ERRORS_H

void occur_error(const char* error_name, int* line_number, const char* file);
void fatal_error(const char *msg) __attribute__((noreturn));


static const char* const ERROR_FILE_NOT_OPENED = "Error: File cannot be opened.";
static const char* const ERROR_NO_CLOSING_QUOTE = "Error: Cannot find closing quote.";
static const char* const ERROR_NO_CHAR_WRITTEN_AND_END_OF_LINE = "Error: Char is not written and end of line.";
static const char* const ERROR_UNKNOWN_TOKEN = "Error: Cannot resolve symbol.";
#endif // ERRORS_H