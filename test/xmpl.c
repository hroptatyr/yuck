#include <stdio.h>
#include "xmpl.yucc"

int main(int argc, char *argv[])
{
	struct yuck_s argp[1];
	yuck_parse(argp, argc, argv);

	if (argp->extra_flag) {
		puts("BLING BLING!");
	}

	yuck_free(argp);
	return 0;
}
