#include "beef.c"

int
main(int argc, char *argv[])
{
	struct yuck_s argi[1];

	if (yuck_parse(argi, argc, argv) < 0) {
		return 1;
	}

	yuck_free(argi);
	return 0;
}

