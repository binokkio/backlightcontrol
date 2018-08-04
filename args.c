#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mapper.h"
#include "args.h"

enum action_type { set, increment, decrement };

const mapping action_mappings[] = {
	{
		.string = "set",
		.value = set,
	},
	{
		.string = "increment",
		.value = increment,
	},
	{
		.string = "decrement",
		.value = decrement,
	},
	{}
};

action *parse_args(int argc, char **argv)
{
	if (argc < 2) return NULL;

	enum action_type action_type = map(argv[1], action_mappings);
	action *action = calloc(1, sizeof(action));

	// direction
	switch (action_type)
	{
		case set : break;
		case increment : action->direction = 1; break;
		case decrement : action->direction = -1; break;
		default : return NULL;
	}

	// change
	if (argc > 2)
		action->change = atoi(argv[2]);
	else
		action->change = 5;

	// target
	if (argc > 3)
		action->target = argv[3];

	return action;
}
