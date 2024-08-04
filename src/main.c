#include <stdio.h>

#include "debug.h"

int main(int argc, char const *argv[])
{
	Chunk chunk;

	init_chunk(&chunk);
	write_chunk(&chunk, OP_RETURN, 123);

	size_t index = add_const(&chunk, 12.0);

	// testing op_const

	write_chunk(&chunk, OP_CONST, 124);
	write_chunk(&chunk, index, 124);
	write_chunk(&chunk, OP_RETURN, 124);

	// testing line units

	for (int i = 0; i < chunk.line_count; i++)
	{
		LineUnit unit = chunk.lines[i];
		printf("Line %d spans %d\n", unit.line, unit.spans);
	}

	printf("Line %d\n", get_line(&chunk, 0));
	printf("Line %d\n", get_line(&chunk, 1));

	// testing op_const_long

	for (int i = 0; i < 260; i++)
	{
		add_const(&chunk, 18.0);
	}

	write_const(&chunk, 24.0, 125);
	write_const(&chunk, 30.0, 126);

	disassemble_chunk(&chunk, "BRILLIANT CHUNK OF DATA");

	free_chunk(&chunk);

	return 0;
}
