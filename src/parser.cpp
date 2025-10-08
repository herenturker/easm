/*
    EASM, Eren's Educational Assembler Project
    Copyright (C) 2025 Habil Eren Türker

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
 * @brief Processes a Token object by storing its type and lexeme, and handling line completion.
 * 
 * This function ignores tokens of type "COMMENT". For other tokens, it stores their
 * type and lexeme in vectors representing the current line. When an "EOL" (end of line)
 * token is encountered, it calls the handler for the collected tokens of the line,
 * unless the line is empty (only contains EOL). Afterwards, it resets the line vectors.
 * 
 * @param token The Token object to process.
 */
void parse_token_and_lexeme(const Token& token) {
    // Skip comments completely
    if (token.type == "COMMENT") {
        return;
    }

    // Store the token type and lexeme for the current line
    tokens_in_line.push_back(token.type);
    lexemes_in_line.push_back(token.lexeme);

    // On end-of-line token, handle the accumulated line tokens
    if (token.type == "EOL") {
        // Determine if the line contains only EOL (empty line)
        bool only_eol = (tokens_in_line.size() == 1 && tokens_in_line[0] == "EOL");

        if (!only_eol) {
            // Process tokens and lexemes for the current line
            handle_parse(tokens_in_line, lexemes_in_line);
        }

        // Clear vectors to prepare for the next line
        tokens_in_line.clear();
        lexemes_in_line.clear();
    }
}

/**
 * @brief Parses a single input line representing a token and extracts its type and lexeme.
 * 
 * The expected input format is:
 * 
 *     Token: <TOKEN_TYPE> Lexeme: <LEXEME>
 * 
 * This function reads the input line, extracts the token type and lexeme, removes any
 * trailing colons from keys, and constructs a Token object. It then forwards the Token
 * to the parser for processing.
 * 
 * @param token_line A C-string representing a single lexer output line.
 */
void parser_process_line(const char *token_line) {
    std::string line(token_line);
    std::istringstream iss(line);

    std::string key1, token_type, key2, lexeme;

    // Extract keys and values from the input line
    iss >> key1 >> token_type >> key2;

    // Extract the rest of the line as lexeme (may contain spaces)
    std::getline(iss, lexeme);
    if (!lexeme.empty() && lexeme[0] == ' ') {
        lexeme.erase(0, 1);
    }

    // Remove trailing colons from keys (e.g., "Token:" -> "Token")
    if (!key1.empty() && key1.back() == ':') {
        key1.pop_back();
    }
    if (!key2.empty() && key2.back() == ':') {
        key2.pop_back();
    }

    // Output token type and lexeme for debugging
    // std::cout << "Token type: " << token_type << ", Lexeme: " << lexeme << std::endl;

    // Construct Token and forward for parsing
    Token current;
    current.type = token_type;
    current.lexeme = lexeme;

    parse_token_and_lexeme(current);
}
