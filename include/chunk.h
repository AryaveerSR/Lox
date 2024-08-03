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
    uint8_t *code;
    size_t count;
    size_t capacity;

    size_t *lines;

    ValueArray constants;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, size_t line);
size_t add_const(Chunk *chunk, Value value);
void free_chunk(Chunk *chunk);

#endif