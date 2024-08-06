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

static size_t print_long_constant_instruction(Chunk *chunk, size_t offset)
{
    uint32_t constant =
        chunk->code[offset + 1] |
        (chunk->code[offset + 2] << 8) |
        (chunk->code[offset + 3] << 16);

    printf("%-16s %4d (", "OP_CONST_LONG", constant);
    print_value(chunk->constants.values[constant]);
    printf(")\n");

    return offset + 4;
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
    case OP_NEGATE:
        return print_simple_instruction("OP_NEGATE", offset);
    case OP_ADD:
        return print_simple_instruction("OP_ADD", offset);
    case OP_SUBTRACT:
        return print_simple_instruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
        return print_simple_instruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
        return print_simple_instruction("OP_DIVIDE", offset);

    case OP_CONST:
        return print_constant_instruction(chunk, offset);

    case OP_CONST_LONG:
        return print_long_constant_instruction(chunk, offset);

    default:
        printf("Unknown opcode (%d)\n", instruction);
        return offset + 1;
    }
}

/*
 * Pretty prints a chunk and all related op-codes.
 */
void disassemble_chunk(Chunk *chunk, const char *name)
{
    printf("== %s ==\n", name);

    size_t spans_acc = 0;
    size_t offset = 0;

    for (size_t i = 0; i < chunk->line_count; i++)
    {
        printf("%-4d ====\n", chunk->lines[i].line);

        spans_acc += chunk->lines[i].spans;

        while (offset < spans_acc)
        {
            printf("     ");
            offset = disassemble_instruction(chunk, offset);
        }
    }
}
