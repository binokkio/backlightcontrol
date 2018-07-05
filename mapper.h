typedef struct {
	char *string;
	int value;
} mapping;

int map(const char *string, const mapping *mappings);
