#ifndef LEXER_H
#define LEXER_H

#include "registers.h"

/**
 * @brief Represents different types of tokens that can be identified during lexical analysis.
 */
typedef enum {
    TOKEN_INSTR,        /**< Assembly instruction */
    TOKEN_REG,          /**< General register (not specifically typed) */
    TOKEN_REG8,         /**< 8-bit register */
    TOKEN_REG16,        /**< 16-bit register */
    TOKEN_REG32,        /**< 32-bit register */
    TOKEN_SEGREG,       /**< Segment register */
    TOKEN_NUMBER,       /**< Numeric literal */
    TOKEN_COMMA,        /**< Comma symbol (',') */
    TOKEN_COLON,        /**< Colon symbol (':') */
    TOKEN_LABEL,        /**< Label identifier */
    TOKEN_DIRECTIVE,    /**< Assembler directive */
    TOKEN_COMMENT,      /**< Comment line or part */
    TOKEN_CHAR,         /**< Character literal */
    TOKEN_EOF,          /**< End of file or input */
    TOKEN_ERROR,        /**< Invalid or unknown token */
    TOKEN_DOT,          /**< Dot symbol ('.') */
    TOKEN_DATASEGMENT,  /**< Data segment indicator */
    TOKEN_STRING,       /**< String literal */
    TOKEN_SECTION,      /**< Section keyword */
    TOKEN_SECTION_TYPE  /**< Type of section (e.g., DATA, TEXT) */
} TokenType;

/**
 * @brief Structure representing a token returned by the lexer.
 */
typedef struct
{
    TokenType type;         /**< Type of the token */
    int line;               /**< Line number where the token was found */
    char lexeme[64];        /**< The actual lexeme (string representation) */
} Token;

/**
 * @brief Retrieves the next token from the input stream.
 * 
 * @param input_ptr Pointer to the input character stream.
 * @param line Pointer to the current line number.
 * @return Token The next identified token.
 */
Token get_next_token(const char **input_ptr, int *line);

/**
 * @brief Processes a line of code using the lexer and prints tokens.
 * 
 * @param line The line of code to process.
 * @param file The name of the file being parsed.
 * @param line_number_ptr Pointer to the current line number.
 */
void lexer_process_line(const char *line, const char *file, int *line_number_ptr);

/**
 * @brief Converts a TokenType to its corresponding string representation.
 * 
 * @param type The token type.
 * @return const char* String representation of the token type.
 */
const char *token_type_to_string(TokenType type);

/**
 * @brief Checks if a given position is inside quotation marks.
 * 
 * @param line_start Pointer to the start of the line.
 * @param pos Pointer to the character position to check.
 * @return int 1 if inside quotes, 0 otherwise.
 */
int is_inside_quotes(const char *line_start, const char *pos);

/**
 * @brief Sets the current filename for context in error reporting.
 * 
 * @param filename The filename to set.
 */
void set_filename(const char *filename);

/**
 * @brief Determines the token type based on a register's name.
 * 
 * @param lexeme The string representing the register.
 * @return TokenType The corresponding token type.
 */
TokenType get_register_token_type(const char *lexeme);

/**
 * @brief Checks whether a string is a 32-bit register.
 * 
 * @param lexeme The register name.
 * @return int 1 if it's a 32-bit register, 0 otherwise.
 */
int is_register32(const char *lexeme);

/**
 * @brief Checks whether a string is a 16-bit register.
 * 
 * @param lexeme The register name.
 * @return int 1 if it's a 16-bit register, 0 otherwise.
 */
int is_register16(const char *lexeme);

/**
 * @brief Checks whether a string is an 8-bit register.
 * 
 * @param lexeme The register name.
 * @return int 1 if it's an 8-bit register, 0 otherwise.
 */
int is_register8(const char *lexeme);

/**
 * @brief Checks whether a string is a segment register.
 * 
 * @param lexeme The register name.
 * @return int 1 if it's a segment register, 0 otherwise.
 */
int is_segment_register(const char *lexeme);

#endif
