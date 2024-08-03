#include <stdio.h>

#include "value.h"
#include "memory.h"

void init_value_array(ValueArray *array)
{
    array->capacity = 0;
    array->count = 0;
    array->values = NULL;
}

void write_value_array(ValueArray *array, Value value)
{
    if (array->count + 1 > array->capacity)
    {
        size_t old_capacity = array->capacity;
        array->capacity = GROW_CAPACITY(old_capacity);
        array->values = GROW_ARRAY(Value, array->values, old_capacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void free_value_array(ValueArray *array)
{
    FREE_ARRAY(Value, array->values, array->count);
    init_value_array(array);
}

void print_value(Value value)
{
    printf("%g", value);
}