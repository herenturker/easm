#include "include/parser_handler.h"
#include "include/opcode_table.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

std::unordered_map<std::string, int> label_table;

int current_bits_mode = 16;

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
bool is_label_token(const std::string& token, const std::string& lexeme) {
    if (token == "LABEL") return true;
    if (!lexeme.empty() && lexeme.back() == ':') return true;
    return false;
}

/**
 * @brief Removes the trailing colon from a label token.
 *
 * @param token The token string (used for label check).
 * @param lexeme The label lexeme string (with trailing colon).
 * @return The label string without the trailing colon.
 */
std::string strip_label(const std::string& token, const std::string& lexeme) {
    if (is_label_token(token, lexeme)) {
        if (!lexeme.empty() && lexeme.back() == ':')
            return lexeme.substr(0, lexeme.size() - 1);
        else
            return lexeme;
    }
    return lexeme;
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
OperandType get_operand_type_from_token(const std::string& token) {
    if (token.find("REG8_") == 0) return OperandType::REG8;
    if (token == "REG8") return OperandType::REG8;

    if (token.find("REG16_") == 0) return OperandType::REG16;
    if (token == "REG16") return OperandType::REG16;

    if (token.find("REG32_") == 0) return OperandType::REG32;
    if (token == "REG32") return OperandType::REG32;

    if (token.find("SEGREG_") == 0) return OperandType::SEGREG;
    if (token == "SEGREG") return OperandType::SEGREG;

    if (token == "IMM8") return OperandType::IMM8;
    if (token == "IMM16") return OperandType::IMM16;
    if (token == "IMM32") return OperandType::IMM32;

    if (token == "MEM8") return OperandType::MEM8;
    if (token == "MEM16") return OperandType::MEM16;
    if (token == "MEM32") return OperandType::MEM32;

    if (token == "NUMBER") {
        return (current_bits_mode == 16) ? OperandType::IMM16 : OperandType::IMM32;
    }

    return OperandType::NONE;
}

/**
 * @brief Processes assembler directives and updates assembler state accordingly.
 *
 * Supports directives such as BITS, ORG, DB, DW, DD, DQ, DT, EQU, SECTION, EXTERN, GLOBAL,
 * ALIGN, TIMES, and can be easily extended.
 *
 * @param directive The directive keyword (e.g., "BITS", "ORG").
 * @param lexemes The vector of lexemes representing the directive and its arguments.
 * @return true if the directive was successfully handled; false if an error or unknown directive occurs.
 */
bool handle_directive(const std::string& directive, const std::vector<std::string>& lexemes) {
    try {
        if (directive == "BITS") {
            if (lexemes.size() < 2) {
                std::cerr << "Error: BITS directive requires an argument (16, 32, or 64)." << std::endl;
                return false;
            }
            int bits = std::stoi(lexemes[1]);
            if (bits == 16 || bits == 32 || bits == 64) {
                current_bits_mode = bits;
                std::cout << "Set BITS mode to " << bits << std::endl;
                return true;
            } else {
                std::cerr << "Error: Invalid BITS mode specified: " << bits << std::endl;
                return false;
            }
        }
        else if (directive == "ORG") {
            if (lexemes.size() < 2) {
                std::cerr << "Error: ORG directive requires an address argument." << std::endl;
                return false;
            }
            int origin = std::stoi(lexemes[1], nullptr, 0);
            std::cout << "Set ORG to address 0x" << std::hex << origin << std::dec << std::endl;
            return true;
        }
        else if (directive == "DB" || directive == "DW" || directive == "DD" ||
                 directive == "DQ" || directive == "DT") {
            if (lexemes.size() < 2) {
                std::cerr << "Error: " << directive << " directive requires at least one argument." << std::endl;
                return false;
            }
            std::cout << directive << " directive with data:";
            for (size_t i = 1; i < lexemes.size(); ++i) {
                std::cout << " " << lexemes[i];
            }
            std::cout << std::endl;
            return true;
        }
        else if (directive == "EQU") {
            if (lexemes.size() < 3) {
                std::cerr << "Error: EQU directive requires a label and a value." << std::endl;
                return false;
            }
            std::cout << "EQU directive: define " << lexemes[1] << " = " << lexemes[2] << std::endl;
            return true;
        }
        else if (directive == "SECTION") {
            if (lexemes.size() < 2) {
                std::cerr << "Error: SECTION directive requires a section name." << std::endl;
                return false;
            }
            std::cout << "Switching to SECTION: " << lexemes[1] << std::endl;
            return true;
        }
        else if (directive == "EXTERN") {
            if (lexemes.size() < 2) {
                std::cerr << "Error: EXTERN directive requires at least one symbol name." << std::endl;
                return false;
            }
            std::cout << "Declaring EXTERN symbols:";
            for (size_t i = 1; i < lexemes.size(); ++i) {
                std::cout << " " << lexemes[i];
            }
            std::cout << std::endl;
            return true;
        }
        else if (directive == "GLOBAL") {
            if (lexemes.size() < 2) {
                std::cerr << "Error: GLOBAL directive requires at least one symbol name." << std::endl;
                return false;
            }
            std::cout << "Declaring GLOBAL symbols:";
            for (size_t i = 1; i < lexemes.size(); ++i) {
                std::cout << " " << lexemes[i];
            }
            std::cout << std::endl;
            return true;
        }
        else if (directive == "ALIGN") {
            if (lexemes.size() < 2) {
                std::cerr << "Error: ALIGN directive requires an alignment value." << std::endl;
                return false;
            }
            int alignment = std::stoi(lexemes[1]);
            std::cout << "Aligning to " << alignment << "-byte boundary" << std::endl;
            return true;
        }
        else if (directive == "TIMES") {
            if (lexemes.size() < 3) {
                std::cerr << "Error: TIMES directive requires a repeat count and a value." << std::endl;
                return false;
            }
            int count = std::stoi(lexemes[1]);
            std::cout << "TIMES directive: repeat " << lexemes[2] << " " << count << " times" << std::endl;
            return true;
        }
        else {
            std::cerr << "Warning: Unknown directive encountered: " << directive << std::endl;
            return false;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid argument for directive " << directive << ": " << e.what() << std::endl;
        return false;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Error: Argument out of range for directive " << directive << ": " << e.what() << std::endl;
        return false;
    }
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
void handle_parse(std::vector<std::string> token_vector, std::vector<std::string> lexeme_vector) {
    init_opcode_table();

    if (token_vector.empty()) {
        std::cerr << "Error: Empty token vector provided." << std::endl;
        return;
    }

    if (is_label_token(token_vector[0], lexeme_vector[0])) {
        std::string label_name = strip_label(token_vector[0], lexeme_vector[0]);
        label_table[label_name] = 0;
        std::cout << "Label detected and stored: " << label_name << std::endl;

        token_vector.erase(token_vector.begin());
        lexeme_vector.erase(lexeme_vector.begin());

        if (token_vector.empty()) {
            return;
        }
    }

    const std::string directive_prefix = "DIRECTIVE_";
    if (token_vector[0].find(directive_prefix) == 0) {
        std::string directive = lexeme_vector[0];
        if (!handle_directive(directive, lexeme_vector)) {
            std::cerr << "Error processing directive: " << directive << std::endl;
        }
        return;
    }

    const std::string instr_prefix = "INSTR_";
    if (token_vector[0].find(instr_prefix) != 0) {
        std::cerr << "Error: First token is neither an instruction nor a directive: " << token_vector[0] << std::endl;
        return;
    }

    std::string mnemonic = token_vector[0].substr(instr_prefix.length());

    OperandType op1 = OperandType::NONE;
    OperandType op2 = OperandType::NONE;

    size_t idx = 1;

    if (idx < token_vector.size() && token_vector[idx] != "EOL") {
        op1 = get_operand_type_from_token(token_vector[idx]);
        if (op1 == OperandType::NONE) {
            std::cerr << "Error: Unknown operand type encountered: " << token_vector[idx] << std::endl;
            return;
        }
        ++idx;
    }

    if (idx < token_vector.size() && token_vector[idx] == "COMMA") {
        ++idx;
    }

    if (idx < token_vector.size() && token_vector[idx] != "EOL") {
        op2 = get_operand_type_from_token(token_vector[idx]);
        if (op2 == OperandType::NONE) {
            std::cerr << "Error: Unknown operand type encountered: " << token_vector[idx] << std::endl;
            return;
        }
    }

OperandKey key{ mnemonic, op1, op2 };

auto it = opcode_map.find(key);
if (it == opcode_map.end()) {
    std::cerr << "Error: No opcode found for instruction combination: " << mnemonic
              << " " << (op1 != OperandType::NONE ? lexeme_vector[1] : "")
              << (op2 != OperandType::NONE ? ", " + lexeme_vector[(lexeme_vector.size() > 3) ? 3 : 2] : "")
              << std::endl;
    return;
}

const OpcodeInfo& op_info = it->second;

std::cout << "Parsed instruction: " << mnemonic
          << " Operand1: " << static_cast<int>(op1)
          << " Operand2: " << static_cast<int>(op2)
          << " Opcode: 0x" << std::hex << static_cast<int>(op_info.primary_opcode) << std::dec << std::endl;
}