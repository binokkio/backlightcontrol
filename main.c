#include <stdio.h>
#include "args.h"

int main(int argc, char **argv)
{
	action *action = parse_args(argc, argv);

	if (action == NULL)
	{
		printf("Help...\n");
	}
	else
	{
		printf(
			"Change: %d, direction: %d, target: %p\n",
			action->change,
			action->direction,
			action->target
		);
	}

	return 0;
}
