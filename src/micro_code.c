#include "micro_code.h"
#include "opcode.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

long get_file_size(FILE *f)
{
    fseek(f, 0, SEEK_END);
    return ftell(f);
}

void load_into_flash(FILE *f, Mc *mc)
{
    size_t size = get_file_size(f) - 7;
    fseek(f, 7, SEEK_SET);
    if (fread(mc->flash, sizeof(uint8_t), size, f) != size)
    {
        perror("Error when reading the file");
        return;
    }
}

void get_header(FILE *f, uint16_t *stack_size, uint16_t *ram_size, uint8_t *gmode)
{
    char magic_number[2];
    fseek(f, 0, SEEK_SET);
    if (fread(magic_number, sizeof(char[2]), 1, f) != 1)
    {
        perror("Error when getting magic number");
        return;
    }
    if (fread(stack_size, sizeof(uint16_t), 1, f) != 1)
    {
        perror("Error when getting the size of the stack");
        return;
    }
    if (fread(ram_size, sizeof(uint16_t), 1, f) != 1)
    {
        perror("Error when getting the size of RAM");
        return;
    }
    fseek(f, 6, SEEK_SET);
    *gmode = fgetc(f);
}

int new_mc(Mc *mc, const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL)
    {
        perror("Error when opening the file");
        return -1;
    }

    get_header(f, &mc->stack_size, &mc->ram_size, &mc->gmode);

    mc->ram = malloc(mc->ram_size * sizeof(int32_t));

    mc->flash_size = get_file_size(f) - 7;
    mc->flash = malloc(mc->flash_size * sizeof(uint8_t));
    load_into_flash(f, mc);

    mc->stack = malloc(mc->stack_size * sizeof(int32_t));
    mc->pc = 0;
    mc->sp = -1;
    mc->regs[0] = 0;
    mc->regs[1] = 0;
    mc->regs[2] = 0;
    mc->regs[3] = 0;

    printf("New MicroCode VM created!\n");
    printf("Size of stack : %d | Size of RAM : %d | Graphic mode : %d | Size of flash : %d\n\n", mc->stack_size, mc->ram_size, mc->gmode, mc->flash_size);
    printf("Size of the VM : %lu\n", sizeof(*mc));

    if (fclose(f) != 0)
    {
        perror("Error when closing the file");
        return -1;
    }
    return 0;
}

int execute_mc(Mc *mc)
{
    if (mc->pc >= mc->flash_size)
        return 1;

    // Fetch
    uint8_t opcode = mc->flash[mc->pc++];

    // Decode and execute
    switch (opcode)
    {
        case OP_HALT:
            return 1;
            break;

        case OP_ADD:
            execute_add(mc);
            break;
        case OP_SUB:
            execute_sub(mc);
            break;
        case OP_MUL:
            execute_mul(mc);
            break;
        case OP_DIV:
            execute_div(mc);
            break;

        case OP_PUSH:
            execute_push(mc);
            break;
        case OP_DROP:
            execute_drop(mc);
            break;
        case OP_SWAP:
            execute_swap(mc);
            break;
        case OP_OVER:
            execute_over(mc);
            break;
        case OP_DUP:
            execute_dup(mc);
            break;

        case OP_DELAY:
            usleep(1000 * mc->stack[mc->sp]);
            break;

        case OP_JMP:
            execute_jmp(mc);
        	break;
        case OP_JZ:
            execute_jz(mc);
        	break;
        case OP_JNZ:
            execute_jnz(mc);
            break;
        case OP_EQ:
            execute_eq(mc);
            break;
        case OP_LT:
            execute_lt(mc);
            break;
        case OP_GT:
            execute_gt(mc);
            break;

        case OP_STORE:
            execute_store(mc);
            break;
        case OP_MOVR:
            execute_movv(mc);
            break;

        case OP_PRINT:
            execute_print(mc);
            break;
        case OP_PUTC:
            execute_putc(mc);
            break;

        case OP_INCLUDE:
            execute_include(mc);
            break;

        default:
            printf("Error: unknow opcode %d at pos %d\n", opcode, mc->pc);
            return 1;
    }
    return 0;
}

void free_mc(Mc *mc)
{
    free(mc->flash);
    free(mc->stack);
    free(mc->ram);
    mc->ram = NULL;
    mc->flash = NULL;
    mc->stack = NULL;
}
