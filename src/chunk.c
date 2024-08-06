#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void init_chunk(Chunk *chunk)
{
    chunk->code = NULL;
    chunk->count = 0;
    chunk->capacity = 0;

    chunk->lines = NULL;
    chunk->line_count = 0;
    chunk->line_capacity = 0;

    init_value_array(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte, Line line)
{
    if (chunk->count + 1 > chunk->capacity)
    {
        size_t old_capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(chunk->capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    if ((chunk->line_count != 0) && (chunk->lines[chunk->line_count - 1].line == line))
    // The previous op-code(s) also belong to the same line, so
    // we just increment it's `spans` field to signify that this
    // byte originates from the same line.
    {
        if (chunk->lines[chunk->line_count - 1].spans < UINT8_MAX)
        {
            chunk->lines[chunk->line_count - 1].spans += 1;
            return;
        }
    }

    // If this line already has 256 op-codes, or if it's a new line,
    // we need to create a new line unit and append it.
    if (chunk->line_count + 1 > chunk->line_capacity)
    {
        size_t old_capacity = chunk->line_capacity;
        chunk->line_capacity = GROW_CAPACITY(chunk->line_capacity);
        chunk->lines = GROW_ARRAY(LineUnit, chunk->lines, old_capacity, chunk->line_capacity);
    }

    LineUnit new_unit;
    new_unit.line = line;
    new_unit.spans = 1;

    chunk->lines[chunk->line_count] = new_unit;
    chunk->line_count++;
}

uint16_t get_line(Chunk *chunk, size_t index)
{
    uint16_t spans_sum = 0;
    size_t line_unit_index = 0;

    while (spans_sum < index + 1)
    {
        spans_sum += (uint16_t)chunk->lines[line_unit_index].spans;
        line_unit_index++;
    }

    return chunk->lines[line_unit_index - 1].line;
}

size_t add_const(Chunk *chunk, Value value)
{
    write_value_array(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void write_const(Chunk *chunk, Value value, Line line)
{
    size_t constant_index = add_const(chunk, value);

    if (constant_index < 256)
    {
        write_chunk(chunk, OP_CONST, line);
        write_chunk(chunk, (uint8_t)(constant_index), line);
    }
    else
    {
        write_chunk(chunk, OP_CONST_LONG, line);
        write_chunk(chunk, (uint8_t)(constant_index & 0xff), line);
        write_chunk(chunk, (uint8_t)((constant_index & 0xff00) >> 8), line);
        write_chunk(chunk, (uint8_t)((constant_index & 0xff0000) >> 16), line);
    }
}

void free_chunk(Chunk *chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(LineUnit, chunk->lines, chunk->line_capacity);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}