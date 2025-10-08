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

#include "include/parser_handler.h"
#include "include/opcode_table.h"
#include "include/errors.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <functional>
#include <cstdint>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

//                  label name  address
std::unordered_map<std::string, int> label_table;
//                   symbol name     value
std::unordered_map<std::string, std::string> symbol_table;

extern const std::unordered_map<std::string, int> no_operand_instructions;
extern const std::unordered_map<std::string, int> one_operand_instructions;
extern const std::unordered_map<std::string, int> two_operand_instructions;

int current_bits_mode = 16;    // EASM only supports 16 bit real mode, so this line is a guarantee
int location_counter = 0;      // $$
int base_location_counter = 0; // for saving ORG directive and $
int *lcPointer = &location_counter;
int *blcPointer = &base_location_counter;

static std::string toUpperStr(const std::string &s)
{
    std::string out;
    out.reserve(s.size());
    for (char c : s)
        out.push_back((char)std::toupper((unsigned char)c));
    return out;
}

inline int stringToInt(std::string str)
{
    return stoi(str);
}

int incByte(const std::string &defineSize)
{
    static const std::unordered_map<std::string, int> sizeMap{
        {"DB", 1},
        {"DW", 2},
        {"DD", 4}};

    auto it = sizeMap.find(defineSize);
    if (it != sizeMap.end())
    {
        return it->second;
    }
    return 0; // for unsupported ones (code crash maybe)
}

int countBytesInHexString(const std::string &hexStr)
{
    if (hexStr.empty())
        return 0;
    std::istringstream iss(hexStr);
    int count = 0;
    std::string byteStr;
    while (iss >> byteStr)
    {
        count++;
    }
    return count;
}

/**
 * @brief Main parsing handler for processing tokenized assembly input.
 *
 * This function supports label detection and storage, directive handling,
 * and instruction parsing including operand type resolution and opcode lookup.
 *
 * @param token_vector Vector of token strings representing types (e.g., "INSTR_MOV", "REG16_AX").
 * @param lexeme_vector Vector of lexeme strings representing the original text (e.g., "MOV", "AX").
 */
void handle_parse(std::vector<std::string> token_vector, std::vector<std::string> lexeme_vector)
{
    /* FOR DEBUGGING
    for(const auto& t : token_vector){
        std::cout << "Token: \n" << t << std::endl;
    }

    for(const auto& l : lexeme_vector){
        std::cout << "Lexeme: \n" << l << std::endl;
    }
    */

    if (token_vector[0].find("DIRECTIVE_") == 0) // handle directives first
    {
        if (token_vector[0] == "DIRECTIVE_BITS")
        {
            current_bits_mode = stringToInt(lexeme_vector[1]); // BITS 16
        }
        else if (token_vector[0] == "DIRECTIVE_ORG")
        {
            *lcPointer = stringToHexNumber(lexeme_vector[1]);
            *blcPointer = *lcPointer;
        }
        else if (token_vector[0] == "DIRECTIVE_DB" || token_vector[0] == "DIRECTIVE_DW" || token_vector[0] == "DIRECTIVE_DD") // handle if define x directives come first
        {
            int byteSize = incByte(token_vector[0].substr(10)); // substr(10) = DIRECTIVE_DB -> DB
            if (token_vector.size() > 1)                        // check for syntax error
            {
                if (token_vector[1] == "STRING")
                {
                    std::string hexString = stringToHexInString(lexeme_vector[1]);
                    int bytesCount = countBytesInHexString(hexString); // "Hello" becomes "H" "E" "L" "L" "O" and each character has one byte (does not count null terminator \0)
                    *lcPointer += bytesCount * byteSize;               // if db <string> then for each char, char * byte. this applies for dw and dd too.
                }
                else if (token_vector[1] == "NUMBER") // for example, dw 0xAA55
                {
                    *lcPointer += byteSize;
                }
                else // how can we come here
                {
                    fatal_error("UNKNOWN ERROR HAPPENED");
                }
            }
        }
        else if (token_vector[0] == "DIRECTIVE_EQU")
        {
            fatal_error("DIRECTIVE EQU CANNOT BE USED WITHOUT VARIABLE NAME"); // "MAXLEN equ 64" is OK.  "equ 64" is wrong
        }
        else if (token_vector[0] == "DIRECTIVE_ALIGN")
        {
            int alignVal = stringToInt(lexeme_vector[1]);
            align_address(*lcPointer, alignVal);
        }
        else if (token_vector[0] == "DIRECTIVE_TIMES")
        {

            int repeatCount = 0;
            int dataValue = 0;
            //                                                        full expression
            //                                             code start@----------------|
            // collect full expression after times (up to db) // times 510 - ($ - $$) db 0 // bootloader example
            std::string expr;
            size_t i = 1;
            for (; i < lexeme_vector.size(); ++i)
            {
                if (token_vector[i] == "DIRECTIVE_DB" ||
                    token_vector[i] == "DIRECTIVE_DW" ||
                    token_vector[i] == "DIRECTIVE_DD")
                    break;
                expr += lexeme_vector[i];
            }
            //                                                            dw and dd should be supported too
            if (i < token_vector.size() && token_vector[i] == "DIRECTIVE_DB" && i + 1 < lexeme_vector.size())
                dataValue = stringToInt(lexeme_vector[i + 1]);

            auto replacedExpr = expr;
            size_t pos;

            while ((pos = replacedExpr.find("$$")) != std::string::npos)
                replacedExpr.replace(pos, 2, std::to_string(*blcPointer));

            while ((pos = replacedExpr.find("$")) != std::string::npos)
                replacedExpr.replace(pos, 1, std::to_string(*lcPointer));

            try
            {
                repeatCount = evalWithParentheses(replacedExpr); // for (3 + 2) * 5 like statements
            }
            catch (const std::exception &ex)
            {
                std::cerr << "Error evaluating expression: " << ex.what() << std::endl;
                repeatCount = 0; // some sensible default/error value
            }

            if (token_vector.size() > i + 1)
            {
                std::string defineSize = token_vector[i].substr(9);
                std::string operandToken = token_vector[i + 1];
                std::string operandLexeme = lexeme_vector[i + 1];
                handle_times(repeatCount, defineSize, operandToken, operandLexeme);
            }
        }
    }
    else if (token_vector[0].find("INSTR_") == 0)
    {
        if (token_vector[0] == "INSTR_GENERIC")
        {
            if (likely(token_vector.size() > 1 && token_vector[1].find("DIRECTIVE_") == 0))
            {
                if (token_vector[1] == "DIRECTIVE_EQU")
                {
                    // Handle EQU directive: symbol = value (e.g. symbol EQU value)
                    if (lexeme_vector.size() > 2)
                        symbol_table[lexeme_vector[0]] = lexeme_vector[2];
                    else
                        fatal_error("EQU directive missing value");
                }
                else
                {
                    const std::string &directive = token_vector[1];
                    int byteSize = incByte(directive.substr(10)); // Remove "DIRECTIVE_" prefix

                    // msg db "Hello, EASM!", 0
                    if (token_vector.size() > 4 && token_vector[2] == "STRING" && token_vector[3] == "COMMA" && token_vector[4] == "NUMBER")
                    {
                        // Example: symbol DB "abc", 3
                        std::string hexString = stringAndNumberToHex(lexeme_vector[2], lexeme_vector[4]);
                        symbol_table[lexeme_vector[0]] = hexString;
                        int bytes = countBytesInHexString(hexString);
                        *lcPointer += bytes * byteSize;
                    } // msg db "Hello, EASM!""
                    else if (token_vector.size() > 2 && token_vector[2] == "STRING" && token_vector[3] == "EOL")
                    {
                        // Example: symbol DB "abc"
                        std::string hexString = stringToHexInString(lexeme_vector[2]);
                        symbol_table[lexeme_vector[0]] = hexString;
                        int bytes = countBytesInHexString(hexString);
                        *lcPointer += bytes * byteSize;
                    } // msg db 0
                    else if (token_vector.size() > 2 && token_vector[2] == "NUMBER")
                    {
                        // Example: symbol DB 123
                        symbol_table[lexeme_vector[0]] = lexeme_vector[2];
                        *lcPointer += byteSize;
                    }
                    else
                    {
                        fatal_error("Unsupported operand format in directive");
                    }
                }
            }
            else
            {
                fatal_error("Expected a directive token");
            }
        }
        else
        {
            handleInstructions(token_vector, lexeme_vector);
        }
    }
    else if (token_vector[0] == "EOL") // empty line in .asm code
    {
        return;
    }
    else if (token_vector[0] == "DOT")
    {
        if (token_vector[1] == "LABEL") // local labels like .loop:
        {
            std::string strLabel = "." + lexeme_vector[1];
            label_table[strLabel] = location_counter;
        }
        else if (token_vector[1].find("DIRECTIVE_") == 0) // EASM does not support DOT DIRECTIVES (.equ)
        {
            fatal_error("EASM DOES NOT SUPPORT .DIRECTIVE STRUCTURE");
        }
    }
    else if (token_vector[0] == "LABEL")
    {
        std::string label = lexeme_vector[0];
        if (!label.empty() && label.back() == ':')
        {
            label.pop_back();
        }
        label_table[label] = location_counter;
    }
    else
    {
        fatal_error("UNKNOWN ERROR HAPPENED");
    }
}

std::string stringAndNumberToHex(const std::string &text, const std::string &numberStr)
{
    std::ostringstream oss;

    // Convert text to hex
    for (auto c : text)
    {
        oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(c) << " ";
    }

    // Convert numberStr to integer (supports decimal or hex string)
    int number = 0;
    if (numberStr.size() > 2 && (numberStr[0] == '0') && (numberStr[1] == 'x' || numberStr[1] == 'X'))
    {
        number = std::stoi(numberStr, nullptr, 16); // hex string like "0x1F"
    }
    else
    {
        number = std::stoi(numberStr, nullptr, 10); // decimal string like "32"
    }

    // Append number in hex form
    oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
        << (number & 0xFF);

    return oss.str();
}

std::string stringToHexInString(const std::string &text)
{
    std::ostringstream oss;
    for (char c : text)
    {
        unsigned char uc = static_cast<unsigned char>(c);
        oss << std::uppercase << std::hex
            << std::setw(2) << std::setfill('0')
            << static_cast<int>(uc) << " ";
    }
    std::string result = oss.str();
    if (!result.empty() && result.back() == ' ')
        result.pop_back();
    return result;
}

std::vector<uint8_t> hexStringToBytes(const std::string &hexStr)
{
    std::vector<uint8_t> bytes;
    std::istringstream iss(hexStr);
    std::string byteStr;

    while (iss >> byteStr)
    { // get space-separated hex bytes
        uint8_t byteVal = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
        bytes.push_back(byteVal);
    }
    return bytes;
}
// std::string hexText = stringAndNumberToHex("Hello", 0); // "48 65 6C 6C 6F 00"
// std::vector<uint8_t> realBytes = hexStringToBytes(hexText);

/**
 * @brief Checks whether a token represents a label.
 *
 * A label token is identified by having a trailing colon ':' character
 * or by token type "LABEL".
 *
 * @param token The token string to check.
 * @param lexeme The original lexeme string to check for trailing colon.
 * @return true if the token is a label; otherwise, false.
 */
bool is_label_token(const std::string &token, const std::string &lexeme)
{
    if (token == "LABEL")
        return true;
    if (!lexeme.empty() && lexeme.back() == ':')
        return true;
    return false;
}

/**
 * @brief Removes the trailing colon from a label token, handling both normal and local (.label) formats.
 *
 * For normal labels: LABEL:
 * For local labels: DOT LABEL COLON
 *
 * @param tokens   The token list.
 * @param lexemes  The lexeme list (same indexing as tokens).
 * @param idx      The current index in the token list pointing to the label start.
 * @return The combined label string without the trailing colon.
 */
std::string strip_label(const std::vector<std::string> &tokens,
                        const std::vector<std::string> &lexemes,
                        size_t idx)
{

    // Handle local labels like ".loop:"
    if (tokens[idx] == "DOT" && idx + 1 < tokens.size() && tokens[idx + 1] == "LABEL")
    {
        std::string combined = "." + lexemes[idx + 1];
        return combined; // No colon in lexeme since colon is separate token
    }

    // Handle normal labels like "start:"
    if (tokens[idx] == "LABEL")
    {
        std::string label = lexemes[idx];
        if (!label.empty() && label.back() == ':')
            return label.substr(0, label.size() - 1);
        return label;
    }

    return lexemes[idx];
}

/**
 * @brief Determines the operand type from a token string.
 *
 * This function maps detailed register tokens (e.g., REG16_AX) and
 * immediate or memory tokens to a generalized OperandType.
 *
 * @param token The token string representing the operand.
 * @return The corresponding OperandType. Returns OperandType::NONE if unrecognized.
 */
OperandType get_operand_type_from_token(const std::string &token, const std::string &lexeme)
{
    if (token.find("REG8_") == 0)
        return OperandType::REG8;
    if (token == "REG8")
        return OperandType::REG8;

    if (token.find("REG16_") == 0)
        return OperandType::REG16;
    if (token == "REG16")
        return OperandType::REG16;

    // if (token.find("REG32_") == 0) return OperandType::REG32;
    // if (token == "REG32") return OperandType::REG32;

    if (token.find("SEGREG_") == 0)
        return OperandType::SEGREG;
    if (token == "SEGREG")
        return OperandType::SEGREG;

    // if (token == "IMM8") return OperandType::IMM8;
    // if (token == "IMM16") return OperandType::IMM16;
    // if (token == "IMM32") return OperandType::IMM32;

    if (token == "MEM8")
        return OperandType::MEM8;
    if (token == "MEM16")
        return OperandType::MEM16;
    // if (token == "MEM32") return OperandType::MEM32;

    if (token == "STRING")
    {
        return OperandType::STRING;
    }

    if (token == "NUMBER")
    {
        // Parse the lexeme into an integer
        long value = std::stol(lexeme, nullptr, 0); // auto-detects base (0x.., decimal, etc.)

        // Check range for signed or unsigned 8-bit
        if (value >= -128 && value <= 255)
        {
            return OperandType::IMM8;
        }
        else
        {
            return OperandType::IMM16;
        }
    }
    return OperandType::NONE;
}

int stringToHexNumber(const std::string &input)
{
    return std::stoi(input, nullptr, 0);
}

//  expression evaluator
int evaluateExpr(const std::string &expr, int currentAddr, int baseAddr)
{
    // Replace $ and $$ with currentAddr and baseAddr respectively
    std::string replaced;
    replaced.reserve(expr.size());
    for (size_t i = 0; i < expr.size();)
    {
        if (i + 1 < expr.size() && expr.compare(i, 2, "$$") == 0)
        {
            replaced += std::to_string(baseAddr);
            i += 2;
        }
        else if (expr[i] == '$')
        {
            replaced += std::to_string(currentAddr);
            i += 1;
        }
        else
        {
            replaced += expr[i++];
        }
    }

    size_t pos = 0;
    auto skipSpaces = [&]()
    {
        while (pos < replaced.size() && isspace((unsigned char)replaced[pos]))
            ++pos;
    };

    std::function<int()> parseExpr, parseXor, parseAnd, parseShift, parseTerm, parseFactor;

    // parse number: optional sign, decimal or hex (0x)
    auto parseNumber = [&]() -> int
    {
        skipSpaces();
        bool neg = false;
        if (pos < replaced.size() && (replaced[pos] == '+' || replaced[pos] == '-'))
        {
            if (replaced[pos] == '-')
                neg = true;
            ++pos;
        }
        skipSpaces();
        if (pos >= replaced.size() || (!isdigit((unsigned char)replaced[pos])))
            throw std::runtime_error("Expected number at pos " + std::to_string(pos));

        size_t start = pos;
        if (replaced[pos] == '0' && pos + 1 < replaced.size() &&
            (replaced[pos + 1] == 'x' || replaced[pos + 1] == 'X'))
        {
            pos += 2;
            size_t hexStart = pos;
            while (pos < replaced.size() && isxdigit((unsigned char)replaced[pos]))
                ++pos;
            if (hexStart == pos)
                throw std::runtime_error("Invalid hex literal");
            std::string hexStr = replaced.substr(start, pos - start);
            int val = std::stoi(hexStr, nullptr, 0);
            return neg ? -val : val;
        }
        else
        {
            while (pos < replaced.size() && isdigit((unsigned char)replaced[pos]))
                ++pos;
            std::string numStr = replaced.substr(start, pos - start);
            int val = std::stoi(numStr, nullptr, 0);
            return neg ? -val : val;
        }
    };

    // Factor: handle unary +, -, ~ and parentheses
    parseFactor = [&]() -> int
    {
        skipSpaces();
        if (pos < replaced.size())
        {
            if (replaced[pos] == '+')
            {
                ++pos;
                return parseFactor();
            }
            if (replaced[pos] == '-')
            {
                ++pos;
                return -parseFactor();
            }
            if (replaced[pos] == '~')
            {
                ++pos;
                return ~parseFactor();
            }
            if (replaced[pos] == '(')
            {
                ++pos;
                int val = parseExpr();
                skipSpaces();
                if (pos >= replaced.size() || replaced[pos] != ')')
                    throw std::runtime_error("Missing )");
                ++pos;
                return val;
            }
        }
        return parseNumber();
    };

    // Term: *, /
    parseTerm = [&]() -> int
    {
        int val = parseFactor();
        while (true)
        {
            skipSpaces();
            if (pos >= replaced.size())
                break;
            if (replaced[pos] == '*')
            {
                ++pos;
                val *= parseFactor();
            }
            else if (replaced[pos] == '/')
            {
                ++pos;
                int rhs = parseFactor();
                if (rhs == 0)
                    throw std::runtime_error("Division by zero in expression");
                val /= rhs;
            }
            else
                break;
        }
        return val;
    };

    // Shift: <<, >>
    parseShift = [&]() -> int
    {
        int val = parseTerm();
        while (true)
        {
            skipSpaces();
            if (pos + 1 >= replaced.size())
                break;
            if (replaced[pos] == '<' && replaced[pos + 1] == '<')
            {
                pos += 2;
                val <<= parseTerm();
            }
            else if (replaced[pos] == '>' && replaced[pos + 1] == '>')
            {
                pos += 2;
                val >>= parseTerm();
            }
            else
                break;
        }
        return val;
    };

    // AND: &
    parseAnd = [&]() -> int
    {
        int val = parseShift();
        while (true)
        {
            skipSpaces();
            if (pos >= replaced.size() || replaced[pos] != '&')
                break;
            ++pos;
            val &= parseShift();
        }
        return val;
    };

    // XOR: ^
    parseXor = [&]() -> int
    {
        int val = parseAnd();
        while (true)
        {
            skipSpaces();
            if (pos >= replaced.size() || replaced[pos] != '^')
                break;
            ++pos;
            val ^= parseAnd();
        }
        return val;
    };

    // Expr: +, -, | (lowest precedence)
    parseExpr = [&]() -> int
    {
        int val = parseXor();
        while (true)
        {
            skipSpaces();
            if (pos >= replaced.size())
                break;
            if (replaced[pos] == '+')
            {
                ++pos;
                val += parseXor();
            }
            else if (replaced[pos] == '-')
            {
                ++pos;
                val -= parseXor();
            }
            else if (replaced[pos] == '|')
            {
                ++pos;
                val |= parseXor();
            }
            else
                break;
        }
        return val;
    };

    int result = parseExpr();
    skipSpaces();
    if (pos != replaced.size())
        throw std::runtime_error("Invalid expression (unexpected chars at end)");

    return result;
}

void handleTimesDirective(const std::vector<std::string> &token_vector,
                          const std::vector<std::string> &lexeme_vector,
                          int *p_lcPointer, int *p_blcPointer) // they start with p_ (parameter) so the compiler would not show warning about these.
{
    // 1) Find the index of the size-directive token (db/dw/dd) in lexeme_vector (case-insensitive)
    ssize_t sizeIdx = -1;
    for (size_t i = 1; i < lexeme_vector.size(); ++i)
    {
        std::string up = toUpperStr(lexeme_vector[i]);
        if (up == "DIRECTIVE_DB" || up == "DIRECTIVE_DW" || up == "DIRECTIVE_DD")
        {
            sizeIdx = (ssize_t)i;
            break;
        }
    }

    // 2) Build expr from lexemes between index 1 and sizeIdx (exclusive). If sizeIdx==-1, take rest.
    std::string expr;
    size_t exprEnd = (sizeIdx == -1) ? lexeme_vector.size() : (size_t)sizeIdx;
    for (size_t i = 1; i < exprEnd; ++i)
    {
        expr += lexeme_vector[i];
    }
    if (expr.empty())
    {
        throw std::runtime_error("times: missing expression");
    }

    // 3) Evaluate expression using evaluator
    int repeatCount = 0;
    try
    {
        repeatCount = evaluateExpr(expr, *p_lcPointer, *p_blcPointer);
    }
    catch (const std::exception &ex)
    {
        std::ostringstream oss;
        oss << "times: expression evaluation failed: " << ex.what();
        throw std::runtime_error(oss.str());
    }

    // 4) If we found size/operand, prepare defineSize and operandToken/lexeme
    if (sizeIdx != -1)
    {
        std::string defineSize = toUpperStr(lexeme_vector[sizeIdx]); // DB/DW/DD
        std::string operandToken;
        std::string operandLexeme;
        if (sizeIdx + 1 < lexeme_vector.size())
        {
            // try to find corresponding token at same index (best-effort)
            if (sizeIdx + 1 < token_vector.size())
            {
                operandToken = token_vector[sizeIdx + 1];
            }
            operandLexeme = lexeme_vector[sizeIdx + 1];
        }
        // call existing handle_times
        handle_times(repeatCount, defineSize, operandToken, operandLexeme);
    }
    else
    {
        // no size/operand, just call with empty operand -> delegate behavior to handle_times
        handle_times(repeatCount, "", "", "");
    }
}

void handle_times(int count, const std::string &defineSize, const std::string &operandToken, const std::string &operandLexeme)
{
    int byteSize = incByte(defineSize);
    if (byteSize == 0)
    {
        fatal_error("Unsupported size in times directive");
    }

    for (int i = 0; i < count; ++i)
    {
        if (operandToken == "NUMBER")
        {
            *lcPointer += byteSize;
        }
        else if (operandToken == "STRING")
        {
            std::string hexString = stringToHexInString(operandLexeme);
            int bytesCount = countBytesInHexString(hexString);
            *lcPointer += bytesCount * byteSize;
        }
        else
        {
            fatal_error("Unsupported operand in times directive");
        }
    }
}

void handleInstructions(std::vector<std::string> token_vector,
                        std::vector<std::string> lexeme_vector)
{
    init_opcode_table();
    const std::string mnemonic = toUpperStr(lexeme_vector[0]);

    size_t idx = 1;
    ParsedOperand op1{OperandType::NONE, "", 0, 0, 0, 0};
    ParsedOperand op2{OperandType::NONE, "", 0, 0, 0, 0};

    // Parse operands (if any)
    if (idx < token_vector.size() && token_vector[idx] != "EOL")
    {
        op1 = parseOperand(token_vector, lexeme_vector, idx);
    }
    if (idx < token_vector.size() && token_vector[idx] == "COMMA")
    {
        idx++; // skip comma
        if (idx < token_vector.size() && token_vector[idx] != "EOL")
        {
            op2 = parseOperand(token_vector, lexeme_vector, idx);
        }
        else
        {
            fatal_error("Expected second operand after comma");
        }
    }

    bool skip_opcode_lookup = false;
        if (op1.type == OperandType::CHAR || op1.type == OperandType::STRING ||
            op2.type == OperandType::CHAR || op2.type == OperandType::STRING)
        {
            skip_opcode_lookup = true;
        }

    OpcodeInfo* info = nullptr;

    if (!skip_opcode_lookup)
    {
        // Lookup opcode by (mnemonic, op1.type, op2.type)
        OperandKey key{mnemonic, op1.type, op2.type};
        auto it = opcode_map.find(key);
        if (it == opcode_map.end())
            fatal_error("Opcode not found for given operands");
        info = &it->second;
    }

    auto u8 = [](int v) -> uint8_t
    { return static_cast<uint8_t>(v & 0xFF); };
    auto u16 = [](unsigned v) -> uint16_t
    { return static_cast<uint16_t>(v & 0xFFFF); };

    // Helpers
    auto build_mod = [](const ParsedOperand &m) -> uint8_t
    {
        // Special case: in 8086, mod=00 & r/m=110 means [disp16] direct.
        // If user encoded [BP] with no displacement (r/m=110), force disp8=0 (mod=01) to mean [BP + 0].
        if (m.displacement == 0)
        {
            if (m.modrm_rm == 0b110)
            {                // [BP] cannot be encoded with mod=00
                return 0b01; // force 8-bit disp of 0
            }
            return 0b00;
        }
        const int d = m.displacement;
        return (d >= -128 && d <= 127) ? 0b01 : 0b10; // 8-bit or 16-bit disp
    };
    auto modrm_byte = [](uint8_t mod, uint8_t reg, uint8_t rm) -> uint8_t
    {
        return static_cast<uint8_t>((mod << 6) | ((reg & 7) << 3) | (rm & 7));
    };
    auto print_disp = [&](const ParsedOperand &m, uint8_t mod)
    {
        if (mod == 0b01)
        {
            std::cout << "Disp8:  0x" << std::hex << (int)u8(m.displacement) << "\n";
        }
        else if (mod == 0b10)
        {
            std::cout << "Disp16: 0x" << std::hex << u16(static_cast<unsigned>(m.displacement)) << "\n";
        }
        else if (mod == 0b00 && m.modrm_rm == 0b110)
        {
            // Direct [disp16] addressing (mod=00, r/m=110)
            std::cout << "Disp16 (direct): 0x" << std::hex
                      << u16(static_cast<unsigned>(m.displacement)) << "\n";
        }
    };

    // 1) Primary opcode
    if (!skip_opcode_lookup){
        std::cout << "Opcode: 0x" << std::hex << (int)info->primary_opcode << "\n";
        (*lcPointer)++;
    }
    

    // 2) ModR/M (if needed) + displacement (if any)
    if (info && info->requires_modrm)
    {
        uint8_t mod = 0, reg = 0, rm = 0, mrm = 0;

        // ---- 16-bit width ----
        if (op1.type == OperandType::REG16 && op2.type == OperandType::REG16)
        {
            // r16 <- r16 OR r/m16, r16 depending on opcode; your table determines direction.
            mod = 0b11;
            reg = op2.reg_code; // source in Reg
            rm = op1.reg_code;  // dest  in R/M
            mrm = modrm_byte(mod, reg, rm);
        }
        else if (op1.type == OperandType::REG16 && op2.type == OperandType::MEM16)
        {
            // r16 <- m16
            mod = build_mod(op2);
            reg = op1.reg_code;
            rm = op2.modrm_rm;
            mrm = modrm_byte(mod, reg, rm);
        }
        else if (op1.type == OperandType::MEM16 && op2.type == OperandType::REG16)
        {
            // m16 <- r16
            mod = build_mod(op1);
            reg = op2.reg_code;
            rm = op1.modrm_rm;
            mrm = modrm_byte(mod, reg, rm);
        }
        else if ((op1.type == OperandType::REG16 || op1.type == OperandType::MEM16) &&
                 op2.type == OperandType::IMM16)
        {
            // r/m16 <- imm16 ; Group opcode uses opcode_ext in Reg field
            const uint8_t ext = info->opcode_ext; // set in table (e.g., ADD = 0)
            if (op1.type == OperandType::REG16)
            {
                mod = 0b11;
                reg = ext;
                rm = op1.reg_code;
            }
            else
            {
                mod = build_mod(op1);
                reg = ext;
                rm = op1.modrm_rm;
            }
            mrm = modrm_byte(mod, reg, rm);
        }
        // Some instructions use imm8 to r/m16 (sign-extended), table must have imm_size=1
        else if ((op1.type == OperandType::REG16 || op1.type == OperandType::MEM16) &&
                 op2.type == OperandType::IMM8)
        {
            const uint8_t ext = info->opcode_ext;
            if (op1.type == OperandType::REG16)
            {
                mod = 0b11;
                reg = ext;
                rm = op1.reg_code;
            }
            else
            {
                mod = build_mod(op1);
                reg = ext;
                rm = op1.modrm_rm;
            }
            mrm = modrm_byte(mod, reg, rm);
        }

        // ---- 8-bit width ----
        else if (op1.type == OperandType::REG8 && op2.type == OperandType::REG8)
        {
            mod = 0b11;
            reg = op2.reg_code;
            rm = op1.reg_code;
            mrm = modrm_byte(mod, reg, rm);
        }
        else if (op1.type == OperandType::REG8 && op2.type == OperandType::MEM8)
        {
            mod = build_mod(op2);
            reg = op1.reg_code;
            rm = op2.modrm_rm;
            mrm = modrm_byte(mod, reg, rm);
        }
        else if (op1.type == OperandType::MEM8 && op2.type == OperandType::REG8)
        {
            mod = build_mod(op1);
            reg = op2.reg_code;
            rm = op1.modrm_rm;
            mrm = modrm_byte(mod, reg, rm);
        }
        else if ((op1.type == OperandType::REG8 || op1.type == OperandType::MEM8) &&
                 op2.type == OperandType::IMM8)
        {
            // r/m8 <- imm8 ; Group opcode uses opcode_ext in Reg field
            const uint8_t ext = info->opcode_ext; // e.g., ADD = 0
            if (op1.type == OperandType::REG8)
            {
                mod = 0b11;
                reg = ext;
                rm = op1.reg_code;
            }
            else
            {
                mod = build_mod(op1);
                reg = ext;
                rm = op1.modrm_rm;
            }
            mrm = modrm_byte(mod, reg, rm);
        }
        else if (op1.type == OperandType::MEM16 && op2.type == OperandType::MEM16)
        {
            fatal_error("Memory-to-memory operation not encodable (use a register).");
        }
        else
        {
            fatal_error("Unhandled ModR/M combination.");
        }

        /*
        auto disp_size = [&](const ParsedOperand &op) -> int
        {
            uint8_t m = build_mod(op);
            if (m == 0b00 && op.modrm_rm == 0b110)
                return 2;
            if (m == 0b01)
                return 1;
            if (m == 0b10)
                return 2;
            return 0;
        };
        */

        std::cout << "ModR/M byte: 0x" << std::hex << (int)mrm << "\n";
        (*lcPointer)++;

        // Print displacement if present/required
        // (We recompute mod here the same way to decide printing)
        if ((op1.type == OperandType::MEM8 || op1.type == OperandType::MEM16))
        {
            uint8_t m = build_mod(op1);
            print_disp(op1, m);
            if (m == 0b01)
                (*lcPointer) += 1;
            else if (m == 0b10 || (m == 0b00 && op1.modrm_rm == 0b110))
                (*lcPointer) += 2;
        }
        if ((op2.type == OperandType::MEM8 || op2.type == OperandType::MEM16))
        {
            uint8_t m = build_mod(op2);
            print_disp(op2, m);
            if (m == 0b01)
                (*lcPointer) += 1;
            else if (m == 0b10 || (m == 0b00 && op2.modrm_rm == 0b110))
                (*lcPointer) += 2;
        }
    }

    // 3) Immediate (if any)
    if (info && info->has_imm)
    {
        // Choose which operand carries the immediate
        const ParsedOperand *immOp = nullptr;
        if (op2.type == OperandType::IMM8 || op2.type == OperandType::IMM16)
            immOp = &op2;
        else if (op1.type == OperandType::IMM8 || op1.type == OperandType::IMM16)
            immOp = &op1;

        if (!immOp)
            fatal_error("Opcode expects immediate but none was parsed.");

        if (immOp->type == OperandType::CHAR)
        {
            uint8_t c = static_cast<uint8_t>(immOp->value[0]);
            std::cout << "Char byte: 0x" << std::hex << (int)c << "\n";
            (*lcPointer) += 1;
        }
        else if (immOp->type == OperandType::STRING)
        {
            for (char ch : immOp->value)
            {
                uint8_t b = static_cast<uint8_t>(ch);
                std::cout << "String byte: 0x" << std::hex << (int)b << "\n";
                (*lcPointer) += 1;
            }
        } else {

            unsigned long immParsed = std::stoul(immOp->value, nullptr, 0);
            if (info->imm_size == 1)
            {
                std::cout << "Immediate byte: 0x" << std::hex << (int)u8((int)immParsed) << "\n";
                (*lcPointer) += 1;
            }
            else if (info->imm_size == 2)
            {
                std::cout << "Immediate word: 0x" << std::hex << u16(static_cast<unsigned>(immParsed)) << "\n";
                (*lcPointer) += 2;
            }
            else
            {
                fatal_error("Unsupported immediate size in opcode table");
            }
        }
    }
}

int parseNumber(const std::string &s, size_t &pos)
{
    while (pos < s.size() && isspace(s[pos]))
        pos++; // skip spaces
    int sign = 1;
    if (pos < s.size() && (s[pos] == '+' || s[pos] == '-'))
    {
        if (s[pos] == '-')
            sign = -1;
        pos++;
    }
    while (pos < s.size() && isspace(s[pos]))
        pos++;

    int num = 0;
    bool found = false;
    while (pos < s.size() && isdigit(s[pos]))
    {
        num = num * 10 + (s[pos] - '0');
        pos++;
        found = true;
    }
    if (!found)
        throw std::runtime_error("Expected number at position " + std::to_string(pos));
    return sign * num;
}

int parseFactor(const std::string &s, size_t &pos)
{
    while (pos < s.size() && isspace(s[pos]))
        pos++;
    if (pos < s.size() && s[pos] == '(')
    {
        pos++; // skip '('
        int val = parseExpression(s, pos);
        while (pos < s.size() && isspace(s[pos]))
            pos++;
        if (pos >= s.size() || s[pos] != ')')
            throw std::runtime_error("Expected ')' at position " + std::to_string(pos));
        pos++; // skip ')'
        return val;
    }
    else
    {
        return parseNumber(s, pos);
    }
}

int parseTerm(const std::string &s, size_t &pos)
{
    int val = parseFactor(s, pos);
    while (true)
    {
        while (pos < s.size() && isspace(s[pos]))
            pos++;
        if (pos >= s.size())
            break;
        char op = s[pos];
        if (op != '*' && op != '/')
            break;
        pos++;
        int rhs = parseFactor(s, pos);
        if (op == '*')
            val *= rhs;
        else if (op == '/')
            val /= rhs;
    }
    return val;
}

int parseExpression(const std::string &s, size_t &pos)
{
    int val = parseTerm(s, pos);
    while (true)
    {
        while (pos < s.size() && isspace(s[pos]))
            pos++;
        if (pos >= s.size())
            break;
        char op = s[pos];
        if (op != '+' && op != '-')
            break;
        pos++;
        int rhs = parseTerm(s, pos);
        if (op == '+')
            val += rhs;
        else if (op == '-')
            val -= rhs;
    }
    return val;
}

int evalWithParentheses(const std::string &expr)
{
    size_t pos = 0; // position
    int result = parseExpression(expr, pos);
    while (pos < expr.size() && isspace(expr[pos]))
        pos++;
    if (pos != expr.size())
        throw std::runtime_error("Unexpected characters at end of expression");
    return result;
}



/**
 * @brief Aligns the given address up to the nearest multiple of alignment.
 *
 * This function adjusts the input address so that it is aligned to the specified
 * alignment boundary. If the alignment is a power of two, a fast bitwise operation
 * is used. Otherwise, a modulo operation is performed.
 *
 * @param current_address The current address or offset to be aligned.
 * @param alignment The alignment boundary. Must be non-zero.
 * @return uint32_t The aligned address, which is the smallest address >= current_address
 *                  that is a multiple of alignment.
 */
uint32_t align_address(uint32_t current_address, uint32_t alignment)
{
    if (alignment == 0)
        return current_address; // error check: avoid division/mod by zero

    // Check if alignment is a power of two: only one bit set in 'alignment'
    if ((alignment & (alignment - 1)) == 0)
    {
        // Fast bitwise alignment for power-of-two alignment
        return (current_address + alignment - 1) & ~(alignment - 1);
    }
    else
    {
        // General alignment using modulo for non-power-of-two alignment
        uint32_t remainder = current_address % alignment;
        if (remainder == 0)
            return current_address;
        else
            return current_address + (alignment - remainder);
    }
}
