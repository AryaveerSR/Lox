#include <stdio.h>

#include "debug.h"
#include "vm.h"

int main(int argc, char const *argv[])
{
	VmState state;
	init_vm(&state);

	Chunk chunk;
	init_chunk(&chunk);

	write_const(&chunk, 12.0, 3);
	write_const(&chunk, 24.0, 3);
	write_chunk(&chunk, OP_ADD, 3);
	write_const(&chunk, 12.0, 3);
	write_chunk(&chunk, OP_DIVIDE, 3);

	write_chunk(&chunk, OP_RETURN, 4);

	interpret(&state, &chunk);

	printf("_____________________________________\n");
	disassemble_chunk(&chunk, "TEST CHUNK");

	free_vm(&state);
	free_chunk(&chunk);

	return 0;
}
