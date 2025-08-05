#ifndef LEXER_H
#define LEXER_H

#include "registers.h"

typedef enum {
    TOKEN_INSTR,
    TOKEN_REG,
    TOKEN_REG8,
    TOKEN_REG16,
    TOKEN_REG32,
    TOKEN_SEGREG,       
    TOKEN_NUMBER,
    TOKEN_COMMA,
    TOKEN_COLON,        
    TOKEN_LABEL,
    TOKEN_DIRECTIVE,
    TOKEN_COMMENT,
    TOKEN_CHAR,
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_DOT,
    TOKEN_DATASEGMENT,
    TOKEN_STRING,
    TOKEN_SECTION,
    TOKEN_SECTION_TYPE
} TokenType;


typedef struct
{
    TokenType type;
    int line;
    char lexeme[64];
} Token;

Token get_next_token(const char **input_ptr, int *line);
void lexer_process_line(const char *line, const char *file, int *line_number_ptr);
const char *token_type_to_string(TokenType type);
int is_inside_quotes(const char *line_start, const char *pos);
void set_filename(const char *filename);

TokenType get_register_token_type(const char *lexeme);
int is_register32(const char *lexeme);
int is_register16(const char *lexeme);
int is_register8(const char *lexeme);
int is_segment_register(const char *lexeme);

#endif
