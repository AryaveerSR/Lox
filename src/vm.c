#include <stdio.h>

#include "vm.h"
#include "debug.h"

static void reset_stack(VmState *state)
{
    state->stack_top = state->stack;
}

void init_vm(VmState *state)
{
    reset_stack(state);
}

void free_vm(VmState *state)
{
}

static void push(VmState *state, Value value)
{
    *state->stack_top = value;
    state->stack_top++;
}

static Value pop(VmState *state)
{
    state->stack_top--;
    return *state->stack_top;
}

#ifdef DEBUG_TRACE_EXECUTION
#define DEBUG_ADD_TRACE                                                                  \
    do                                                                                   \
    {                                                                                    \
        printf("STK  ");                                                                 \
                                                                                         \
        for (Value *slot = state->stack; slot < state->stack_top; slot++)                \
        {                                                                                \
            printf("[");                                                                 \
            print_value(*slot);                                                          \
            printf("]");                                                                 \
        }                                                                                \
                                                                                         \
        printf("\n");                                                                    \
                                                                                         \
        disassemble_instruction(state->chunk, (size_t)(state->ip - state->chunk->code)); \
    } while (0)
#else
#define DEBUG_ADD_TRACE
#endif

#define READ_BYTE(state) (*state->ip++)
#define READ_CONST(state) (state->chunk->constants.values[READ_BYTE(state)])
#define BINARY_OP(op)          \
    do                         \
    {                          \
        double b = pop(state); \
        double a = pop(state); \
                               \
        push(state, a op b);   \
    } while (0)

static InterpretResult run(VmState *state)
{
    while (true)
    {
        DEBUG_ADD_TRACE;

        uint8_t instruction;

        switch (instruction = READ_BYTE(state))
        {
        case OP_RETURN:
            print_value(pop(state));
            printf("\n");
            return INTERPRET_OK;

        case OP_CONST:
        {
            Value constant = READ_CONST(state);
            push(state, constant);
            break;
        }

        case OP_NEGATE:
            push(state, -pop(state));
            break;

        case OP_ADD:
            BINARY_OP(+);
            break;

        case OP_SUBTRACT:
            BINARY_OP(-);
            break;

        case OP_MULTIPLY:
            BINARY_OP(*);
            break;

        case OP_DIVIDE:
            BINARY_OP(/);
            break;

        default:
            break;
        }
    }
}

#undef DEBUG_ADD_TRACE
#undef READ_BYTE
#undef READ_CONST
#undef BINARY_OP

InterpretResult interpret(VmState *state, Chunk *chunk)
{
    state->chunk = chunk;
    state->ip = state->chunk->code;

    return run(state);
}