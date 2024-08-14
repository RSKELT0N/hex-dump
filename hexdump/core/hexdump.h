#pragma once

#include <stdio.h>
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#define min(x,y) (x < y ? x : y)
#define max(x,y) (x < y ? y : x)

namespace hexdump
{
    extern jmp_buf buf;                                // External symbol for long jmp.

    static constexpr uint8_t LOWER_BOUND_ARGC = 1;     // Constant for lower bound input arguments.
    static constexpr uint8_t BYTES_PER_LINE = 16;      // Constant for the amount of bytes per line in stdout.

    void hex_input(int argc, char ** argv);
    void process_hex(const char * input_file, const char * input, size_t input_size);
    void handle_eol(uint32_t current_byte, const char * input);
    void handle_end_characters(uint32_t current_byte, const char * input,  const uint32_t offset);

    namespace utils
    {
        FILE * open_file(const char * file, const char * mode);
        void read_entire_file(FILE *, char **);
        size_t get_file_size(FILE *);
        char * shift_input_args(int * argc, char *** argv);
    } // namespace hexdump::utils
} // namespace hexdump
