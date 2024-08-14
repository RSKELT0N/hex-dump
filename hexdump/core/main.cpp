#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "hexdump.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv)
{
    (void)hexdump::utils::shift_input_args(&argc, &argv);

    if(argc < hexdump::LOWER_BOUND_ARGC)
    {
        fprintf(stderr, "Number of cmd arguments is less than the required amount: %d\nPlease provide input file.\n", hexdump::LOWER_BOUND_ARGC);
        return 1;
    }

    int state = setjmp(hexdump::buf);

    if(state != 0)
    {
        fprintf(stderr, "hex-dump has occurred an error; exit status (%d)\n", state);
        exit(state);
    }

    hexdump::hex_input(argc, argv);
    return state;
}
