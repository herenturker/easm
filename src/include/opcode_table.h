#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H
#ifdef __cplusplus
#include <string>
#include <unordered_map>
#include <stdint.h>


enum class OperandType {
    NONE,
    IMM8,
    IMM16,
    IMM32,
    REG8,
    REG16,
    REG32,
    MEM8,
    MEM16,
    MEM32,
    SEGREG
};


struct OpcodeInfo {
    uint8_t primary_opcode;  // The main opcode byte for the instruction
    bool requires_modrm;     // Whether this opcode requires a ModRM byte (extra info about operands)
    bool has_imm;            // Whether this instruction includes an immediate value
    int imm_size;            // Size (in bytes) of the immediate value, if any
};



struct OperandKey {
    std::string mnemonic;
    OperandType op1;
    OperandType op2;

    bool operator==(const OperandKey &other) const {
        return mnemonic == other.mnemonic && op1 == other.op1 && op2 == other.op2;
    }
};


struct OperandKeyHash {
    std::size_t operator()(const OperandKey& k) const {
        return std::hash<std::string>()(k.mnemonic)
             ^ (std::hash<int>()((int)k.op1) << 1)
             ^ (std::hash<int>()((int)k.op2) << 2);
    }
};


extern std::unordered_map<OperandKey, OpcodeInfo, OperandKeyHash> opcode_map;

void init_opcode_table();

#endif // __cplusplus
#endif // OPCODE_TABLE_H
