# EASM — Eren's Educational 2-Pass Assembler

**EASM** is an educational project that demonstrates the inner workings of a simple **two-pass assembler**.  
It’s designed to help students and hobbyists learn about assembler structure, symbol resolution, and code generation using the GNU toolchain.

---

## 📘 Overview

An assembler translates human-readable assembly language into machine code.  
A *two-pass assembler* first scans the source to collect symbols and labels, then generates final machine code in the second pass.

EASM is **not intended as a production assembler**, but as an educational resource for understanding:

- How instruction parsing and tokenization work
- How symbol tables and relocations are handled
- How code is emitted after symbol resolution

---

## 🧠 Features

- Two-pass parsing and symbol table management
- Basic instruction and directive recognition
- Minimal, clear code aimed at readability and learning
- Written in C/C++ using the GNU toolchain (`gcc`, `g++`)
- Cross-platform build system using `make`
- Compatible with Windows (MSYS2 / MinGW) and Linux

---

## ⚙️ Building the Project

Clone the repository and build using GNU Make:

```bash
git clone https://github.com/herenturker/easm.git
cd easm
make
```

This command will compile all source files and produce the executable:

```bash
easm.exe    # On Windows
./easm      # On Linux
```

## Requirements

You will need:

- GCC (C and C++ compilers)
- GNU Make
- Optionally MSYS2 or MinGW if you are on Windows

To verify installation:
```bash
gcc --version
g++ --version
make --version
```


By default, EASM reads the input assembly file and performs two passes:

1. Pass 1: Scans for labels, directives, and builds the symbol table
2. Pass 2: Resolves symbols and emits the final binary output

Output files can then be examined using a hex viewer or loaded into a simple virtual machine for testing.

## Runtime Dependencies

When compiled with GCC, the generated binary may depend on these runtime libraries:

| DLL                | Source                   | License                 | AGPL Compatibility 
|--------------------|--------------------------|-------------------------|-----|
| libgcc_s_seh-1.dll | GCC runtime              | GPL + linking exception | Yes |
| libstdc++-6.dll    | GNU C++ standard library | GPL + linking exception | Yes |
| KERNEL32.dll       | Windows API              | System component        | Yes |
| msvcrt.dll         | Microsoft C runtime      | System component        | Yes |

These are standard system or compiler-provided libraries.
They are not distributed with this repository and are automatically available on typical systems.

## Usage
After compiling codes, you can run:
```bash
./easm examples/hello.asm
```

Thank you for reading.




