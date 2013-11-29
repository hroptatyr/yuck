#include <stdio.h>

#include "test.x"

int
main(int argc, char *argv[])
{
	struct yuck_s argi[1];

	if (yuck_parse(argi, argc, argv) < 0) {
		return 1;
	}

	switch (argi->cmd) {
	case test_gen:
		puts("gen command");
		break;
	case test_NONE:
		puts("no command");
		break;
	default:
		printf("%u command\n", argi->cmd);
		break;
	}

	/* go through common options */
	if (argi->help_auto) {
		printf("common help set\n");
	}

	switch (argi->cmd) {
	case test_gen:
		if (argi->gen.extra_arg) {
			printf("gen extra=%s\n", argi->gen.extra_arg);
		}
		if (argi->gen.dashh_flag) {
			printf("gen dashh set\n");
		}
		if (argi->gen.version_flag) {
			printf("gen version set\n");
		}
	};

	for (size_t i = 0U; i < argi->nargs; i++) {
		printf("got `%s'\n", argi->args[i]);
	}

	yuck_free(argi);
	return 0;
}

