#include <stdio.h>
#define yuck_post_help		yuck_post_help
#define yuck_post_version	yuck_post_version
#define yuck_post_usage		yuck_post_usage
#include YUCC_FILE

static void yuck_post_help()
{
	puts("SOME POST HELP TEXT");
	return;
}

static void yuck_post_version()
{
	puts("(C) 2014 written by yuck test-suite");
	return;
}

static void yuck_post_usage()
{
	puts("Options are as follows");
	return;
}

int main(int argc, char *argv[])
{
	yuck_t argp[1U];
	yuck_parse(argp, argc, argv);
	yuck_free(argp);
	return 0;
}
