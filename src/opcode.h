#ifndef OPCODE_H
#define OPCODE_H

#include "micro_code.h"
#include "micro_lib.h"

void execute_add(Mc *mc);
void execute_sub(Mc *mc);
void execute_mul(Mc *mc);
void execute_div(Mc *mc);

void execute_push(Mc *mc);
void execute_drop(Mc *mc);
void execute_swap(Mc *mc);
void execute_over(Mc *mc);
void execute_dup(Mc *mc);


void execute_delay(Mc *mc);

void execute_jmp(Mc *mc);
void execute_jz(Mc *mc);
void execute_jnz(Mc *mc);
void execute_eq(Mc *mc);
void execute_lt(Mc *mc);
void execute_gt(Mc *mc);

void execute_store(Mc *mc);
void execute_load(Mc *mc);

void execute_movv(Mc *mc);      // Mov value

void execute_print(Mc *mc);
void execute_putc(Mc *mc);

void execute_include(Mc *mc);

void execute_fn(Mc *mc);

#endif
