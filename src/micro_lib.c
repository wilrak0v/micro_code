#include "micro_lib.h"

void get_lib(FILE *f, Mc *mc)
{
    char magic_number[2];
    fseek(f, 0, SEEK_SET);
    if (fread(magic_number, sizeof(magic_number), 1, f) != 1)
    {
        perror("Error when getting ml magic number");
        return;
    }

    uint32_t current_fn_addr;
    int content_size = 0;
    while (fread(&current_fn_addr, sizeof(current_fn_addr), 1, f) == 1)
    {
        if (current_fn_addr == 0xFFFFFFFF)
        {
            printf("That's the end at pos %d\n", mc->pc);
            break;
        }
        content_size++;
        // READ the addr and add it into linked table
        printf("Fn '%d' at addr '%d'\n", content_size, current_fn_addr);
    }
}
