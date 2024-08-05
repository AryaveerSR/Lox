#ifndef LOX_VM_H
#define LOX_VM_H

#include "chunk.h"

#define STACK_MAX 256

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERR,
    INTERPRET_RUNTIME_ERR,
} InterpretResult;

typedef struct
{
    Chunk *chunk;
    // Instruction Pointer
    uint8_t *ip;

    Value stack[STACK_MAX];
    Value *stack_top;
} VmState;

void init_vm(VmState *state);
void free_vm(VmState *state);

InterpretResult interpret(VmState *state, Chunk *code);

#endif