#include <stdio.h>
#include <string.h>
#include "map.h"

int map(const char *string, const mapping *mappings)
{
	for (; mappings->string != NULL; mappings++)
	{
		if (strcmp(string, mappings->string) == 0)
		{
			return mappings->value;
		}
	}

	return -1;
}
