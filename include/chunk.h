#ifndef LOX_CHUNK_H
#define LOX_CHUNK_H

#include "shared.h"
#include "value.h"

typedef enum
{
    OP_RETURN,
    OP_CONST,
    OP_CONST_LONG,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE
} OpCode;

typedef uint16_t Line;

typedef struct
{
    // The line number in source file.
    Line line;
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
void write_chunk(Chunk *chunk, uint8_t byte, Line line);
void free_chunk(Chunk *chunk);

Line get_line(Chunk *chunk, size_t index);

size_t add_const(Chunk *chunk, Value value);
void write_const(Chunk *chunk, Value value, Line line);

#endif