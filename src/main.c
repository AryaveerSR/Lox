#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "vm.h"

char *read_file(const char *path)
{
	FILE *file_ptr = fopen(path, "rb");

	if (file_ptr == NULL)
	{
		fprintf(stderr, "File does not exist. %s\n", path);
		exit(74);
	}

	// Get the file size
	fseek(file_ptr, 0L, SEEK_END);
	size_t file_size = ftell(file_ptr);
	rewind(file_ptr);

	char *buffer = malloc(file_size + 1);

	if (buffer == NULL)
	{
		fprintf(stderr, "Not enough memory to read file. %s\n", path);
		exit(74);
	}

	size_t bytes_read = fread(buffer, sizeof(char), file_size, file_ptr);

	if (bytes_read < file_size)
	{
		fprintf(stderr, "Failed to read file. %s\n", path);
		exit(74);
	}

	buffer[bytes_read] = '\0';

	fclose(file_ptr);
	return buffer;
}

void run_file(VmState *state, const char *path)
{
	char *source = read_file(path);
	InterpretResult result = interpret(state, source);

	free(source);

	if (result == INTERPRET_COMPILE_ERR)
	{
		exit(65);
	}

	if (result == INTERPRET_RUNTIME_ERR)
	{
		exit(70);
	}
}

int main(int argc, char const *argv[])
{
	VmState state;
	init_vm(&state);

	if (argc == 2)
	{
		run_file(&state, argv[1]);
	}

	free_vm(&state);
	return 0;
}
