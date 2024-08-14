#include "hexdump.h"

#include <unistd.h>

namespace hexdump
{
    namespace utils
    {
        char * shift_input_args(int * argc, char *** argv)
        {
            if(*argc == 0)
            {
                fprintf(stderr, "%s\n", "Cannot shift arguments; no arguments to shift\n");
                return {};
            }

            return --(*argc), *(*argv)++;
        }

        FILE * open_file(const char * file, const char * mode)
        {
            FILE * ptr = {0};
            if((ptr = ::fopen(file, mode)) == NULL)
            {
                fprintf(stderr, "Unable to get create a file descriptor of file: %s and mode: %s. Error: %s\n", file, mode, strerror(errno));
                longjmp(buf, 1);
            }
            return ptr;
        }

        void read_entire_file(FILE * fd, char ** input)
        {
            size_t bytes_read = 0;
            size_t input_size = get_file_size(fd);

            fseek(fd, 0L, SEEK_SET);

            while(input_size - bytes_read > 0)
                bytes_read += fread((*input)+bytes_read, sizeof(char), (input_size - bytes_read), fd);
        }

        size_t get_file_size(FILE * fd)
        {
            fseek(fd, 0L, SEEK_END);
            size_t size = ftell(fd);
            fseek(fd, 0L, SEEK_SET);

            return size;
        }
    }

    // Linked symbol to long jmp.
    jmp_buf buf = {0};

    void handle_end_characters(uint32_t current_byte, const char * input,  const uint32_t offset = 0)
    {
        printf("|");
        for(size_t i = offset; i < hexdump::BYTES_PER_LINE; i++)
        {
            char c = input[current_byte - (hexdump::BYTES_PER_LINE - i)];

            if(c >= 32) // Lower bound of printable ascii characters.
                printf("%c", c);
            else
                printf(".");

        }
        printf("|");
    }

    void handle_eol(uint32_t current_byte, const char * input)
    {
        if(current_byte >= hexdump::BYTES_PER_LINE)
            handle_end_characters(current_byte, input);

        printf("\n%#08x: ", current_byte);
    }

    void process_hex(const char * input_file, const char * input, size_t input_size)
    {
        uint32_t current_byte;
        uint32_t overflow = (input_size % hexdump::BYTES_PER_LINE);
        uint32_t remaining = (overflow == 0) ? 0 : hexdump::BYTES_PER_LINE - overflow;

        printf("File: %s\n", input_file);
        printf("Bytes: %zu\n", input_size);

        for(current_byte = 0; current_byte < input_size; current_byte++)
        {
            if(current_byte % hexdump::BYTES_PER_LINE == 0)
                handle_eol(current_byte, input);

            printf("%02x ", static_cast<uint8_t>(input[current_byte]));
        }

        if(remaining > 0)
            printf("%*c", remaining * 3, ' ');

        handle_end_characters(current_byte, input, remaining);
        printf("\n");
    }

    void hex_input([[maybe_unused]] int argc, [[maybe_unused]] char ** argv)
    {
        char * input_file = utils::shift_input_args(&argc, &argv);
        FILE * input_fd = utils::open_file(input_file, "rb");

        size_t input_size = utils::get_file_size(input_fd);
        char * input_arr = new char[input_size];

        utils::read_entire_file(input_fd, &input_arr);
        process_hex(input_file, input_arr, input_size);

        delete[] input_arr;
    }
}
