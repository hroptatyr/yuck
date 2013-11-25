#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <stdbool.h>
#include <string.h>
#include "yuck.h"

static int parse_cmd_long(struct yuck_s *restrict tgt, const char *opt)
{

	return 0;
}

static int parse_none_long(struct yuck_s *restrict tgt, const char *opt)
{

	return 0;
}

static int parse_cmd_short(struct yuck_s *restrict tgt, const char arg)
{

	return 0;
}

static int parse_none_short(struct yuck_s *restrict tgt, const char arg)
{

	return 0;
}

static int parse_cmd(struct yuck_s *restrict tgt, const char *cmd)
{
	if (0) {
		;
	} else if (!strcmp(cmd, "gen")) {
		tgt->cmd = yuck_gen;
	}
	return 0;
}

int yuck_parse(struct yuck_s *restrict tgt, int argc, char *const argv[])
{
	bool dashdash_seen_p = false;
	char **args;
	size_t nargs;

	/* we'll have at most this many args */
	args = calloc(argc, sizeof(*args));
	tgt->cmd = (enum yuck_cmds_e)0U;
	for (int i = 1U; i < argc; i++) {
		switch (*argv[i]) {
		case '-':
			/* could be an option */
			if (!dashdash_seen_p) {
				const char *op = argv[i] + 1U;

				switch (*op) {
				case '-':
					if (*++op == '\0') {
						dashdash_seen_p = true;
						continue;
					}
					/* long opt,
					 * try command specific options first */
					if (yuck_NCMDS && tgt->cmd) {
						parse_cmd_long(tgt, op);
					}
					/* and now common options */
					parse_none_long(tgt, op);
					continue;
				default:
					/* try command specific opts */
					if (yuck_NCMDS && tgt->cmd) {
						parse_cmd_short(tgt, *op);
					}
					/* and common opts now */
					parse_none_short(tgt, *op);

					/* could be glued into one */
					for (; *op; op++) {
						/* try command specific opts */
						;

						/* and common opts now */
						;
					}
					continue;
				}
			}
			/* otherwise fallthrough */
		default:
			if (yuck_NCMDS && !tgt->cmd) {
				parse_cmd(tgt, argv[i]);
			} else {
				args[nargs++] = argv[i];
			}
			break;
		}
	}

	tgt->nargs = nargs;
	tgt->args = args;
	return 0;
}

void yuck_free(struct yuck_s *restrict tgt)
{
	if (tgt->nargs > 0U) {
		/* free despite const qualifier */
		free((char*)NULL + ((const char*)tgt->args - (char*)NULL));
	}
	return;
}

/* beef.c ends here */
