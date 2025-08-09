#include "include/parser_handler.h"
#include "include/opcode_table.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <limits>

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
std::string strip_label(const std::vector<std::string>& tokens,
                        const std::vector<std::string>& lexemes,
                        size_t idx)
{
    // Handle local labels like ".loop:"
    if (tokens[idx] == "DOT" && idx + 1 < tokens.size() && tokens[idx + 1] == "LABEL") {
        std::string combined = "." + lexemes[idx + 1];
        return combined; // No colon in lexeme since colon is separate token
    }

    // Handle normal labels like "start:"
    if (tokens[idx] == "LABEL") {
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
OperandType get_operand_type_from_token(const std::string& token, const std::string& lexeme) {
    if (token.find("REG8_") == 0) return OperandType::REG8;
    if (token == "REG8") return OperandType::REG8;

    if (token.find("REG16_") == 0) return OperandType::REG16;
    if (token == "REG16") return OperandType::REG16;

    // if (token.find("REG32_") == 0) return OperandType::REG32;
    // if (token == "REG32") return OperandType::REG32;

    if (token.find("SEGREG_") == 0) return OperandType::SEGREG;
    if (token == "SEGREG") return OperandType::SEGREG;

    // if (token == "IMM8") return OperandType::IMM8;
    // if (token == "IMM16") return OperandType::IMM16;
    // if (token == "IMM32") return OperandType::IMM32;

    if (token == "MEM8") return OperandType::MEM8;
    if (token == "MEM16") return OperandType::MEM16;
    // if (token == "MEM32") return OperandType::MEM32;

    if (token == "STRING") {
        return OperandType::STRING;
    }


    if (token == "NUMBER") {
    // Parse the lexeme into an integer
    long value = std::stol(lexeme, nullptr, 0); // auto-detects base (0x.., decimal, etc.)

    // Check range for signed or unsigned 8-bit
    if (value >= -128 && value <= 255) {
        return OperandType::IMM8;
    } else {
        return OperandType::IMM16;
        }
    }
    return OperandType::NONE;
}

/**
 * @brief Parse lexeme to an unsigned integer only if the token indicates a number.
 *
 * This function only attempts to parse when token == "NUMBER".
 * It uses base=0 so that "0x"/"0X" hex, leading-zero octal, and decimal are supported.
 *
 * @param token The token string (e.g., "NUMBER", "REG16_AX").
 * @param lexeme The original lexeme string (e.g., "0X89E4", "16").
 * @param out_value Reference to store the parsed value if successful.
 * @return true if parsing succeeded (and out_value is valid), false otherwise.
 */
static bool parse_number_if_token(const std::string& token, const std::string& lexeme, uint32_t &out_value) {
    if (token != "NUMBER") {
        return false; // Not a numeric token, caller shouldn't treat it as a number.
    }

    try {
        // std::stoul with base=0 will accept 0x/0X hex, 0... octal, or decimal.
        unsigned long val = std::stoul(lexeme, nullptr, 0);

        // Clamp to 32-bit range as a precaution.
        if (val > std::numeric_limits<uint32_t>::max()) {
            std::cerr << "Error: Numeric literal out of range for 32-bit: " << lexeme << std::endl;
            return false;
        }

        out_value = static_cast<uint32_t>(val);
        return true;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid numeric literal: " << lexeme << " (" << e.what() << ")" << std::endl;
        return false;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Error: Numeric literal out of range: " << lexeme << " (" << e.what() << ")" << std::endl;
        return false;
    }
}

/**
 * @brief Processes assembler directives and updates assembler state accordingly.
 *
 * Supports directives such as BITS, ORG, DB, DW, DD, DQ, DT, EQU, SECTION, EXTERN, GLOBAL,
 * ALIGN, TIMES, and can be easily extended.
 *
 * NOTE: This function does not change: it assumes directive arguments are provided
 * as lexemes.
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
            int bits = std::stoi(lexemes[1], nullptr, 0);
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
            int alignment = std::stoi(lexemes[1], nullptr, 0);
            std::cout << "Aligning to " << alignment << "-byte boundary" << std::endl;
            return true;
        }
        else if (directive == "TIMES") {
            if (lexemes.size() < 3) {
                std::cerr << "Error: TIMES directive requires a repeat count and a value." << std::endl;
                return false;
            }
            int count = std::stoi(lexemes[1], nullptr, 0);
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

    // Handle label at start
    if (is_label_token(token_vector[0], lexeme_vector[0])) {
        std::string label_name = strip_label(token_vector, lexeme_vector, 0);
        label_table[label_name] = 0;
        std::cout << "Label detected and stored: " << label_name << std::endl;

        token_vector.erase(token_vector.begin());
        lexeme_vector.erase(lexeme_vector.begin());

        if (token_vector.empty()) {
            return;
        }
    }

    // Remove trailing EOL tokens to avoid treating them as operands
    while (!token_vector.empty() && (token_vector.back() == "<EOL>" || token_vector.back() == "EOL")) {
        token_vector.pop_back();
        lexeme_vector.pop_back();
    }

    const std::string directive_prefix = "DIRECTIVE_";
    const std::string instr_prefix = "INSTR_";

    if (token_vector.empty()) {
        // std::cerr << "Error: No tokens left after removing EOLs." << std::endl;
        return;
    }

    if (token_vector[0].find(directive_prefix) == 0) {
        std::string directive = lexeme_vector[0];
        if (!handle_directive(directive, lexeme_vector)) {
            std::cerr << "Error processing directive: " << directive << std::endl;
        }
        return;
    }

    if (token_vector[0].find(instr_prefix) != 0 && token_vector[0].find(directive_prefix) != 0) {
        std::cerr << "Error: First token is neither an instruction nor a directive: " << token_vector[0] << std::endl;
        return;
    }

    std::string mnemonic = token_vector[0].substr(instr_prefix.length());

    OperandType op1 = OperandType::NONE;
    OperandType op2 = OperandType::NONE;

    size_t token_count = token_vector.size();
    size_t lex_count = lexeme_vector.size();

    // Determine operand token indices
    size_t op1_token_idx = 1;
    size_t op2_token_idx = (token_count > 2 && token_vector[2] == "COMMA") ? 3 : 2;

    // Resolve operand 1
    if (op1_token_idx < token_count && op1_token_idx < lex_count) {
        const std::string& tok1 = token_vector[op1_token_idx];
        if (tok1 == "<EOL>" || tok1 == "EOL") {
            op1 = OperandType::NONE;
        } else {
            op1 = get_operand_type_from_token(tok1, lexeme_vector[op1_token_idx]);
            if (op1 == OperandType::NONE) {
                std::cerr << "Error: Unknown operand type encountered: " << tok1 << std::endl;
                return;
            }
        }
    }

    // Resolve operand 2
    if (op2_token_idx < token_count && op2_token_idx < lex_count) {
        const std::string& tok2 = token_vector[op2_token_idx];
        if (tok2 == "<EOL>" || tok2 == "EOL") {
            op2 = OperandType::NONE;
        } else {
            op2 = get_operand_type_from_token(tok2, lexeme_vector[op2_token_idx]);
            if (op2 == OperandType::NONE) {
                std::cerr << "Error: Unknown operand type encountered: " << tok2 << std::endl;
                return;
            }
        }
    }

    // Parse immediate values if token is NUMBER
    uint32_t imm_value1 = 0, imm_value2 = 0;
    bool has_imm1 = (op1_token_idx < token_count && op1_token_idx < lex_count) &&
                    parse_number_if_token(token_vector[op1_token_idx], lexeme_vector[op1_token_idx], imm_value1);
    bool has_imm2 = (op2_token_idx < token_count && op2_token_idx < lex_count) &&
                    parse_number_if_token(token_vector[op2_token_idx], lexeme_vector[op2_token_idx], imm_value2);

    OperandKey key{mnemonic, op1, op2};

    auto it = opcode_map.find(key);
    if (it == opcode_map.end()) {
        std::string disp_op1 = (op1_token_idx < lex_count) ? lexeme_vector[op1_token_idx] : "";
        std::string disp_op2 = (op2_token_idx < lex_count) ? lexeme_vector[op2_token_idx] : "";

        std::cerr << "Error: No opcode found for instruction combination: " << mnemonic;
        if (!disp_op1.empty()) std::cerr << " " << disp_op1;
        if (!disp_op2.empty()) std::cerr << ", " << disp_op2;
        std::cerr << std::endl;
        return;
    }

    const OpcodeInfo& op_info = it->second;

    std::cout << "Parsed instruction: " << mnemonic
              << " Operand1: " << static_cast<int>(op1)
              << " Operand2: " << static_cast<int>(op2)
              << " Opcode: 0x" << std::hex << static_cast<int>(op_info.primary_opcode) << std::dec;

    if (has_imm1) {
        std::cout << " Imm1=0x" << std::hex << imm_value1 << std::dec << " (" << imm_value1 << ")";
    }
    if (has_imm2) {
        std::cout << " Imm2=0x" << std::hex << imm_value2 << std::dec << " (" << imm_value2 << ")";
    }

    std::cout << std::endl;
}
