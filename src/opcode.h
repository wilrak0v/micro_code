#ifndef OPCODE_H
#define OPCODE_H

#include "micro_code.h"

void execute_add(Mc *mc);
void execute_sub(Mc *mc);

void execute_store(Mc *mc);
void execute_load(Mc *mc);

void execute_print(Mc *mc);
void execute_putc(Mc *mc);

#endif
