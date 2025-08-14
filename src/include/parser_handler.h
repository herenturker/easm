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