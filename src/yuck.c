/*** yuck.c -- generate umbrella commands
 *
 * Copyright (C) 2013 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@ga-group.nl>
 *
 * This file is part of yuck.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***/
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>

#if !defined LIKELY
# define LIKELY(_x)	__builtin_expect((_x), 1)
#endif	/* !LIKELY */
#if !defined UNLIKELY
# define UNLIKELY(_x)	__builtin_expect((_x), 0)
#endif	/* UNLIKELY */

#if !defined countof
# define countof(x)	(sizeof(x) / sizeof(*x))
#endif	/* !countof */

#define _paste(x, y)	x ## y
#define paste(x, y)	_paste(x, y)
#if !defined with
# define with(args...)							\
	for (args, *paste(__ep, __LINE__) = (void*)1;			\
	     paste(__ep, __LINE__); paste(__ep, __LINE__)= 0)
#endif	/* !with */


static void
__attribute__((format(printf, 1, 2)))
error(const char *fmt, ...)
{
	va_list vap;
	va_start(vap, fmt);
	vfprintf(stderr, fmt, vap);
	va_end(vap);
	if (errno) {
		fputc(':', stderr);
		fputc(' ', stderr);
		fputs(strerror(errno), stderr);
	}
	fputc('\n', stderr);
	return;
}

static __inline void*
deconst(const void *cp)
{
	union {
		const void *c;
		void *p;
	} tmp = {cp};
	return tmp.p;
}

static FILE*
get_fn(int argc, char *argv[])
{
	FILE *res;

	if (argc > 1) {
		const char *fn = argv[1];
		if (UNLIKELY((res = fopen(fn, "r")) == NULL)) {
			error("cannot open file `%s'", fn);
		}
	} else {
		res = stdin;
	}
	return res;
}

static char*
streqp(const char *str, const char *cmp, size_t cmpz)
{
	if (!strncasecmp(str, cmp, cmpz)) {
		return deconst(str + cmpz);
	}
	return NULL;
}

static int
snarf(char *line, size_t llen)
{
	static char cur_umb[64U];
	static char cur_cmd[64U];
	static char cur_opt[64U];
	char *lp;

#define STREQLITP(x, lit)	(streqp((x), lit, sizeof(lit) - 1))

	/* first keep looking for Usage: lines */
	if (UNLIKELY((lp = STREQLITP(line, "usage:")) != NULL)) {
		/* new umbrella, or new command */
		char *sp;
		char *ep;
		const char *const eolp = line + llen;

		/* skip whitespace */
		for (sp = lp; sp < eolp && isspace(*sp); sp++);
		/* find end-of-command */
		for (ep = sp; ep < eolp && !isspace(*ep); ep++);

		*ep = '\0';
		memcpy(cur_umb, sp, ep - sp + 1U);
		printf("set_umb(\"%s\")\n", cur_umb);
	}

#undef STREQLITP
	return 0;
}


int
main(int argc, char *argv[])
{
	int rc = 0;
	FILE *yf;

	if (UNLIKELY((yf = get_fn(argc, argv)) == NULL)) {
		rc = 1;
		goto out;
	}

	{
		char *line = NULL;
		size_t llen = 0U;
		ssize_t nrd;

		while ((nrd = getline(&line, &llen, yf)) > 0) {
			if (*line == '#') {
				continue;
			}
			snarf(line, nrd);
		}

		free(line);
	}

	/* clean up */
	fclose(yf);
out:
	return rc;
}

/* yuck.c ends here */
