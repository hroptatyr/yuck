#include <stdio.h>

#include "yuck.x"

int
main(int argc, char *argv[])
{
	struct yuck_s argi[1];

	if (yuck_parse(argi, argc, argv) < 0) {
		return 1;
	}

	switch (argi->cmd) {
	case yuck_gen:
		puts("gen command");
		break;
	case yuck_NONE:
		puts("no command");
		break;
	default:
		puts("cannot understand command");
		break;
	}

	for (size_t i = 0U; i < argi->nargs; i++) {
		printf("got `%s'\n", argi->args[i]);
	}

	yuck_free(argi);
	return 0;
}

