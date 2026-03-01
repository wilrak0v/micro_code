#include "opcode.h"

void execute_add(Mc *mc)
{
    mc->stack[mc->sp-1] = mc->stack[mc->sp-1] + mc->stack[mc->sp];
    mc->stack[mc->sp] = 0;
    mc->sp--;
}

void execute_sub(Mc *mc)
{
    mc->stack[mc->sp-1] = mc->stack[mc->sp-1] - mc->stack[mc->sp];
    mc->stack[mc->sp] = 0;
    mc->sp--;
}


void execute_store(Mc *mc)
{
    int32_t addr = *(int32_t*)(&mc->flash[mc->pc]);
    mc->pc += 4;
    if (mc->ram_size < addr || 0 > addr) return;
    mc->ram[addr] = mc->stack[mc->sp];
    mc->stack[mc->sp--] = 0;
}

void execute_load(Mc *mc)
{
    int32_t addr = *(int32_t*)(&mc->flash[mc->pc]);
    mc->pc += 4;
    if (mc->ram_size < addr || 0 > addr) return;
    mc->stack[++mc->sp] = mc->ram[addr];
}


void execute_print(Mc *mc)
{
    printf("%d", mc->stack[mc->sp]);
}

void execute_putc(Mc *mc)
{
    putchar((char)mc->stack[mc->sp]);
}
