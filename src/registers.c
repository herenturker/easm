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

/**
 * @brief Returns the enum value of a 32-bit register by its name.
 *
 * @param name Null-terminated string name of the register (e.g., "EAX").
 * @return Register32 Corresponding enum value, or REG32_NONE if unknown.
 */
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
