#include <stdio.h>
#include YUCC_FILE

int main(int argc, char *argv[])
{
	yuck_t argp[1U];
	yuck_parse(argp, argc, argv);
	for (size_t i = 0U; i < argp->nargs; i++) {
		puts(argp->args[i]);
	}
	yuck_free(argp);
	return 0;
}
