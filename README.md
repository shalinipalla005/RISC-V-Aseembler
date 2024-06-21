# RISC_V Assembler

This RISC-V assembler program takes RISC-V assembly code as input in a `.asm` file and generates machine code as output in a `.mc` file.This code is in C++.

## Input Format

The input `.asm` file should contain one RISC-V assembly instruction per line.provided the inputFile <input.asm> Here's an example:

add x1, x2, x3

andi x5, x6, 10

## Output Format

The output `.mc` file will have the following format:

<address of instruction> <machine code of the instruction>

Example:

0x0 0x003100B3

0x4 0x00A37293

Data Segment looks like 

0x100000000 0x10

## Addressing Scheme

- Code Segment: 0x0000 0000
- Data Segment: 0x1000 0000
- Heap: 0x1000 8000
- Stack Segment: 0x7FFF FFDC

## Supported Instructions

### R Format
- add, and, or, sll, slt, sra, srl, sub, xor, mul, div, rem

### I Format
- addi, andi, ori, lb, ld, lh, lw, jalr

### S Format
- sb, sw, sd, sh

### SB Format
- beq, bne, bge, blt

### U Format
- auipc, lui

### UJ Format
- jal

## Assembler Directives

- `.text`: Indicates the start of the text segment.
- `.data`: Indicates the start of the data segment.
- `.byte`, `.half`, `.word`, `.dword`: Directives to allocate memory and initialize it with byte, half, word, or double-word values.
- `.asciiz`: Directive to allocate memory and initialize it with a null-terminated ASCII string.

## Notes

- Pseudo-instructions are not supported.
- Floating-point operations are not supported.
