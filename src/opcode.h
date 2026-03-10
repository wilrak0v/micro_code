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

void execute_print(Mc *mc);
void execute_putc(Mc *mc);

void execute_include(Mc *mc);

void execute_fn(Mc *mc);
void execute_ret(Mc *mc);
void execute_call(Mc *mc);

void execute_movv(Mc *mc);
void execute_movr(Mc *mc);
void execute_movs(Mc *mc);
void execute_storer(Mc *mc);
void execute_loadr(Mc *mc);
void execute_addr(Mc *mc);
void execute_subr(Mc *mc);
void execute_mulr(Mc *mc);
void execute_subr(Mc *mc);
void execute_incr(Mc *mc);
void execute_decr(Mc *mc);

#endif
