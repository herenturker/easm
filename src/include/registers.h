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

#ifndef REGISTERS_H
#define REGISTERS_H

/**
 * @brief Enum for 8-bit general-purpose registers.
 */
typedef enum
{
    REG8_NONE = 0,  /**< No 8-bit register */
    REG8_AL,        /**< Accumulator Low */
    REG8_BL,        /**< Base Low */
    REG8_CL,        /**< Counter Low */
    REG8_DL,        /**< Data Low */
    REG8_AH,        /**< Accumulator High */
    REG8_BH,        /**< Base High */
    REG8_CH,        /**< Counter High */
    REG8_DH         /**< Data High */
} Register8;

/**
 * @brief Enum for 16-bit general-purpose registers.
 */
typedef enum
{
    REG16_NONE = 0, /**< No 16-bit register */
    REG16_AX,       /**< Accumulator Register */
    REG16_BX,       /**< Base Register */
    REG16_CX,       /**< Counter Register */
    REG16_DX,       /**< Data Register */
    REG16_SI,       /**< Source Index */
    REG16_DI,       /**< Destination Index */
    REG16_BP,       /**< Base Pointer */
    REG16_SP        /**< Stack Pointer */
} Register16;

/**
 * @brief Enum for 32-bit general-purpose registers.
 */


// typedef enum
// {
    // REG32_NONE = 0, /**< No 32-bit register */
    // REG32_EAX,      /**< Extended Accumulator */
    // REG32_EBX,      /**< Extended Base */
    // REG32_ECX,      /**< Extended Counter */
    // REG32_EDX,      /**< Extended Data */
    // REG32_ESI,      /**< Extended Source Index */
    // REG32_EDI,      /**< Extended Destination Index */
    // REG32_EBP,      /**< Extended Base Pointer */
    // REG32_ESP       /**< Extended Stack Pointer */
// } Register32;

/**
 * @brief Enum for segment registers.
 */
typedef enum
{
    SEGREG_NONE = 0, /**< No segment register */
    SEGREG_CS,       /**< Code Segment */
    SEGREG_DS,       /**< Data Segment */
    SEGREG_SS,       /**< Stack Segment */
    SEGREG_ES,       /**< Extra Segment */
    SEGREG_FS,       /**< FS Segment */
    SEGREG_GS        /**< GS Segment */
} SegmentRegister;

/**
 * @brief Gets the name of an 8-bit register.
 * 
 * @param reg 8-bit register enum value.
 * @return String representation of the register name.
 */
const char *get_register8_name(Register8 reg);

/**
 * @brief Resolves an 8-bit register from its name.
 * 
 * @param name Register name string.
 * @return Corresponding Register8 enum value.
 */
Register8 get_register8_by_name(const char *name);

/**
 * @brief Gets the name of a 16-bit register.
 * 
 * @param reg 16-bit register enum value.
 * @return String representation of the register name.
 */
const char *get_register16_name(Register16 reg);

/**
 * @brief Resolves a 16-bit register from its name.
 * 
 * @param name Register name string.
 * @return Corresponding Register16 enum value.
 */
Register16 get_register16_by_name(const char *name);

/**
 * @brief Gets the name of a 32-bit register.
 * 
 * @param reg 32-bit register enum value.
 * @return String representation of the register name.
 */
// const char *get_register32_name(Register32 reg);

/**
 * @brief Resolves a 32-bit register from its name.
 * 
 * @param name Register name string.
 * @return Corresponding Register32 enum value.
 */
// Register32 get_register32_by_name(const char *name);

/**
 * @brief Gets the name of a segment register.
 * 
 * @param reg Segment register enum value.
 * @return String representation of the register name.
 */
const char *get_segment_register_name(SegmentRegister reg);

/**
 * @brief Resolves a segment register from its name.
 * 
 * @param name Register name string.
 * @return Corresponding SegmentRegister enum value.
 */
SegmentRegister get_segment_register_by_name(const char *name);



/**
 * @brief Convert an 8-bit register enum value to its string representation.
 *
 * @param reg_type The Register8 enum value.
 * @return A constant string representing the 8-bit register name.
 */
const char *reg8_type_to_string(Register8 reg_type);

/**
 * @brief Convert a 16-bit register enum value to its string representation.
 *
 * @param reg_type The Register16 enum value.
 * @return A constant string representing the 16-bit register name.
 */
const char *reg16_type_to_string(Register16 reg_type);

/**
 * @brief Convert a 32-bit register enum value to its string representation.
 *
 * @param reg_type The Register32 enum value.
 * @return A constant string representing the 32-bit register name.
 */
// const char *reg32_type_to_string(Register32 reg_type);

/**
 * @brief Convert a segment register enum value to its string representation.
 *
 * @param reg_type The SegmentRegister enum value.
 * @return A constant string representing the segment register name.
 */
const char *segreg_type_to_string(SegmentRegister reg_type);


#endif // REGISTERS_H
