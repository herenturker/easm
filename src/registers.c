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

// registers.c
#include <string.h>
#include "include/registers.h"

/**
 * @brief Returns the name of an 8-bit register.
 *
 * @param reg Enum value representing the 8-bit register.
 * @return const char* String representation of the register, or "UNKNOWN" if invalid.
 */
const char *get_register8_name(Register8 reg) {
    switch (reg) {
        case REG8_AL: return "AL";
        case REG8_BL: return "BL";
        case REG8_CL: return "CL";
        case REG8_DL: return "DL";
        case REG8_AH: return "AH";
        case REG8_BH: return "BH";
        case REG8_CH: return "CH";
        case REG8_DH: return "DH";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Returns the enum value of an 8-bit register by its name.
 *
 * @param name Null-terminated string name of the register (e.g., "AL").
 * @return Register8 Corresponding enum value, or REG8_NONE if unknown.
 */
Register8 get_register8_by_name(const char *name) {
    if (strcmp(name, "AL") == 0) return REG8_AL;
    if (strcmp(name, "BL") == 0) return REG8_BL;
    if (strcmp(name, "CL") == 0) return REG8_CL;
    if (strcmp(name, "DL") == 0) return REG8_DL;
    if (strcmp(name, "AH") == 0) return REG8_AH;
    if (strcmp(name, "BH") == 0) return REG8_BH;
    if (strcmp(name, "CH") == 0) return REG8_CH;
    if (strcmp(name, "DH") == 0) return REG8_DH;
    return REG8_NONE;
}

/**
 * @brief Returns the name of a 16-bit register.
 *
 * @param reg Enum value representing the 16-bit register.
 * @return const char* String representation of the register, or "UNKNOWN" if invalid.
 */
const char *get_register16_name(Register16 reg) {
    switch (reg) {
        case REG16_AX: return "AX";
        case REG16_BX: return "BX";
        case REG16_CX: return "CX";
        case REG16_DX: return "DX";
        case REG16_SI: return "SI";
        case REG16_DI: return "DI";
        case REG16_BP: return "BP";
        case REG16_SP: return "SP";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Returns the enum value of a 16-bit register by its name.
 *
 * @param name Null-terminated string name of the register (e.g., "AX").
 * @return Register16 Corresponding enum value, or REG16_NONE if unknown.
 */
Register16 get_register16_by_name(const char *name) {
    if (strcmp(name, "AX") == 0) return REG16_AX;
    if (strcmp(name, "BX") == 0) return REG16_BX;
    if (strcmp(name, "CX") == 0) return REG16_CX;
    if (strcmp(name, "DX") == 0) return REG16_DX;
    if (strcmp(name, "SI") == 0) return REG16_SI;
    if (strcmp(name, "DI") == 0) return REG16_DI;
    if (strcmp(name, "BP") == 0) return REG16_BP;
    if (strcmp(name, "SP") == 0) return REG16_SP;
    return REG16_NONE;
}

/**
 * @brief Returns the name of a 32-bit register.
 *
 * @param reg Enum value representing the 32-bit register.
 * @return const char* String representation of the register, or "UNKNOWN" if invalid.
 */

/*
const char *get_register32_name(Register32 reg) {
    switch (reg) {
        case REG32_EAX: return "EAX";
        case REG32_EBX: return "EBX";
        case REG32_ECX: return "ECX";
        case REG32_EDX: return "EDX";
        case REG32_ESI: return "ESI";
        case REG32_EDI: return "EDI";
        case REG32_EBP: return "EBP";
        case REG32_ESP: return "ESP";
        default: return "UNKNOWN";
    }
}
*/

/**
 * @brief Returns the enum value of a 32-bit register by its name.
 *
 * @param name Null-terminated string name of the register (e.g., "EAX").
 * @return Register32 Corresponding enum value, or REG32_NONE if unknown.
 */
/*
Register32 get_register32_by_name(const char *name) {
    if (strcmp(name, "EAX") == 0) return REG32_EAX;
    if (strcmp(name, "EBX") == 0) return REG32_EBX;
    if (strcmp(name, "ECX") == 0) return REG32_ECX;
    if (strcmp(name, "EDX") == 0) return REG32_EDX;
    if (strcmp(name, "ESI") == 0) return REG32_ESI;
    if (strcmp(name, "EDI") == 0) return REG32_EDI;
    if (strcmp(name, "EBP") == 0) return REG32_EBP;
    if (strcmp(name, "ESP") == 0) return REG32_ESP;
    return REG32_NONE;
}
*/

/**
 * @brief Returns the name of a segment register.
 *
 * @param reg Enum value representing the segment register.
 * @return const char* String representation of the segment register, or "UNKNOWN" if invalid.
 */
const char *get_segment_register_name(SegmentRegister reg) {
    switch (reg) {
        case SEGREG_CS: return "CS";
        case SEGREG_DS: return "DS";
        case SEGREG_SS: return "SS";
        case SEGREG_ES: return "ES";
        case SEGREG_FS: return "FS";
        case SEGREG_GS: return "GS";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Returns the enum value of a segment register by its name.
 *
 * @param name Null-terminated string name of the register (e.g., "DS").
 * @return SegmentRegister Corresponding enum value, or SEGREG_NONE if unknown.
 */
SegmentRegister get_segment_register_by_name(const char *name) {
    if (strcmp(name, "CS") == 0) return SEGREG_CS;
    if (strcmp(name, "DS") == 0) return SEGREG_DS;
    if (strcmp(name, "SS") == 0) return SEGREG_SS;
    if (strcmp(name, "ES") == 0) return SEGREG_ES;
    if (strcmp(name, "FS") == 0) return SEGREG_FS;
    if (strcmp(name, "GS") == 0) return SEGREG_GS;
    return SEGREG_NONE;
}


/**
 * @brief Convert an 8-bit register enum value to its string representation.
 *
 * @param reg_type The Register8 enum value.
 * @return A constant string representing the 8-bit register name.
 */
const char *reg8_type_to_string(Register8 reg_type) {
    switch (reg_type) {
        case REG8_AL: return "REG8_AL";
        case REG8_BL: return "REG8_BL";
        case REG8_CL: return "REG8_CL";
        case REG8_DL: return "REG8_DL";
        case REG8_AH: return "REG8_AH";
        case REG8_BH: return "REG8_BH";
        case REG8_CH: return "REG8_CH";
        case REG8_DH: return "REG8_DH";
        default: return "REG8_UNKNOWN";
    }
}

/**
 * @brief Convert a 16-bit register enum value to its string representation.
 *
 * @param reg_type The Register16 enum value.
 * @return A constant string representing the 16-bit register name.
 */
const char *reg16_type_to_string(Register16 reg_type) {
    switch (reg_type) {
        case REG16_AX: return "REG16_AX";
        case REG16_BX: return "REG16_BX";
        case REG16_CX: return "REG16_CX";
        case REG16_DX: return "REG16_DX";
        case REG16_SI: return "REG16_SI";
        case REG16_DI: return "REG16_DI";
        case REG16_BP: return "REG16_BP";
        case REG16_SP: return "REG16_SP";
        default: return "REG16_UNKNOWN";
    }
}

/**
 * @brief Convert a 32-bit register enum value to its string representation.
 *
 * @param reg_type The Register32 enum value.
 * @return A constant string representing the 32-bit register name.
 */

 /*
const char *reg32_type_to_string(Register32 reg_type) {
    switch (reg_type) {
        case REG32_EAX: return "REG32_EAX";
        case REG32_EBX: return "REG32_EBX";
        case REG32_ECX: return "REG32_ECX";
        case REG32_EDX: return "REG32_EDX";
        case REG32_ESI: return "REG32_ESI";
        case REG32_EDI: return "REG32_EDI";
        case REG32_EBP: return "REG32_EBP";
        case REG32_ESP: return "REG32_ESP";
        default: return "REG32_UNKNOWN";
    }
}

*/

/**
 * @brief Convert a segment register enum value to its string representation.
 *
 * @param reg_type The SegmentRegister enum value.
 * @return A constant string representing the segment register name.
 */
const char *segreg_type_to_string(SegmentRegister reg_type) {
    switch (reg_type) {
        case SEGREG_CS: return "SEGREG_CS";
        case SEGREG_DS: return "SEGREG_DS";
        case SEGREG_SS: return "SEGREG_SS";
        case SEGREG_ES: return "SEGREG_ES";
        case SEGREG_FS: return "SEGREG_FS";
        case SEGREG_GS: return "SEGREG_GS";
        default: return "SEGREG_UNKNOWN";
    }
}
