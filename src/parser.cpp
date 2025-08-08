#include "include/parser.h"
#include "include/parser_handler.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

std::vector<Token> tokens;
std::vector<std::string> tokens_in_line;
std::vector<std::string> lexemes_in_line;

/**
 * @brief Parses a Token type token
 * 
 * @param token 
 */
void parse_token_and_lexeme(const Token& token) {
    // Skip comments completely
    if (token.type == "COMMENT") {
        return;
    }

    // Always store the token (including EOL)
    tokens_in_line.push_back(token.type);
    lexemes_in_line.push_back(token.lexeme);

    // If EOL, handle the current line
    if (token.type == "EOL") {
        // Check if this EOL is the only token (empty line) 
        // or if the previous line was already empty
        bool only_eol = (tokens_in_line.size() == 1 && tokens_in_line[0] == "EOL");

        if (!only_eol) {
            handle_parse(tokens_in_line, lexemes_in_line);
        }

        // Reset for next line
        tokens_in_line.clear();
        lexemes_in_line.clear();
    }
}


/**
 * @brief Parses a single token line and extracts the token type and lexeme.
 * 
 * The expected input format is:
 * "Token: <TOKEN_TYPE> Lexeme: <LEXEME>"
 * 
 * This function splits the input line into parts, removes trailing colons (':')
 * from the keys ("Token:" and "Lexeme:"), and then outputs the token type and lexeme.
 * 
 * @param token_line C-style string representing one token line from the lexer.
 */
void parser_process_line(const char *token_line) {
    // Convert C-string to C++ string for easier manipulation
    std::string line(token_line);

    // Create an input string stream to parse the line by whitespace
    std::istringstream iss(line);

    // Variables to hold the parsed components
    std::string key1, token_type;
    std::string key2, lexeme;

    // Extract four whitespace-separated parts from the line
    iss >> key1 >> token_type >> key2 >> lexeme;

    // Remove trailing ':' from key1 if present (e.g., "Token:" -> "Token")
    if (!key1.empty() && key1.back() == ':') {
        key1.pop_back();
    }

    // Remove trailing ':' from key2 if present (e.g., "Lexeme:" -> "Lexeme")
    if (!key2.empty() && key2.back() == ':') {
        key2.pop_back();
    }

    // Output the parsed token type and lexeme
    // std::cout << "Token type: " << token_type << ", Lexeme: " << lexeme << std::endl;

    Token current; // Current line's token and lexeme

    current.type = token_type;
    current.lexeme = lexeme;

    parse_token_and_lexeme(current);

}
