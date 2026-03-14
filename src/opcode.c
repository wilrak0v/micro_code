#include "opcode.h"
#include "micro_code.h"
#include "micro_lib.h"

/* Arithmetic Functions */
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

void execute_mul(Mc *mc)
{
    mc->stack[mc->sp-1] = mc->stack[mc->sp-1] * mc->stack[mc->sp];
    mc->stack[mc->sp] = 0;
    mc->sp--;
}

void execute_div(Mc *mc)
{
    mc->stack[mc->sp-1] = mc->stack[mc->sp-1] / mc->stack[mc->sp];
    mc->stack[mc->sp] = 0;
    mc->sp--;
}


/* Stack Functions */
void execute_push(Mc *mc)
{
    mc->stack[++mc->sp] = *(int32_t*)(&mc->flash[mc->pc]);
    mc->pc += 4;
}

void execute_drop(Mc *mc)
{
    mc->stack[mc->sp] = 0;
    mc->sp--;
}

void execute_swap(Mc *mc)
{
    int32_t temp = mc->stack[mc->sp];
    mc->stack[mc->sp] = mc->stack[mc->sp-1];
    mc->stack[mc->sp-1] = temp;

}

void execute_over(Mc *mc)
{
    mc->stack[mc->sp + 1] = mc->stack[mc->sp - 1];
    mc->sp++;
}

void execute_dup(Mc *mc)
{
    mc->stack[mc->sp + 1] = mc->stack[mc->sp];
    mc->sp++;
}





/* Logic and Jump Functions */
void execute_jmp(Mc *mc)
{
    int32_t target = *(int32_t*)(&mc->flash[mc->pc]);
    mc->pc = target;
}

void execute_jz(Mc *mc)
{
    int32_t target = *(int32_t*)(&mc->flash[mc->pc]);
    mc->pc += 4;
    if (mc->stack[mc->sp] == 0) {
   	    mc->pc = target;
    }
    mc->stack[mc->sp--] = 0;
}

void execute_jnz(Mc *mc)
{
    int32_t target = *(int32_t*)(&mc->flash[mc->pc]);
    mc->pc += 4;
    if (mc->stack[mc->sp] > 0) {
        printf("DEBUG: Jump to %d (Value was %d)\n", target, mc->stack[mc->sp]);
        mc->pc = target;
    }
    mc->stack[mc->sp--] = 0;
}

void execute_eq(Mc *mc)
{
    mc->stack[mc->sp - 1] = mc->stack[mc->sp - 1] == mc->stack[mc->sp];
    mc->stack[mc->sp--] = 0;
}

void execute_lt(Mc *mc)
{
    mc->stack[mc->sp - 1] = mc->stack[mc->sp - 1] > mc->stack[mc->sp];
    mc->stack[mc->sp--] = 0;
}

void execute_gt(Mc *mc)
{
    mc->stack[mc->sp - 1] = mc->stack[mc->sp - 1] < mc->stack[mc->sp];
    mc->stack[mc->sp--] = 0;
}


/* RAM Functions */
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


/* Registers Functions */
void execute_movv(Mc *mc)
{
    // regs[arg] = value
    int8_t register_number = mc->flash[mc->pc++];
    int32_t value = *(int32_t*)(&mc->flash[mc->pc]);
    mc->pc += 4;
    if (register_number == 4) mc->sp = value;
    else if (register_number == 5) mc-> sp = value;
    else if (register_number < 4) mc->regs[register_number] = value;
    else return;
    printf("Register number : %d | Value : %d\n", register_number, value);
    printf("REGS : %d | %d | %d | %d\n", mc->regs[0], mc->regs[1], mc->regs[2], mc->regs[3]);
}


/* Display and Events Functions */
void execute_print(Mc *mc)
{
    printf("%d", mc->stack[mc->sp]);
}

void execute_putc(Mc *mc)
{
    putchar((char)mc->stack[mc->sp]);
}


/* Include Function */
void execute_include(Mc *mc)
{
    // 1. Get the filename
    char fn[16];
    int i = 0;
    while (mc->flash[mc->pc] != 0x00)
        fn[i++] = mc->flash[mc->pc++];
    fn[i] = '\0';

    FILE *fp = fopen(fn, "rb");
    if (fp == NULL)
    {
        char buf[25 + 16];
        snprintf(buf, sizeof(buf), "Error when opening lib '%s'", fn);
        perror(buf);
        return;
    }

    get_lib(fp, mc);

    if(fclose(fp) != 0)
    {
        char buf[25 + 16];
        snprintf(buf, sizeof(buf), "Error when closing lib '%s'", fn);
        perror(buf);
        return;
    }
}


/* Functions functions? hilarant */
void execute_fn(Mc *mc)
{
    int32_t hash = *(int32_t*)(&mc->flash[mc->pc]);
    mc->pc += 4;
    printf("(McFunction) hash: %d ; addr: %d\n", hash, mc->pc);
    push_lt(mc, (McFunction){hash, mc->pc});
    while(mc->flash[mc->pc] != OP_RET);
    mc->pc++;
}

void execute_ret(Mc *mc)
{
    int8_t register_number = mc->flash[mc->pc++];
    if (register_number == 4) mc->pc = mc->sp;
    else if (register_number < 4) mc->pc = mc->regs[register_number];
    else return;
}
