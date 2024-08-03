#ifndef LOX_VALUE_H
#define LOX_VALUE_H

#include "shared.h"

typedef double Value;

typedef struct
{
    Value *values;

    size_t capacity;
    size_t count;
} ValueArray;

void init_value_array(ValueArray *array);
void write_value_array(ValueArray *array, Value value);
void free_value_array(ValueArray *array);

void print_value(Value value);

#endif