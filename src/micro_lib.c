#include "micro_lib.h"
#include "micro_code.h"
#include <stdint.h>
#include <stdio.h>

void get_lib(FILE *f, Mc *mc)
{
    char magic_number[2];
    fseek(f, 0, SEEK_SET);
    if (fread(magic_number, sizeof(magic_number), 1, f) != 1)
    {
        perror("Error when getting ml magic number");
        return;
    }

    size_t start_pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size_t lib_size = ftell(f) - start_pos; // Plus rapide que la boucle getc
    fseek(f, start_pos, SEEK_SET);

    size_t new_total_size = lib_size + mc->flash_size;

    uint8_t *temp = realloc(mc->flash, new_total_size * sizeof(uint8_t));
    if (temp == NULL) {
        perror("Failed to realloc flash");
        return;
    }

    mc->flash = temp;

    fread(mc->flash + mc->flash_size, 1, lib_size, f);
    mc->flash_size = new_total_size;
}
