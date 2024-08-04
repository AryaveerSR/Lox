#include <stdio.h>

#include "debug.h"

int main(int argc, char const *argv[])
{
	Chunk chunk;

	init_chunk(&chunk);
	write_chunk(&chunk, OP_RETURN, 123);

	size_t index = add_const(&chunk, 12.0);

	write_chunk(&chunk, OP_CONST, 124);
	write_chunk(&chunk, index, 124);
	write_chunk(&chunk, OP_RETURN, 124);

	write_chunk(&chunk, OP_RETURN, 125);
	write_chunk(&chunk, OP_RETURN, 125);
	write_chunk(&chunk, OP_RETURN, 125);
	write_chunk(&chunk, OP_RETURN, 125);
	write_chunk(&chunk, OP_RETURN, 125);
	write_chunk(&chunk, OP_RETURN, 125);

	for (int i = 0; i < chunk.line_count; i++)
	{
		LineUnit unit = chunk.lines[i];
		printf("Line %d has %d spans\n", unit.line, unit.spans);
	}

	printf("Line-> %d\n", get_line(&chunk, 0));
	printf("Line-> %d\n", get_line(&chunk, 3));
	printf("Line-> %d\n", get_line(&chunk, 4));

	disassemble_chunk(&chunk, "BRILLIANT CHUNK OF DATA");

	free_chunk(&chunk);

	return 0;
}
