#ifndef LEXER_H
#define LEXER_H

#include "registers.h"
#include "instructions.h"

/**
 * @brief Represents different types of tokens that can be identified during lexical analysis.
 */
typedef enum {
    // === Core assembly elements ===
    TOKEN_INSTR,            /**< Assembly instruction (e.g., MOV, JMP) */
    TOKEN_REG,              /**< General-purpose register */
    TOKEN_REG8,             /**< 8-bit register (e.g., AL, BH) */
    TOKEN_REG16,            /**< 16-bit register (e.g., AX, SI) */
    TOKEN_REG32,            /**< 32-bit register (e.g., EAX, ESI) */
    TOKEN_SEGREG,           /**< Segment register (e.g., DS, ES) */

    TOKEN_NUMBER,           /**< Numeric literal (decimal, hex, etc.) */
    TOKEN_STRING,           /**< String literal (e.g., "Hello") */
    TOKEN_CHAR,             /**< Character literal (e.g., 'A') */

    TOKEN_LABEL,            /**< Label identifier (e.g., my_label:) */
    TOKEN_COMMENT,          /**< Comment (e.g., ; This is a comment) */
    TOKEN_DIRECTIVE,        /**< Assembler directive (e.g., db, org, equ) */

    // === Sections ===
    TOKEN_SECTION,          /**< 'section' keyword */
    TOKEN_SECTION_TYPE,     /**< Section type (e.g., .text, .data) */
    TOKEN_DATASEGMENT,      /**< Identifier in the data section (e.g., msg) */

    // === Symbols & punctuation ===
    TOKEN_COMMA,            /**< , */
    TOKEN_COLON,            /**< : */
    TOKEN_DOT,              /**< . */
    TOKEN_PLUS,             /**< + */
    TOKEN_MINUS,            /**< - */
    TOKEN_STAR,              /**< * */
    TOKEN_MODULO,           /**< % (modulo symbol) */
    TOKEN_SEMICOLON,        /**< ; */

    TOKEN_OPEN_PARENTHESIS,     /**< ( */
    TOKEN_CLOSE_PARENTHESIS,    /**< ) */
    TOKEN_OPEN_BRACKET,         /**< [ (used for memory references) */
    TOKEN_CLOSE_BRACKET,        /**< ] */

    TOKEN_DOLLAR_SIGN,      /**< $ (current address) */

    // === Utility ===
    TOKEN_EOF,              /**< End of file/input */
    TOKEN_ERROR             /**< Unknown or invalid token */
} TokenType;


/**
 * @brief Structure representing a token returned by the lexer.
 */
typedef struct
{
    TokenType type;         /**< Type of the token */
    InstructionType instr_type; /**< Type of the instruction */
    Register8 t_register8; /**< Type of the register (8 bit) */
    Register16 t_register16; /**< Type of the register (16 bit) */
    Register32 t_register32; /**< Type of the register (32 bit) */
    SegmentRegister t_segregister; /**< Type of the register (segment register) */
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

#endif // LEXER_H
