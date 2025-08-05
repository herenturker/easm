#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/registers.h"
#include "include/errors.h"
#include "include/strops.h"

char g_filename[256] = "";

void set_filename(const char *filename)
{
    strncpy(g_filename, filename, sizeof(g_filename) - 1);
    g_filename[sizeof(g_filename) - 1] = '\0';
}

int is_segment_register(const char *lexeme)
{

    return (strcmp(lexeme, "CS") == 0) ||
           (strcmp(lexeme, "DS") == 0) ||
           (strcmp(lexeme, "SS") == 0) ||
           (strcmp(lexeme, "ES") == 0) ||
           (strcmp(lexeme, "FS") == 0) ||
           (strcmp(lexeme, "GS") == 0);
}

int is_inside_quotes(const char *line_start, const char *pos)
{
    int quote_count = 0;
    for (const char *c = line_start; c < pos; c++)
    {
        if (*c == '"')
        {
            quote_count++;
        }
    }
    return (quote_count % 2) != 0;
}

int is_register8(const char *lexeme)
{
    return get_register8_by_name(lexeme) != REG8_NONE;
}

int is_register16(const char *lexeme)
{
    return get_register16_by_name(lexeme) != REG16_NONE;
}

int is_register32(const char *lexeme)
{
    return get_register32_by_name(lexeme) != REG32_NONE;
}

TokenType get_register_token_type(const char *lexeme)
{
    if (is_register8(lexeme))
        return TOKEN_REG8;
    if (is_register16(lexeme))
        return TOKEN_REG16;
    if (is_register32(lexeme))
        return TOKEN_REG32;
    if (is_segment_register(lexeme))
        return TOKEN_SEGREG;
    return TOKEN_REG;
}

Token get_next_token(const char **input_ptr, int *line)
{
    Token token;
    token.line = *line;
    token.lexeme[0] = '\0';

    const char *p = *input_ptr;
    const char *line_start = *input_ptr;

    while (*p == ' ' || *p == '\t')
        p++;

    if (*p == '\0' || *p == '\n')
    {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "<EOF>");
        *input_ptr = p;
        return token;
    }

    if (*p == ',')
    {
        token.type = TOKEN_COMMA;
        strcpy(token.lexeme, ",");
        p++;
        *input_ptr = p;
        return token;
    }

    if (*p == '.')
    {
        token.type = TOKEN_DOT;
        strcpy(token.lexeme, ".");
        p++;
        *input_ptr = p;
        return token;
    }

    if (*p == ':')
    {
        token.type = TOKEN_COLON;
        strcpy(token.lexeme, ":");
        p++;
        *input_ptr = p;
        return token;
    }

    if (*p == ';')
    {
        if (!is_inside_quotes(line_start, p))
        {
            token.type = TOKEN_COMMENT;
            int i = 0;
            while (*p != '\0' && *p != '\n')
            {
                if (i < sizeof(token.lexeme) - 1)
                    token.lexeme[i++] = *p;
                p++;
            }
            token.lexeme[i] = '\0';
            *input_ptr = p;
            return token;
        }
    }

    if (*p == '\'')
    {
        if (!is_inside_quotes(line_start, p))
        {
            token.type = TOKEN_CHAR;
            int i = 0;

            token.lexeme[i++] = *p;
            p++;

            if (*p != '\0' && *p != '\n')
            {
                token.lexeme[i++] = *p;
                p++;
            }
            else
            {
                occur_error(ERROR_NO_CHAR_WRITTEN_AND_END_OF_LINE, line, g_filename);
                goto terminate;
            }

            if (*p == '\'')
            {
                token.lexeme[i++] = *p;
                p++;
            }
            else
            {
                occur_error(ERROR_NO_CLOSING_QUOTE, line, g_filename);
                goto terminate;
            }

            token.lexeme[i] = '\0';

            *input_ptr = p;
            return token;
        }
    }

    if ((*p >= '0' && *p <= '9') || (*p == '0' && (*(p + 1) == 'x' || *(p + 1) == 'X')))
    {
        int i = 0;
        while ((*p >= '0' && *p <= '9') ||
               (*p >= 'a' && *p <= 'f') ||
               (*p >= 'A' && *p <= 'F') ||
               *p == 'x' || *p == 'X')
        {
            if (i < (int)sizeof(token.lexeme) - 1)
                token.lexeme[i++] = *p;
            p++;
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;
        *input_ptr = p;
        return token;
    }

    if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || *p == '_')
    {
        int i = 0;
        while (((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') ||
                (*p >= '0' && *p <= '9') || *p == '_' || *p == '.'))
        {
            if (i < (int)sizeof(token.lexeme) - 1)
                token.lexeme[i++] = *p;
            p++;
        }
        token.lexeme[i] = '\0';

        if (*p == ':')
        {
            token.type = TOKEN_LABEL;
            strcpy(token.lexeme, token.lexeme);
            *input_ptr = p;
            return token;
        }

        char upper_lexeme[64];
        strncpy(upper_lexeme, token.lexeme, sizeof(upper_lexeme) - 1);
        upper_lexeme[sizeof(upper_lexeme) - 1] = '\0';
        str_to_upper(upper_lexeme);

        TokenType reg_type = get_register_token_type(upper_lexeme);
        if (reg_type != TOKEN_REG)
        {
            token.type = reg_type;
            strcpy(token.lexeme, upper_lexeme);
        }
        else
        {

            if (strcmp(upper_lexeme, "SECTION") == 0)
            {
                token.type = TOKEN_SECTION;
                strcpy(token.lexeme, upper_lexeme);
            }
            else if (strcmp(upper_lexeme, "DATA") == 0 || strcmp(upper_lexeme, "TEXT") == 0)
            {
                token.type = TOKEN_SECTION_TYPE;
                strcpy(token.lexeme, upper_lexeme);
            }
            else if (upper_lexeme[0] == '.')
            {
                token.type = TOKEN_DIRECTIVE;
                strcpy(token.lexeme, upper_lexeme);
            }
            else
            {
                token.type = TOKEN_INSTR;
                strcpy(token.lexeme, upper_lexeme);
            }
        }
        *input_ptr = p;
        return token;
    }

    token.type = TOKEN_ERROR;
    token.lexeme[0] = *p;
    token.lexeme[1] = '\0';
    occur_error(ERROR_UNKNOWN_TOKEN, line, g_filename);
    p++;
    *input_ptr = p;
    return token;

terminate:
    token.type = TOKEN_ERROR;
    strcpy(token.lexeme, "ERROR");
    *input_ptr = p;
    return token;
}

void lexer_process_line(const char *line, const char *file, int *line_number_ptr)
{
    const char *tilde = strchr(line, '~');
    int line_number = 0;

    if (tilde)
    {
        char line_number_str[16];
        int len = (int)(tilde - line);
        if (len > (int)sizeof(line_number_str) - 1)
            len = sizeof(line_number_str) - 1;
        strncpy(line_number_str, line, len);
        line_number_str[len] = '\0';
        line_number = atoi(line_number_str);

        line += (len + 1);
    }
    else if (line_number_ptr != NULL)
    {
        line_number = *line_number_ptr;
        (*line_number_ptr)++;
    }
    else
    {
        // default 0
        line_number = 0;
    }

    printf("Parsing line %d: %s\n", *line_number_ptr, line);
    set_filename(file);

    const char *code_ptr = line;

    while (1)
    {
        Token token = get_next_token(&code_ptr, &line_number);
        if (token.type == TOKEN_EOF)
            break;

        printf("Token: %-12s Lexeme: %s\n",
               token_type_to_string(token.type),
               token.lexeme);
    }
}

const char *token_type_to_string(TokenType type)
{
    switch (type)
    {
    case TOKEN_INSTR:
        return "INSTR";
    case TOKEN_REG:
        return "REG";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_COMMA:
        return "COMMA";
    case TOKEN_LABEL:
        return "LABEL";
    case TOKEN_DIRECTIVE:
        return "DIRECTIVE";
    case TOKEN_COMMENT:
        return "COMMENT";
    case TOKEN_CHAR:
        return "CHAR";
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_ERROR:
        return "ERROR";
    case TOKEN_DOT:
        return "DOT";
    case TOKEN_DATASEGMENT:
        return "DATASEGMENT";
    case TOKEN_REG8:
        return "REG8";
    case TOKEN_REG16:
        return "REG16";
    case TOKEN_REG32:
        return "REG32";
    case TOKEN_STRING:
        return "STRING";
    case TOKEN_SECTION:
        return "SECTION";
    case TOKEN_SECTION_TYPE:
        return "SECTION_TYPE";
    case TOKEN_COLON:
        return "COLON";
    case TOKEN_SEGREG:
        return "SEGREG";

    default:
        return "UNKNOWN";
    }
}
