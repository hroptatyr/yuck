#include <stdio.h>
#include "xmpl-command.yucc"

int main(int argc, char *argv[])
{
	yuck_t argp[1U];
	yuck_parse(argp, argc, argv);

	if (argp->extra_flag) {
		puts("BLING BLING!");
	}

	switch (argp->cmd) {
	case XMPL_CMD_NONE:
	default:
		puts("no command :(");
		break;
	}

	yuck_free(argp);
	return 0;
}
