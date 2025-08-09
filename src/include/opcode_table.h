#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H

#ifdef __cplusplus
#include <string>
#include <unordered_map>
#include <stdint.h>

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
    // IMM32,  /**< Immediate value (32-bit). */ // I removed these because the assembler only supports 16 bit operations (for now. they are as comments here for future usage)
    REG8,   /**< General-purpose register (8-bit). */
    REG16,  /**< General-purpose register (16-bit). */
    // REG32,  /**< General-purpose register (32-bit). */
    MEM8,   /**< Memory operand (8-bit). */
    MEM16,  /**< Memory operand (16-bit). */
    // MEM32,  /**< Memory operand (32-bit). */
    SEGREG,  /**< Segment register. */
    STRING /**< String expression */
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
