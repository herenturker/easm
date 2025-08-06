#include <string.h>
#include <ctype.h>
#include "include/instructions.h"

/**
 * @brief Case-insensitive string compare.
 *
 * @param s1 First string.
 * @param s2 Second string.
 * @return 0 if equal (case-insensitive), non-zero otherwise.
 */
static int strcasecmp_custom(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2))
        {
            return (int)((unsigned char)tolower(*s1) - (unsigned char)tolower(*s2));
        }
        s1++;
        s2++;
    }
    return (int)((unsigned char)tolower(*s1) - (unsigned char)tolower(*s2));
}

/**
 * @brief Get the instruction or directive type from mnemonic.
 *
 * @param lexeme The mnemonic string to lookup.
 * @return InstructionType for the mnemonic or INSTR_GENERIC if not found.
 */
InstructionType get_instruction_type(const char *lexeme)
{
    if (lexeme == NULL)
    {
        return INSTR_GENERIC;
    }

    for (int i = 0; instruction_table[i].mnemonic != NULL; i++)
    {
        if (strcasecmp_custom(instruction_table[i].mnemonic, lexeme) == 0)
        {
            return instruction_table[i].type;
        }
    }

    return INSTR_GENERIC;
}

/**
 * @brief Returns the string representation of an InstructionType.
 *
 * @param type The InstructionType to convert.
 * @return The corresponding string literal.
 */
const char *instruction_type_to_string(InstructionType type)
{
    switch (type)
    {
    // General instructions
    case INSTR_GENERIC:
        return "INSTR_GENERIC";
    case INSTR_HLT:
        return "INSTR_HLT";
    case INSTR_INT:
        return "INSTR_INT";
    case INSTR_MOV:
        return "INSTR_MOV";
    case INSTR_PUSH:
        return "INSTR_PUSH";
    case INSTR_POP:
        return "INSTR_POP";
    case INSTR_LEA:
        return "INSTR_LEA";
    case INSTR_ADD:
        return "INSTR_ADD";
    case INSTR_SUB:
        return "INSTR_SUB";
    case INSTR_INC:
        return "INSTR_INC";
    case INSTR_DEC:
        return "INSTR_DEC";
    case INSTR_IMUL:
        return "INSTR_IMUL";
    case INSTR_IDIV:
        return "INSTR_IDIV";
    case INSTR_AND:
        return "INSTR_AND";
    case INSTR_OR:
        return "INSTR_OR";
    case INSTR_XOR:
        return "INSTR_XOR";
    case INSTR_NOT:
        return "INSTR_NOT";
    case INSTR_NEG:
        return "INSTR_NEG";
    case INSTR_SHL:
        return "INSTR_SHL";
    case INSTR_SHR:
        return "INSTR_SHR";
    case INSTR_SAL:
        return "INSTR_SAL";
    case INSTR_SAR:
        return "INSTR_SAR";
    case INSTR_JMP:
        return "INSTR_JMP";
    case INSTR_JE:
        return "INSTR_JE";
    case INSTR_JNE:
        return "INSTR_JNE";
    case INSTR_JZ:
        return "INSTR_JZ";
    case INSTR_JNZ:
        return "INSTR_JNZ";
    case INSTR_JG:
        return "INSTR_JG";
    case INSTR_JGE:
        return "INSTR_JGE";
    case INSTR_JL:
        return "INSTR_JL";
    case INSTR_JLE:
        return "INSTR_JLE";
    case INSTR_JA:
        return "INSTR_JA";
    case INSTR_JAE:
        return "INSTR_JAE";
    case INSTR_JB:
        return "INSTR_JB";
    case INSTR_JBE:
        return "INSTR_JBE";
    case INSTR_CALL:
        return "INSTR_CALL";
    case INSTR_RET:
        return "INSTR_RET";
    case INSTR_SET:
        return "INSTR_SET";
    case INSTR_TEST:
        return "INSTR_TEST";
    case INSTR_JS:
        return "INSTR_JS";
    case INSTR_JNS:
        return "INSTR_JNS";
    case INSTR_CMP:
        return "INSTR_CMP";
    case INSTR_XCHG:
        return "INSTR_XCHG";
    case INSTR_LOCK:
        return "INSTR_LOCK";
    case INSTR_LEAVE:
        return "INSTR_LEAVE";
    case INSTR_NOP:
        return "INSTR_NOP";
    case INSTR_MOVSX:
        return "INSTR_MOVSX";
    case INSTR_MOVZX:
        return "INSTR_MOVZX";
    case INSTR_BOUND:
        return "INSTR_BOUND";
    case INSTR_WAIT:
        return "INSTR_WAIT";
    case INSTR_LODS:
        return "INSTR_LODS";
    case INSTR_STOS:
        return "INSTR_STOS";
    case INSTR_SCAS:
        return "INSTR_SCAS";
    case INSTR_CMPS:
        return "INSTR_CMPS";
    case INSTR_REP:
        return "INSTR_REP";
    case INSTR_REPE:
        return "INSTR_REPE";
    case INSTR_REPNE:
        return "INSTR_REPNE";
    case INSTR_SALC:
        return "INSTR_SALC";
    case INSTR_ARPL:
        return "INSTR_ARPL";
    case INSTR_CLD:
        return "INSTR_CLD";
    case INSTR_STD:
        return "INSTR_STD";
    case INSTR_CLC:
        return "INSTR_CLC";
    case INSTR_STC:
        return "INSTR_STC";
    case INSTR_CMC:
        return "INSTR_CMC";
    case INSTR_ESC:
        return "INSTR_ESC";
    case INSTR_IN:
        return "INSTR_IN";
    case INSTR_OUT:
        return "INSTR_OUT";
    case INSTR_INT3:
        return "INSTR_INT3";
    case INSTR_IRET:
        return "INSTR_IRET";
    case INSTR_SYSCALL:
        return "INSTR_SYSCALL";
    case INSTR_SYSRET:
        return "INSTR_SYSRET";
    case INSTR_CLI:
        return "INSTR_CLI";
    case INSTR_LOOP:
        return "INSTR_LOOP";
    case INSTR_LODSB:
        return "INSTR_LODSB";
    case INSTR_LODSW:
        return "INSTR_LODSW";
    case INSTR_LODSD:
        return "INSTR_LODSD";
    case INSTR_LODSQ:
        return "INSTR_LODSQ";

    // Control & system instructions
    case INSTR_LGDT:
        return "INSTR_LGDT";
    case INSTR_SGDT:
        return "INSTR_SGDT";
    case INSTR_LIDT:
        return "INSTR_LIDT";
    case INSTR_SIDT:
        return "INSTR_SIDT";
    case INSTR_LTR:
        return "INSTR_LTR";
    case INSTR_STR:
        return "INSTR_STR";
    case INSTR_MOVCR:
        return "INSTR_MOVCR";
    case INSTR_CLTS:
        return "INSTR_CLTS";
    case INSTR_INVLPG:
        return "INSTR_INVLPG";
    case INSTR_VERR:
        return "INSTR_VERR";
    case INSTR_VERW:
        return "INSTR_VERW";

    // Directives
    case DIRECTIVE_ORG:
        return "DIRECTIVE_ORG";
    case DIRECTIVE_BITS:
        return "DIRECTIVE_BITS";
    case DIRECTIVE_DB:
        return "DIRECTIVE_DB";
    case DIRECTIVE_DW:
        return "DIRECTIVE_DW";
    case DIRECTIVE_DD:
        return "DIRECTIVE_DD";
    case DIRECTIVE_DQ:
        return "DIRECTIVE_DQ";
    case DIRECTIVE_DT:
        return "DIRECTIVE_DT";
    case DIRECTIVE_EQU:
        return "DIRECTIVE_EQU";
    case DIRECTIVE_SECTION:
        return "DIRECTIVE_SECTION";
    case DIRECTIVE_EXTERN:
        return "DIRECTIVE_EXTERN";
    case DIRECTIVE_GLOBAL:
        return "DIRECTIVE_GLOBAL";
    case DIRECTIVE_ALIGN:
        return "DIRECTIVE_ALIGN";
    case DIRECTIVE_TIMES:
        return "DIRECTIVE_TIMES";

    default:
        return "INSTR_UNKNOWN";
    }
}
