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

#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

#ifdef __cplusplus
#include <string>
#include <unordered_map>
#include <stdint.h>
#include <vector>

/**
 * @enum OperandType
 * @brief Describes the type of an instruction operand.
 *
 * This enumeration specifies the size and kind of operand used by an instruction.
 * It can be an immediate value, a register, a memory reference, or none at all.
 */
enum class OperandType {
    NONE,   /**< No operand. */
    IMM8,   /**< Immediate value (8-bit). */
    IMM16,  /**< Immediate value (16-bit). */
    // IMM32,  /**< Immediate value (32-bit). */ // I removed these because the assembler only supports 16 bit operations (for now. they are as comments here for future usage maybe)
    REG8,   /**< General-purpose register (8-bit). */
    REG16,  /**< General-purpose register (16-bit). */
    // REG32,  /**< General-purpose register (32-bit). */
    MEM8,   /**< Memory operand (8-bit). */
    MEM16,  /**< Memory operand (16-bit). */
    // MEM32,  /**< Memory operand (32-bit). */
    SEGREG,  /**< Segment register. */
    STRING, /**< String expression */
    CHAR
};

/**
 * @struct OpcodeInfo
 * @brief Stores binary encoding information for a machine instruction.
 *
 * This structure holds the primary opcode byte and other encoding
 * details necessary for assembling or decoding an instruction.
 */
struct OpcodeInfo {
    uint8_t primary_opcode;  /**< Main opcode byte for the instruction. */
    bool requires_modrm;     /**< True if the instruction requires a ModR/M byte. */
    bool has_imm;            /**< True if the instruction contains an immediate value. */
    int imm_size;            /**< Size of the immediate value in bytes (0 if none). */
    uint8_t opcode_ext;      /**< NEW: ModR/M reg field for group instructions. */
};

/**
 * @struct OperandKey
 * @brief Represents a lookup key for an opcode map.
 *
 * The key is based on the mnemonic string and the types of up to two operands.
 * Used in a hash map to retrieve the corresponding OpcodeInfo.
 */
struct OperandKey {
    std::string mnemonic; /**< Instruction mnemonic (e.g., "MOV", "ADD"). */
    OperandType op1;      /**< Type of the first operand. */
    OperandType op2;      /**< Type of the second operand. */

    /**
     * @brief Equality operator for OperandKey.
     * @param other The OperandKey to compare against.
     * @return True if mnemonic and both operand types match.
     */
    bool operator==(const OperandKey &other) const {
        return mnemonic == other.mnemonic && op1 == other.op1 && op2 == other.op2;
    }
};

struct ParsedOperand {
    OperandType type;
    std::string value;
    uint8_t reg_code;
    uint8_t seg_code;
    uint8_t modrm_rm;
    int16_t displacement;
};

ParsedOperand parseOperand(const std::vector<std::string>& tokens,
                           const std::vector<std::string>& lexemes,
                           size_t& idx);

/**
 * @struct OperandKeyHash
 * @brief Custom hash functor for OperandKey.
 *
 * Allows OperandKey to be used as a key in std::unordered_map
 * by combining the hashes of the mnemonic and operand types.
 */
struct OperandKeyHash {
    /**
     * @brief Computes a hash value for the given OperandKey.
     * @param k The OperandKey to hash.
     * @return A size_t hash value for the key.
     */
    std::size_t operator()(const OperandKey& k) const {
        return std::hash<std::string>()(k.mnemonic)
             ^ (std::hash<int>()((int)k.op1) << 1)
             ^ (std::hash<int>()((int)k.op2) << 2);
    }
};

// No operand instructions (including implicit operand string instructions)
const std::unordered_map<std::string, int> no_operand_instructions = {
    {"HLT", 0}, {"NOP", 0}, {"RET", 0}, {"LEAVE", 0},
    {"LOCK", 0}, {"WAIT", 0}, {"SALC", 0}, {"CLC", 0},
    {"STC", 0}, {"CMC", 0}, {"CLI", 0}, {"STD", 0},
    {"CLD", 0}, {"ESC", 0}, {"INT3", 0}, {"IRET", 0},
    {"SYSCALL", 0}, {"SYSRET", 0}, {"PUSHA", 0}, {"POPA", 0},
    {"LODS", 0}, {"LODSB", 0}, {"LODSW", 0}, {"LODSD", 0}, {"LODSQ", 0},
    {"STOS", 0}, {"SCAS", 0}, {"CMPS", 0}, {"REP", 0}, {"REPE", 0}, {"REPNE", 0},
    {"LOOP", 0}
};

// One operand instructions
const std::unordered_map<std::string, int> one_operand_instructions = {
    {"INT", 1}, {"PUSH", 1}, {"POP", 1}, {"INC", 1}, {"DEC", 1},
    {"NOT", 1}, {"NEG", 1}, {"SHL", 1}, {"SAL", 1}, {"SHR", 1},
    {"SAR", 1}, {"JMP", 1}, {"JE", 1}, {"JNE", 1}, {"JZ", 1},
    {"JNZ", 1}, {"JG", 1}, {"JGE", 1}, {"JL", 1}, {"JLE", 1},
    {"JA", 1}, {"JAE", 1}, {"JB", 1}, {"JBE", 1}, {"JS", 1},
    {"JNS", 1}, {"CALL", 1}, {"SET", 1}, {"IN", 1}, {"OUT", 1},
    {"ARPL", 1}, {"CLTS", 1}, {"INVLPG", 1}, {"VERR", 1}, {"VERW", 1},
    {"STR", 1}, {"LTR", 1}, {"LGDT", 1}, {"SGDT", 1},
    {"LIDT", 1}, {"SIDT", 1}, {"IDIV", 1}
};

// Two operand instructions
const std::unordered_map<std::string, int> two_operand_instructions = {
    {"MOV", 2}, {"LEA", 2}, {"ADD", 2}, {"SUB", 2}, {"IMUL", 2},
    {"AND", 2}, {"OR", 2}, {"XOR", 2}, {"TEST", 2}, {"CMP", 2},
    {"XCHG", 2}, {"MOVSX", 2}, {"MOVZX", 2}, {"BOUND", 2}, {"MOVCR", 2}
};



/**
 * @brief Global opcode map.
 *
 * Maps an OperandKey (mnemonic + operand types) to an OpcodeInfo structure
 * containing the binary encoding details for that instruction.
 */
extern std::unordered_map<OperandKey, OpcodeInfo, OperandKeyHash> opcode_map;

/**
 * @brief Initializes the opcode table.
 *
 * Populates the opcode_map with all supported instructions
 * and their corresponding binary encoding details.
 */
void init_opcode_table();

#endif // __cplusplus
#endif // OPCODE_TABLE_H
