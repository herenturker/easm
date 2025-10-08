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

#include "include/opcode_table.h"
#include "include/errors.h"

std::unordered_map<OperandKey, OpcodeInfo, OperandKeyHash> opcode_map;

void init_opcode_table()
{
    //             mnemonic       op1                  op2                  opcode  modrm   imm  imm size ext
    // MOV
    opcode_map[{"MOV", OperandType::REG16, OperandType::IMM16}] = {0xB8, false, true, 2, 0};
    opcode_map[{"MOV", OperandType::REG16, OperandType::REG16}] = {0x89, true, false, 0, 0};
    opcode_map[{"MOV", OperandType::REG16, OperandType::MEM16}] = {0x8B, true, false, 0, 0};
    opcode_map[{"MOV", OperandType::MEM16, OperandType::REG16}] = {0x89, true, false, 0, 0};


    // ADD r/m16, imm8 → Group 1, ext = 0
    opcode_map[{"ADD", OperandType::REG16, OperandType::IMM8}] = {0x83, true, true, 1, 0};
    opcode_map[{"ADD", OperandType::MEM16, OperandType::IMM8}] = {0x83, true, true, 1, 0};

    // NOP
    opcode_map[{"NOP", OperandType::NONE, OperandType::NONE}] = {0x90, false, false, 0, 0};
}

std::unordered_map<std::string, uint8_t> reg16_codes = {
    {"AX", 0}, {"CX", 1}, {"DX", 2}, {"BX", 3}, {"SP", 4}, {"BP", 5}, {"SI", 6}, {"DI", 7}};

std::unordered_map<std::string, uint8_t> reg8_codes = {
    {"AL", 0}, {"CL", 1}, {"DL", 2}, {"BL", 3}, {"AH", 4}, {"CH", 5}, {"DH", 6}, {"BH", 7}};

std::unordered_map<std::string, uint8_t> seg_codes = {
    {"CS", 0}, {"DS", 1}, {"SS", 2}, {"ES", 3}};

ParsedOperand parseOperand(const std::vector<std::string> &tokens,
                           const std::vector<std::string> &lexemes,
                           size_t &idx)
{
    ParsedOperand op;

    if (tokens[idx].find("REG16") != std::string::npos)
    {
        op.type = OperandType::REG16;
        op.value = lexemes[idx];
        auto it = reg16_codes.find(op.value);
        if (it == reg16_codes.end())
            fatal_error("Unknown REG16 ");
        op.reg_code = it->second;
        idx++;
    }
    else if (tokens[idx].find("REG8") != std::string::npos)
    {
        op.type = OperandType::REG8;
        op.value = lexemes[idx];
        auto it = reg8_codes.find(op.value);
        if (it == reg8_codes.end())
            fatal_error("Unknown REG8 ");
        op.reg_code = it->second;
        idx++;
    }
    else if (tokens[idx].find("SEG") != std::string::npos)
    {
        op.type = OperandType::SEGREG;
        op.value = lexemes[idx];
        auto it = seg_codes.find(op.value);
        if (it == seg_codes.end())
            fatal_error("Unknown segment register ");
        op.seg_code = it->second;
        idx++;
    }
    else if (tokens[idx].find("NUMBER") != std::string::npos)
    {
        op.type = OperandType::IMM16;
        op.value = lexemes[idx];
        idx++;
    }
    else if (tokens[idx] == "OPEN_BRACKET")
    {
        idx++;
        std::string mem_expr;
        while (idx < tokens.size() && tokens[idx] != "CLOSING_BRACKET")
        {
            mem_expr += lexemes[idx];
            idx++;
        }
        if (idx == tokens.size())
            fatal_error("Unmatched [ in memory operand");
        idx++;
        op.type = OperandType::MEM16;
        op.value = mem_expr;
    }
    else if (tokens[idx] == "CHAR")
    {
        op.type = OperandType::CHAR;
        op.value = lexemes[idx];
        idx++;
    }
    else if (tokens[idx] == "STRING")
    {
        op.type = OperandType::STRING;
        op.value = lexemes[idx];
        idx++;
    }
    else
    {
        fatal_error("Unknown operand type");
    }

    return op;
}
