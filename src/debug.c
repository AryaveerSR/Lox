#include <stdio.h>
#include "debug.h"
#include "value.h"

static size_t print_simple_instruction(const char *name, size_t offset)
{
    printf("%s\n", name);
    return offset + 1;
}

static size_t print_constant_instruction(Chunk *chunk, size_t offset)
{
    uint8_t constant = chunk->code[offset + 1];

    printf("%-16s %4d (", "OP_CONST", constant);
    print_value(chunk->constants.values[constant]);
    printf(")\n");

    return offset + 2;
}

/*
 * Pretty prints a single instruction and all related operands.
 * Returns the number of bytes it actually occupies (incl. the operands).
 */
size_t disassemble_instruction(Chunk *chunk, size_t offset)
{
    printf("%04d ", offset);

    // If the current instruction is from the same line of source
    // as the previous one, do not print the line number, and indent
    // the output to make the grouping clear.
    if ((offset > 0) && (chunk->lines[offset] == chunk->lines[offset - 1]))
    {
        printf("     ");
    }
    else
    {
        printf("%-4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];

    switch (instruction)
    {
    case OP_RETURN:
        return print_simple_instruction("OP_RETURN", offset);

    case OP_CONST:
        return print_constant_instruction(chunk, offset);

    default:
        printf("Unknown opcode (%d)", instruction);
        return offset + 1;
    }
}

/*
 * Pretty prints a chunk and all related op-codes.
 */
void disassemble_chunk(Chunk *chunk, const char *name)
{
    printf("== %s ==\n", name);

    // Loops through the chunk op-code to op-code, by setting
    // the offset to after one instruction's end (next one's start).
    for (size_t offset = 0; offset < chunk->count;)
    {
        offset = disassemble_instruction(chunk, offset);
    }
}
