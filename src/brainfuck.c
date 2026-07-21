/// Brainfuck interpreter written in C
// See https://brainfuck.net/learn

#include "native.h"
#include "defs.h"

// Increment and decrement macros
// Use this because the brainfuck requires wrapping around the byte (255 + 1 == 0, 0 - 1 == 255)
#define BF_INC(x) ((unsigned char)((unsigned char)(x) + 1))
#define BF_DEC(x) ((unsigned char)((unsigned char)(x) - 1))

#define BF_INCP(p) ((p + 1) % BF_TAPE_SIZE)
#define BF_DECP(p) ((p - 1 + BF_TAPE_SIZE) % BF_TAPE_SIZE)

#ifdef __cplusplus
extern "C" {
#endif

void bf_main(char* file) {
    unsigned char tape[BF_TAPE_SIZE]; // memory tape
    unsigned int ptr = 0; // pointer

    if (file == BF_NULL)
        bf_exit(BF_EXIT_NOARG); // no argument

    fd_t fd = bf_open(file);
    size_t size;
    if (fd == BF_STAT_BAD_FD)
        goto badfile;
    if ((size = bf_fsize()) == 0)
        goto badfile_with_fd;

    unsigned char *buf = (unsigned char*)bf_mmap(size, fd);
    if (buf == BF_STAT_BAD_MMAP)
        goto badfile_with_fd;
    bf_close(fd);

    // Eval
    for (size_t i = 0; i < size; i++) {
        unsigned char c = buf[i];
        switch (c) {
            case '>': ptr = BF_INCP(ptr); break; // Cycling
            case '<': ptr = BF_DECP(ptr); break; // Cycling
            case '+': tape[ptr] = BF_INC(tape[ptr]); break;
            case '-': tape[ptr] = BF_DEC(tape[ptr]); break;
            case '.': if (bf_write(&tape[ptr]) != 1) goto bad_io; break;
            case ',': int c = bf_read(); if (c == -1) goto bad_io; tape[ptr] = (unsigned char)c; break;
            case '[': if (tape[ptr] == 0) {
                size_t depth = 1;
                while (depth > 0) {
                    i++;
                    if (buf[i] == '[') depth++;
                    if (buf[i] == ']') depth--;
                }
            }
            case ']': if (tape[ptr] != 0) {
                size_t depth = 1;
                while (depth > 0) {
                    i--;
                    if (buf[i] == ']') depth++;
                    if (buf[i] == '[') depth--;
                }
            }
        }
    }

    bf_munmap(buf);

bad_io:
    bf_munmap(buf);
    bf_exit(BF_EXIT_BADIO);

badfile_with_fd:
    bf_close(fd);
badfile:
    bf_exit(BF_EXIT_BADFILE);
}

#ifdef __cplusplus
}
#endif
