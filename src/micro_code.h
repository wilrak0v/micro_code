#ifndef MICRO_CODE_H
#define MICRO_CODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

typedef enum {
    OP_HALT  = 0x00,

    OP_ADD   = 0x01,
    OP_SUB   = 0x02,
    OP_MUL   = 0x03,
    OP_DIV   = 0x04,

    OP_PUSH  = 0x10,
    OP_DROP  = 0x11,
    OP_SWAP  = 0x12, // Inverse les deux premiers (ex : [a, b] -> [b, a])
    OP_OVER  = 0x13, // Copie le second élément au sommet (ex : [a, b] -> [a, b, a])
    OP_DUP   = 0x14, // Duplique la dernière valeur
    
    OP_DELAY = 0x20, // Mets en pause le programme x ms
    
    OP_JMP   = 0x30,
    OP_JZ    = 0x31,
    OP_JNZ   = 0x32,
    OP_EQ    = 0x33,
    OP_LT    = 0x34,
    OP_GT    = 0x35,

    OP_STORE = 0x40,
    OP_LOAD  = 0x41,

    OP_PRINT = 0x50,
    OP_PUTC  = 0x51,
} OpCode;

typedef struct {
    uint8_t *flash;
    uint8_t gmode;
    uint16_t pc;
    uint16_t stack_size;
    uint16_t flash_size;
    uint16_t ram_size;
    int32_t *stack;
    int32_t *ram;
    int32_t sp;
} Mc;

int new_mc(Mc *mc, const char *path);
int execute_mc(Mc *mc);
void free_mc(Mc *mc);

#endif
