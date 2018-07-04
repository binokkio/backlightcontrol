typedef struct {
	char *target;
	int change;
	int direction;
} action;

action *parse_args(int argc, char **argv);
