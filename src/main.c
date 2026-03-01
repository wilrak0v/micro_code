#include "micro_code.h"

void draw_stack(Mc mc)
{
    printf("\n");
    for (int i = 0 ; i < mc.stack_size ; i++)
    {
        printf("%d ", mc.stack[i]);
    }
    printf("\n");
}

int main()
{
    Mc mc;
    new_mc(&mc, "test.bin");
    while (1)
    {
        if (execute_mc(&mc))
            break;
        //draw_stack(mc);
    }

    free_mc(&mc);
    printf("\n=== Fin du programme ===\n");
    return 0;
}
