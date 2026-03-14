#ifndef MICRO_CODE_H
#define MICRO_CODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <wchar.h>

typedef enum {
    OP_HALT    = 0x00,

    OP_ADD     = 0x01,
    OP_SUB     = 0x02,
    OP_MUL     = 0x03,
    OP_DIV     = 0x04,
    OP_MOD     = 0x05,

    OP_PUSH    = 0x06,
    OP_DROP    = 0x07,
    OP_SWAP    = 0x08, // Inverse les deux premiers (ex : [a, b] -> [b, a])
    OP_OVER    = 0x09, // Copie le second élément au sommet (ex : [a, b] -> [a, b, a])
    OP_DUP     = 0x0A, // Duplique la dernière valeur


    OP_JMP     = 0x0B,
    OP_JZ      = 0x0C,
    OP_JNZ     = 0x0D,
    OP_EQ      = 0x0E,
    OP_LT      = 0x0F,
    OP_GT      = 0x10,

    OP_STORE   = 0x11,
    OP_LOAD    = 0x12,

    OP_MOVR    = 0x65,

    OP_DELAY   = 0x13,
    OP_PRINT   = 0x14,
    OP_PUTC    = 0x15,

    OP_INCLUDE = 0x16,
    OP_FN      = 0x17,
    OP_RET     = 0x18,
} OpCode;

typedef struct {
    uint32_t hash;
    uint32_t addr;
} McFunction;

typedef struct {
    uint8_t *flash;
    uint8_t gmode;
    uint16_t pc;
    uint16_t stack_size;
    uint16_t flash_size;
    uint16_t ram_size;
    uint16_t lt_size;       // linked table size
    uint16_t lt_capacity;   // linked table capacity
    int32_t *stack;
    int32_t *ram;
    McFunction *linked_table;
    int32_t sp;
    int32_t regs[4];
} Mc;

int new_mc(Mc *mc, const char *path);
int execute_mc(Mc *mc);
void push_lt(Mc *mc, McFunction value);
void free_mc(Mc *mc);

#endif
