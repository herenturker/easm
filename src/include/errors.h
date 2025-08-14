// Informations about errors.

#ifndef ERRORS_H
#define ERRORS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Handles a non-fatal error occurrence.
 * 
 * Prints the provided error message along with the file name and line number.
 * 
 * @param error_name The descriptive error message.
 * @param line_number Pointer to the current line number.
 * @param file The filename where the error occurred.
 */
void occur_error(const char* error_name, int* line_number, const char* file);

/**
 * @brief Reports a fatal error and terminates the program.
 * 
 * This function does not return. It prints the given error message and exits the application.
 * 
 * @param msg The fatal error message.
 */
void fatal_error(const char *msg) __attribute__((noreturn));

/**
 * @brief Error message indicating failure to open a file.
 */
static const char* const ERROR_FILE_NOT_OPENED = "Error: File cannot be opened.";

/**
 * @brief Error message for a missing closing quote.
 */
static const char* const ERROR_NO_CLOSING_QUOTE = "Error: Cannot find closing quote.";

/**
 * @brief Error message for a character constant that ends prematurely.
 */
static const char* const ERROR_NO_CHAR_WRITTEN_AND_END_OF_LINE = "Error: Char is not written and end of line.";

/**
 * @brief Error message for an unrecognized or invalid token.
 */
static const char* const ERROR_UNKNOWN_TOKEN = "Error: Cannot resolve symbol.";

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ERRORS_H
