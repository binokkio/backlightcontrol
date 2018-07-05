#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char buffer[32];

int read_int_from_file(int fd)
{
	int byte_count = read(fd, buffer, sizeof(buffer) - 1);
	buffer[byte_count] = '\0';
	return atoi(buffer);
}

void write_int_to_file(int fd, int value)
{
	int byte_count = sprintf(buffer, "%d\n", value);
	write(fd, buffer, byte_count);
}
