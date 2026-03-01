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

void get_header(FILE *f, uint16_t *flash_size, uint16_t *ram_size, uint8_t *gmode)
{
    char magic_number[2];
    fseek(f, 0, SEEK_SET);
    if (fread(magic_number, sizeof(char[2]), 1, f) != 1)
    {
        perror("Error when getting magic number");
        return;
    }
    if (fread(flash_size, sizeof(uint16_t), 1, f) != 1)
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

    mc->flash_size = get_file_size(f) - 3;
    mc->flash = malloc(mc->flash_size * sizeof(uint8_t));
    load_into_flash(f, mc);

    mc->stack = malloc(mc->stack_size * sizeof(int32_t));
    mc->pc = 0;
    mc->sp = -1;

    printf("New MicroCode VM created!\n");
    printf("Size of stack : %d | Size of RAM : %d | Graphic mode : %d | Size of flash : %d\n\n", mc->stack_size, mc->ram_size, mc->gmode, mc->flash_size);

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
            mc->stack[mc->sp-1] = mc->stack[mc->sp-1] - mc->stack[mc->sp];
            mc->stack[mc->sp] = 0;
            mc->sp--;
            break;
        case OP_MUL:
            mc->stack[mc->sp-1] = mc->stack[mc->sp-1] * mc->stack[mc->sp];
            mc->stack[mc->sp] = 0;
            mc->sp--;
            break;
        case OP_DIV:
            mc->stack[mc->sp-1] = mc->stack[mc->sp-1] / mc->stack[mc->sp];
            mc->stack[mc->sp] = 0;
            mc->sp--;
            break;

        case OP_PUSH:
            mc->stack[++mc->sp] = *(int32_t*)(&mc->flash[mc->pc]);
            mc->pc += 4;
            break;
        case OP_DROP:
            mc->stack[mc->sp] = 0;
            mc->sp--;
            break;
        case OP_SWAP:
        {
            int32_t temp = mc->stack[mc->sp];
            mc->stack[mc->sp] = mc->stack[mc->sp-1];
            mc->stack[mc->sp-1] = temp;
            break;
        }
        case OP_OVER:
            mc->stack[mc->sp + 1] = mc->stack[mc->sp - 1];
            mc->sp++;
            break;
        case OP_DUP:
            mc->stack[mc->sp + 1] = mc->stack[mc->sp];
            mc->sp++;
            break;

        case OP_DELAY:
            usleep(1000 * mc->stack[mc->sp]);
            break;

        case OP_JMP:
        {
            int32_t target = *(int32_t*)(&mc->flash[mc->pc]);
            mc->pc = target; 
        	break;
        }       
        case OP_JZ:
        {
            int32_t target = *(int32_t*)(&mc->flash[mc->pc]);
            mc->pc += 4; 
            if (mc->stack[mc->sp] == 0) {
            	mc->pc = target;
            }
            mc->stack[mc->sp--] = 0; 
        	break;
        }
        case OP_JNZ:
        {
            int32_t target = *(int32_t*)(&mc->flash[mc->pc]);
            mc->pc += 4; 
            if (mc->stack[mc->sp] > 0) {
                printf("DEBUG: Jump to %d (Value was %d)\n", target, mc->stack[mc->sp]);
            	mc->pc = target;
            }
            mc->stack[mc->sp--] = 0;
        	break;
        }
        case OP_EQ:
            mc->stack[mc->sp - 1] = mc->stack[mc->sp - 1] == mc->stack[mc->sp];
            mc->stack[mc->sp--] = 0;
            break;
        case OP_LT:
            mc->stack[mc->sp - 1] = mc->stack[mc->sp - 1] > mc->stack[mc->sp];
            mc->stack[mc->sp--] = 0;
            break;
        case OP_GT:
            mc->stack[mc->sp - 1] = mc->stack[mc->sp - 1] < mc->stack[mc->sp];
            mc->stack[mc->sp--] = 0;
            break;

        case OP_STORE:
            execute_store(mc);
            break;
        case OP_LOAD:
            execute_load(mc);
            break;

        case OP_PRINT:
            execute_print(mc);
            break;
        case OP_PUTC:
            execute_putc(mc);
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
