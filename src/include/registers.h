#ifndef REGISTERS_H
#define REGISTERS_H

typedef enum
{
    REG8_NONE = 0,
    REG8_AL,
    REG8_BL,
    REG8_CL,
    REG8_DL,
    REG8_AH,
    REG8_BH,
    REG8_CH,
    REG8_DH
} Register8;

typedef enum
{
    REG16_NONE = 0,
    REG16_AX,
    REG16_BX,
    REG16_CX,
    REG16_DX,
    REG16_SI,
    REG16_DI,
    REG16_BP,
    REG16_SP
} Register16;

typedef enum
{
    REG32_NONE = 0,
    REG32_EAX,
    REG32_EBX,
    REG32_ECX,
    REG32_EDX,
    REG32_ESI,
    REG32_EDI,
    REG32_EBP,
    REG32_ESP
} Register32;

typedef enum
{
    SEGREG_NONE = 0,
    SEGREG_CS,
    SEGREG_DS,
    SEGREG_SS,
    SEGREG_ES,
    SEGREG_FS,
    SEGREG_GS
} SegmentRegister;

const char *get_register8_name(Register8 reg);
Register8 get_register8_by_name(const char *name);
const char *get_register16_name(Register16 reg);
Register16 get_register16_by_name(const char *name);
const char *get_register32_name(Register32 reg);
Register32 get_register32_by_name(const char *name);
const char *get_segment_register_name(SegmentRegister reg);
SegmentRegister get_segment_register_by_name(const char *name);

#endif
