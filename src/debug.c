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
    printf("%04ld ", offset);

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

    size_t offset = 0;
    size_t prev_offset = 0;

    for (size_t line_unit_offset = 0; line_unit_offset < chunk->line_count; line_unit_offset++)
    {
        printf("%-4d ", chunk->lines[line_unit_offset].line);
        offset = disassemble_instruction(chunk, offset);

        while (offset < prev_offset + chunk->lines[line_unit_offset].spans)
        {
            printf("     ");
            offset = disassemble_instruction(chunk, offset);
        }

        prev_offset = offset;
    }
}
