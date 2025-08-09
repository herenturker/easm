#include "include/opcode_table.h"

std::unordered_map<OperandKey, OpcodeInfo, OperandKeyHash> opcode_map;

void init_opcode_table() {
    //          mnemonic       operand1           operand2          opcode  modrm   imm  imm size    
    opcode_map[{ "MOV", OperandType::REG16, OperandType::IMM16 }] = { 0xB8, false, true, 2 };
    opcode_map[{ "MOV", OperandType::REG16, OperandType::REG16 }] = { 0x89, true, false, 0 };
    opcode_map[{ "MOV", OperandType::REG16, OperandType::MEM16 }] = { 0x8B, true, false, 0 };
    opcode_map[{ "MOV", OperandType::MEM16, OperandType::REG16 }] = { 0x89, true, false, 0 };
    opcode_map[{ "ADD", OperandType::REG16, OperandType::IMM8  }] = { 0x83, true, true, 1 };

    
}
