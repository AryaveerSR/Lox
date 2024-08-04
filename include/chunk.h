#ifndef LOX_CHUNK_H
#define LOX_CHUNK_H

#include "shared.h"
#include "value.h"

typedef enum
{
    OP_RETURN,
    OP_CONST,
} OpCode;

typedef struct
{
    // The line number in source file.
    uint16_t line;
    // The number of consecutive bytes in the chunk that originate
    // from the same line.
    uint8_t spans;
} LineUnit;

typedef struct
{
    uint8_t *code;
    size_t count;
    size_t capacity;

    LineUnit *lines;
    size_t line_count;
    size_t line_capacity;

    ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, size_t line);
uint16_t get_line(Chunk *chunk, size_t index);
size_t add_const(Chunk *chunk, Value value);
void free_chunk(Chunk *chunk);

#endif