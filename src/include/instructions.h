#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdlib.h>

/**
 * @enum InstructionType
 * @brief Enumeration of supported assembly instruction types and directives.
 */
typedef enum {
    // General instructions
    INSTR_GENERIC,   /**< Unknown or generic instruction */
    INSTR_HLT,       /**< Halt the CPU */
    INSTR_INT,       /**< Software interrupt */
    INSTR_MOV,       /**< Move data */
    INSTR_PUSH,      /**< Push onto stack */
    INSTR_POP,       /**< Pop from stack */
    INSTR_LEA,       /**< Load effective address */
    INSTR_ADD,       /**< Add */
    INSTR_SUB,       /**< Subtract */
    INSTR_INC,       /**< Increment */
    INSTR_DEC,       /**< Decrement */
    INSTR_IMUL,      /**< Integer multiply */
    INSTR_IDIV,      /**< Integer divide */
    INSTR_AND,       /**< Bitwise AND */
    INSTR_OR,        /**< Bitwise OR */
    INSTR_XOR,       /**< Bitwise XOR */
    INSTR_NOT,       /**< Bitwise NOT */
    INSTR_NEG,       /**< Negate */
    INSTR_SHL,       /**< Shift left */
    INSTR_SHR,       /**< Shift right */
    INSTR_SAL,       /**< Shift arithmetic left (alias for SHL) */
    INSTR_SAR,       /**< Shift arithmetic right */
    INSTR_JMP,       /**< Unconditional jump */
    INSTR_JE,        /**< Jump if equal */
    INSTR_JNE,       /**< Jump if not equal */
    INSTR_JZ,        /**< Jump if zero */
    INSTR_JNZ,       /**< Jump if not zero */
    INSTR_JG,        /**< Jump if greater (signed) */
    INSTR_JGE,       /**< Jump if greater or equal (signed) */
    INSTR_JL,        /**< Jump if less (signed) */
    INSTR_JLE,       /**< Jump if less or equal (signed) */
    INSTR_JA,        /**< Jump if above (unsigned) */
    INSTR_JAE,       /**< Jump if above or equal (unsigned) */
    INSTR_JB,        /**< Jump if below (unsigned) */
    INSTR_JBE,       /**< Jump if below or equal (unsigned) */
    INSTR_CALL,      /**< Call procedure */
    INSTR_RET,       /**< Return from procedure */
    INSTR_SET,       /**< Set byte on condition */
    INSTR_TEST,      /**< Test bits */
    INSTR_JS,        /**< Jump if sign */
    INSTR_JNS,       /**< Jump if not sign */
    INSTR_CMP,       /**< Compare */
    INSTR_XCHG,      /**< Exchange */
    INSTR_LOCK,      /**< LOCK prefix */
    INSTR_LEAVE,     /**< Leave procedure */
    INSTR_NOP,       /**< No operation */
    INSTR_MOVSX,     /**< Move with sign extend */
    INSTR_MOVZX,     /**< Move with zero extend */
    INSTR_BOUND,     /**< Check array index */
    INSTR_WAIT,      /**< Wait */
    INSTR_LODS,      /**< Load string */
    INSTR_STOS,      /**< Store string */
    INSTR_SCAS,      /**< Scan string */
    INSTR_CMPS,      /**< Compare string */
    INSTR_REP,       /**< Repeat prefix */
    INSTR_REPE,      /**< Repeat while equal */
    INSTR_REPNE,     /**< Repeat while not equal */
    INSTR_SALC,      /**< Set AL on carry */
    INSTR_ARPL,      /**< Adjust RPL */
    INSTR_CLD,       /**< Clear direction flag */
    INSTR_STD,       /**< Set direction flag */
    INSTR_CLC,       /**< Clear carry flag */
    INSTR_STC,       /**< Set carry flag */
    INSTR_CMC,       /**< Complement carry flag */
    INSTR_ESC,       /**< Escape to coprocessor */
    INSTR_IN,        /**< Input from port */
    INSTR_OUT,       /**< Output to port */
    INSTR_INT3,      /**< Breakpoint interrupt */
    INSTR_IRET,      /**< Interrupt return */
    INSTR_SYSCALL,   /**< System call */
    INSTR_SYSRET,    /**< System return */

    // Control & System instructions
    INSTR_LGDT,      /**< Load Global Descriptor Table */
    INSTR_SGDT,      /**< Store Global Descriptor Table */
    INSTR_LIDT,      /**< Load Interrupt Descriptor Table */
    INSTR_SIDT,      /**< Store Interrupt Descriptor Table */
    INSTR_LTR,       /**< Load Task Register */
    INSTR_STR,       /**< Store Task Register */
    INSTR_MOVCR,     /**< Move to/from Control Register (CR0, CR2, CR3, CR4) */
    INSTR_CLTS,      /**< Clear Task Switched Flag */
    INSTR_INVLPG,    /**< Invalidate TLB entry */
    INSTR_VERR,      /**< Verify segment for reading */
    INSTR_VERW,      /**< Verify segment for writing */

    // Assembler directives (tokens, treated separately but useful here)
    DIRECTIVE_ORG,   /**< ORG directive */
    DIRECTIVE_BITS,  /**< BITS directive */
    DIRECTIVE_DB,    /**< Define Byte */
    DIRECTIVE_DW,    /**< Define Word */
    DIRECTIVE_DD,    /**< Define Double Word */
    DIRECTIVE_DQ,    /**< Define Quad Word */
    DIRECTIVE_DT,    /**< Define Ten Bytes */
    DIRECTIVE_EQU,   /**< EQU directive */
    DIRECTIVE_SECTION, /**< SECTION directive */
    DIRECTIVE_EXTERN,  /**< EXTERN directive */
    DIRECTIVE_GLOBAL,  /**< GLOBAL directive */
    DIRECTIVE_ALIGN,   /**< ALIGN directive */

} InstructionType;

/**
 * @struct InstructionEntry
 * @brief Maps instruction mnemonic strings to their corresponding instruction or directive type.
 */
typedef struct {
    const char* mnemonic;       /**< Instruction or directive mnemonic (uppercase) */
    InstructionType type;       /**< Corresponding instruction or directive type */
} InstructionEntry;

/**
 * @brief Instruction and directive lookup table.
 *
 * Contains all supported instruction mnemonics and assembler directives.
 * The last entry is a sentinel with NULL mnemonic.
 */
static const InstructionEntry instruction_table[] = {
    // Instructions
    {"MOV", INSTR_MOV},
    {"HLT", INSTR_HLT},
    {"INT", INSTR_INT},
    {"PUSH", INSTR_PUSH},
    {"POP", INSTR_POP},
    {"LEA", INSTR_LEA},
    {"ADD", INSTR_ADD},
    {"SUB", INSTR_SUB},
    {"INC", INSTR_INC},
    {"DEC", INSTR_DEC},
    {"IMUL", INSTR_IMUL},
    {"IDIV", INSTR_IDIV},
    {"AND", INSTR_AND},
    {"OR", INSTR_OR},
    {"XOR", INSTR_XOR},
    {"NOT", INSTR_NOT},
    {"NEG", INSTR_NEG},
    {"SHL", INSTR_SHL},
    {"SAL", INSTR_SAL},
    {"SHR", INSTR_SHR},
    {"SAR", INSTR_SAR},
    {"JMP", INSTR_JMP},
    {"JE", INSTR_JE},
    {"JNE", INSTR_JNE},
    {"JZ", INSTR_JZ},
    {"JNZ", INSTR_JNZ},
    {"JG", INSTR_JG},
    {"JGE", INSTR_JGE},
    {"JL", INSTR_JL},
    {"JLE", INSTR_JLE},
    {"JA", INSTR_JA},
    {"JAE", INSTR_JAE},
    {"JB", INSTR_JB},
    {"JBE", INSTR_JBE},
    {"CALL", INSTR_CALL},
    {"RET", INSTR_RET},
    {"SET", INSTR_SET},
    {"TEST", INSTR_TEST},
    {"JS", INSTR_JS},
    {"JNS", INSTR_JNS},
    {"CMP", INSTR_CMP},
    {"XCHG", INSTR_XCHG},
    {"LOCK", INSTR_LOCK},
    {"LEAVE", INSTR_LEAVE},
    {"NOP", INSTR_NOP},
    {"MOVSX", INSTR_MOVSX},
    {"MOVZX", INSTR_MOVZX},
    {"BOUND", INSTR_BOUND},
    {"WAIT", INSTR_WAIT},
    {"LODS", INSTR_LODS},
    {"STOS", INSTR_STOS},
    {"SCAS", INSTR_SCAS},
    {"CMPS", INSTR_CMPS},
    {"REP", INSTR_REP},
    {"REPE", INSTR_REPE},
    {"REPNE", INSTR_REPNE},
    {"SALC", INSTR_SALC},
    {"ARPL", INSTR_ARPL},
    {"CLD", INSTR_CLD},
    {"STD", INSTR_STD},
    {"CLC", INSTR_CLC},
    {"STC", INSTR_STC},
    {"CMC", INSTR_CMC},
    {"ESC", INSTR_ESC},
    {"IN", INSTR_IN},
    {"OUT", INSTR_OUT},
    {"INT3", INSTR_INT3},
    {"IRET", INSTR_IRET},
    {"SYSCALL", INSTR_SYSCALL},
    {"SYSRET", INSTR_SYSRET},

    // System control instructions
    {"LGDT", INSTR_LGDT},
    {"SGDT", INSTR_SGDT},
    {"LIDT", INSTR_LIDT},
    {"SIDT", INSTR_SIDT},
    {"LTR", INSTR_LTR},
    {"STR", INSTR_STR},
    {"MOVCR", INSTR_MOVCR},
    {"CLTS", INSTR_CLTS},
    {"INVLPG", INSTR_INVLPG},
    {"VERR", INSTR_VERR},
    {"VERW", INSTR_VERW},

    // Directives
    {"ORG", DIRECTIVE_ORG},
    {"BITS", DIRECTIVE_BITS},
    {"DB", DIRECTIVE_DB},
    {"DW", DIRECTIVE_DW},
    {"DD", DIRECTIVE_DD},
    {"DQ", DIRECTIVE_DQ},
    {"DT", DIRECTIVE_DT},
    {"EQU", DIRECTIVE_EQU},
    {"SECTION", DIRECTIVE_SECTION},
    {"EXTERN", DIRECTIVE_EXTERN},
    {"GLOBAL", DIRECTIVE_GLOBAL},
    {"ALIGN", DIRECTIVE_ALIGN},

    {NULL, INSTR_GENERIC}  /**< Sentinel marking end of table */
};

/**
 * @brief Retrieves the instruction or directive type for a given mnemonic.
 *
 * This function performs a case-insensitive lookup in the instruction table.
 *
 * @param lexeme The instruction or directive mnemonic to lookup (case-insensitive).
 * @return The corresponding InstructionType or INSTR_GENERIC if not found.
 */
InstructionType get_instruction_type(const char* lexeme);

/**
 * @brief Converts an InstructionType enum value to its string representation.
 *
 * This function returns a string literal corresponding to the given
 * InstructionType enumeration constant. The returned string matches the
 * enum constant name, e.g., "INSTR_MOV", "DIRECTIVE_DB", etc.
 *
 * @param type The InstructionType enum value to convert.
 * @return A constant string literal representing the enum name.
 *         Returns "INSTR_UNKNOWN" if the type is not recognized.
 */
const char* instruction_type_to_string(InstructionType type);

#endif // INSTRUCTIONS_H
