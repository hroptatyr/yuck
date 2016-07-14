#include <stdio.h>
#include "xmpl-posarg-with-angles.yucc"

int main(int argc, char *argv[])
{
	yuck_t argp[1U];
	yuck_parse(argp, argc, argv);

	switch (argp->cmd) {
	case XMPL_CMD_NONE:
	default:
		puts("no command :(");
		break;
	}

	yuck_free(argp);
	return 0;
}
