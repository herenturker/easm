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

#ifndef PARSER_HANDLER_H
#define PARSER_HANDLER_H

#ifdef __cplusplus
#include <vector>
#include <string>
#include <cstdint>
#include "opcode_table.h"

void handle_parse(std::vector<std::string> token_vector, std::vector<std::string> lexeme_vector);

std::string stringAndNumberToHex(const std::string &text, const std::string &numberStr);

std::string stringToHexInString(const std::string &text);

std::vector<uint8_t> hexStringToBytes(const std::string &hexStr);

bool is_label_token(const std::string &token, const std::string &lexeme);

std::string strip_label(const std::vector<std::string> &tokens, const std::vector<std::string> &lexemes, size_t idx);

OperandType get_operand_type_from_token(const std::string &token, const std::string &lexeme);

int stringToHexNumber(const std::string &input);

void handle_times(int count, const std::string &defineSize, const std::string &operandToken, const std::string &operandLexeme);

int evaluateExpr(const std::string &expr, int currentAddr, int baseAddr);

void handleTimesDirective(const std::vector<std::string> &token_vector,
                          const std::vector<std::string> &lexeme_vector,
                          int *lcPointer, int *blcPointer);

void handleInstructions(std::vector<std::string> token_vector, std::vector<std::string> lexeme_vector);

int parseExpression(const std::string &s, size_t &pos);

int parseNumber(const std::string &s, size_t &pos);

int parseFactor(const std::string &s, size_t &pos);

int parseTerm(const std::string &s, size_t &pos);

int parseExpression(const std::string &s, size_t &pos);

int evalWithParentheses(const std::string &expr);

uint32_t align_address(uint32_t current_address, uint32_t alignment);


#endif // __cplusplus
#endif // PARSER_HANDLER_H