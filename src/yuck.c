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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#if !defined LIKELY
# define LIKELY(_x)	__builtin_expect((_x), 1)
#endif	/* !LIKELY */
#if !defined UNLIKELY
# define UNLIKELY(_x)	__builtin_expect((_x), 0)
#endif	/* UNLIKELY */
#if !defined UNUSED
# define UNUSED(_x)	_x __attribute__((unused))
#endif	/* !UNUSED */

#if !defined countof
# define countof(x)	(sizeof(x) / sizeof(*x))
#endif	/* !countof */

#define _paste(x, y)	x ## y
#define paste(x, y)	_paste(x, y)
#if !defined with
# define with(args...)							\
	for (args, *paste(__ep, __LINE__) = (void*)1;			\
	     paste(__ep, __LINE__); paste(__ep, __LINE__) = 0)
#endif	/* !with */

struct usg_s {
	char *umb;
	char *cmd;
	char *parg;
	char *desc;
};

struct opt_s {
	char sopt;
	char *lopt;
	char *larg;
	char *desc;
};


static __attribute__((format(printf, 1, 2))) void
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

static inline __attribute__((unused)) void*
deconst(const void *cp)
{
	union {
		const void *c;
		void *p;
	} tmp = {cp};
	return tmp.p;
}

static inline __attribute__((always_inline)) unsigned int
fls(unsigned int x)
{
	return x ? sizeof(x) * 8U - __builtin_clz(x) : 0U;
}

static inline __attribute__((always_inline)) size_t
max_zu(size_t x, size_t y)
{
	return x > y ? x : y;
}

static size_t
xstrncpy(char *restrict dst, const char *src, size_t ssz)
{
	memcpy(dst, src, ssz);
	dst[ssz] = '\0';
	return ssz;
}

static __attribute__((unused)) size_t
xstrlcpy(char *restrict dst, const char *src, size_t dsz)
{
	size_t ssz = strlen(src);
	if (ssz > dsz) {
		ssz = dsz - 1U;
	}
	memcpy(dst, src, ssz);
	dst[ssz] = '\0';
	return ssz;
}

static __attribute__((unused)) size_t
xstrlncpy(char *restrict dst, size_t dsz, const char *src, size_t ssz)
{
	if (ssz > dsz) {
		ssz = dsz - 1U;
	}
	memcpy(dst, src, ssz);
	dst[ssz] = '\0';
	return ssz;
}

static __attribute__((unused)) bool
xstreqp(const char *s1, const char *s2)
{
	if (s1 == NULL && s2 == NULL) {
		return true;
	} else if (s1 == NULL || s2 == NULL) {
		/* one of them isn't NULL */
		return false;
	}
	/* resort to normal strcmp */
	return !strcasecmp(s1, s2);
}

static bool
only_whitespace_p(const char *line, size_t llen)
{
	for (const char *lp = line, *const ep = line + llen; lp < ep; lp++) {
		if (!isspace(*lp)) {
			return false;
		}
	}
	return true;
}


/* bang buffers */
typedef struct {
	/* the actual buffer (resizable) */
	char *s;
	/* current size */
	size_t z;
}  bbuf_t;

static char*
bbuf_cpy(bbuf_t b[static 1U], const char *str, size_t ssz)
{
	size_t nu = max_zu(fls(ssz + 1U) + 1U, 6U);
	size_t ol = b->z ? max_zu(fls(b->z) + 1U, 6U) : 0U;

	if (UNLIKELY(nu > ol)) {
		b->s = realloc(b->s, (1U << nu) * sizeof(*b->s));
	}
	xstrncpy(b->s, str, ssz);
	b->z += ssz;
	return b->s;
}

static char*
bbuf_cat(bbuf_t b[static 1U], const char *str, size_t ssz)
{
	size_t nu = max_zu(fls(b->z + ssz + 1U) + 1U, 6U);
	size_t ol = b->z ? max_zu(fls(b->z) + 1U, 6U) : 0U;

	if (UNLIKELY(nu > ol)) {
		b->s = realloc(b->s, (1U << nu) * sizeof(*b->s));
	}
	xstrncpy(b->s + b->z, str, ssz);
	b->z += ssz;
	return b->s;
}


static void yield_usg(const struct usg_s *arg);
static void yield_opt(const struct opt_s *arg);
static void yield_inter(bbuf_t x[static 1U]);

#define DEBUG(args...)

static int
usagep(const char *line, size_t llen)
{
#define STREQLITP(x, lit)      (!strncasecmp((x), lit, sizeof(lit) - 1))
	static struct usg_s cur_usg;
	static bbuf_t umb[1U];
	static bbuf_t cmd[1U];
	static bbuf_t parg[1U];
	static bbuf_t desc[1U];
	static bool cur_usg_ylddp;
	const char *sp;
	const char *up;
	const char *cp;
	const char *const ep = line + llen;

	if (UNLIKELY(line == NULL)) {
		goto yield;
	}

	DEBUG("USAGEP CALLED with %s", line);

	if (!STREQLITP(line, "usage:")) {
		if (only_whitespace_p(line, llen) && !desc->z) {
			return 1;
		} else if (!isspace(*line) && !cur_usg_ylddp) {
			/* append to description */
			cur_usg.desc = bbuf_cat(desc, line, llen);
			return 1;
		}
	yield:
		if (!cur_usg_ylddp) {
			yield_usg(&cur_usg);
			/* reset */
			memset(&cur_usg, 0, sizeof(cur_usg));
			desc->z = 0U;
			cur_usg_ylddp = true;
		}
		return 0;
	}
	/* overread whitespace then */
	for (sp = line + sizeof("usage:") - 1; sp < ep && isspace(*sp); sp++);
	/* first thing should name the umbrella, find its end */
	for (up = sp; sp < ep && !isspace(*sp); sp++);

	if (cur_usg.umb && !strncasecmp(cur_usg.umb, up, sp - up)) {
		/* nothing new and fresh */
		;
	} else {
		cur_usg.umb = bbuf_cpy(umb, up, sp - up);
	}

	/* overread more whitespace and [--BLA] decls then */
	do {
		for (; sp < ep && isspace(*sp); sp++);
		/* we might be strafed with option decls here */
		if (*sp != '[') {
			break;
		}
		for (sp++; sp < ep && *sp++ != ']';);
		for (sp++; sp < ep && *sp == '.'; sp++);
	} while (1);

	/* now it's time for the command innit */
	for (cp = sp; sp < ep && !isspace(*sp); sp++);

	if (cur_usg.cmd && !strncasecmp(cur_usg.cmd, cp, sp - cp)) {
		/* nothing new and fresh */
		;
	} else if ((*cp != '<' || cp[--sp - cp++] == '>') &&
		   !strncasecmp(cp, "command", sp - cp)) {
		/* special command COMMAND or <command> */
		cur_usg.cmd = NULL;
	} else {
		cur_usg.cmd = bbuf_cpy(cmd, cp, sp - cp);
	}

	/* now there might be positional args, snarf them */
	with (const char *pp) {
		for (; sp < ep && isspace(*sp); sp++);
		for (pp = sp; sp < ep && !isspace(*sp); sp++);
		cur_usg.parg = bbuf_cpy(parg, pp, sp - pp);
	}

	cur_usg_ylddp = false;
	return 1;
}

static int
optionp(const char *line, size_t llen)
{
	static struct opt_s cur_opt;
	static bbuf_t desc[1U];
	static bbuf_t lopt[1U];
	static bbuf_t larg[1U];
	const char *sp = line;
	const char *const ep = line + llen;

	if (UNLIKELY(line == NULL)) {
		goto yield;
	}

	DEBUG("OPTIONP CALLED with %s", line);

	/* overread whitespace */
	for (; sp < ep && isspace(*sp); sp++);
	if (sp - line >= 8) {
		/* we dont expect option specs that far out */
		goto desc;
	}

yield:
	/* must yield the old current option before it's too late */
	if (cur_opt.sopt || cur_opt.lopt) {
		yield_opt(&cur_opt);
	}
	/* complete reset */
	memset(&cur_opt, 0, sizeof(cur_opt));
	if (sp - line < 2) {
		/* can't be an option, can it? */
		return 0;
	}

	/* no yield pressure anymore, try parsing the line */
	sp++;
	if (*sp >= '0') {
		char sopt = *sp++;

		/* eat a comma as well */
		if (*sp == ',') {
			sp++;
		}
		if (!isspace(*sp)) {
			/* dont know -x.SOMETHING? */
			return 0;
		}
		/* start over with the new option */
		sp++;
		cur_opt.sopt = sopt;
		if (isspace(*sp)) {
			/* no arg name, no longopt */
		} else if (*sp == '-') {
			/* must be a --long now, maybe */
			sp++;
		} else {
			/* just an arg name */
			const char *ap;
			for (ap = sp; sp < ep && !isspace(*sp); sp++);
			cur_opt.larg = bbuf_cpy(larg, ap, sp - ap);
		}
	} else if (*sp == '-') {
		/* --option */
		;
	} else {
		/* dont know what this is */
		return 0;
	}

	/* --option */
	if (*sp++ == '-') {
		const char *op;

		for (op = sp; sp < ep && !isspace(*sp) && *sp != '='; sp++);
		cur_opt.lopt = bbuf_cpy(lopt, op, sp - op);

		if (*sp++ == '=') {
			/* has got an arg */
			const char *ap;
			for (ap = sp; sp < ep && !isspace(*sp); sp++);
			cur_opt.larg = bbuf_cpy(larg, ap, sp - ap);
		}
	}
	/* require at least one more space? */
	;
	/* space eater */
	for (; sp < ep && isspace(*sp); sp++);
	/* dont free but reset the old guy */
	desc->z = 0U;
desc:
	with (size_t sz = llen - (sp - line)) {
		cur_opt.desc = bbuf_cat(desc, sp, sz);
	}
	return 1;
}

static int
interp(const char *line, size_t llen)
{
	static bbuf_t desc[1U];
	bool only_ws_p = only_whitespace_p(line, llen);

	if (UNLIKELY(line == NULL)) {
		goto yield;
	}

	DEBUG("INTERP CALLED with %s", line);
	if (only_ws_p && desc->z) {
	yield:
		yield_inter(desc);
		/* reset */
		desc->z = 0U;
	} else if (!only_ws_p) {
		/* snarf the line */
		bbuf_cat(desc, line, llen);
		return 1;
	}
	return 0;
}


static const char *UNUSED(curr_umb);
static const char *curr_cmd;
static const char nul_str[] = "";
static FILE *outf;

static void
yield_help(void)
{
	const char *cmd = curr_cmd ?: nul_str;

	fprintf(outf, "yuck_add_option([h], [help], [auto], [%s])\n", cmd);
	fprintf(outf, "yuck_set_option_desc([h], [help], [%s], [\
display this help and exit])\n", cmd);
	return;
}

static void
yield_version(void)
{
	const char *cmd = curr_cmd ?: nul_str;

	fprintf(outf, "yuck_add_option([V], [version], [auto], [%s])\n", cmd);
	fprintf(outf, "yuck_set_option_desc([V], [version], [%s], [\
output version information and exit])\n", cmd);
	return;
}

static void
yield_usg(const struct usg_s *arg)
{
	if (arg->desc != NULL) {
		/* kick last newline */
		size_t z = strlen(arg->desc);
		if (arg->desc[z - 1U] == '\n') {
			arg->desc[z - 1U] = '\0';
		}
	}
	if (arg->cmd != NULL) {
		curr_cmd = arg->cmd;
		fprintf(outf, "\nyuck_add_command([%s])\n", arg->cmd);
		if (arg->desc != NULL) {
			fprintf(outf, "yuck_set_command_desc([%s], [%s])\n",
				arg->cmd, arg->desc);
		}
	} else if (arg->umb != NULL) {
		curr_umb = arg->umb;
		fprintf(outf, "\nyuck_set_umbrella([%s])\n", arg->umb);
		if (arg->desc != NULL) {
			fprintf(outf, "yuck_set_umbrella_desc([%s], [%s])\n",
				arg->umb, arg->desc);
		}
		/* insert auto-help and auto-version */
		yield_help();
		yield_version();
	}
	return;
}

static void
yield_opt(const struct opt_s *arg)
{
	char sopt[2U] = {arg->sopt, '\0'};
	const char *opt = arg->lopt ?: nul_str;
	const char *cmd = curr_cmd ?: nul_str;

	if (arg->larg == NULL) {
		fprintf(outf, "yuck_add_option([%s], [%s], [flag], [%s]);\n",
			sopt, opt, cmd);
	} else {
		fprintf(outf, "yuck_add_option([%s], [%s], [arg, %s], [%s]);\n",
			sopt, opt, arg->larg, cmd);
	}
	if (arg->desc != NULL) {
		/* kick last newline */
		size_t z = strlen(arg->desc);
		if (arg->desc[z - 1U] == '\n') {
			arg->desc[z - 1U] = '\0';
		}
		fprintf(outf, "yuck_set_option_desc([%s], [%s], [%s], [%s])\n",
			sopt, opt, cmd, arg->desc);
	}
	return;
}

static void
yield_inter(bbuf_t x[static 1U])
{
	if (x->z) {
		if (x->s[x->z - 1U] == '\n') {
			x->s[x->z - 1U] = '\0';
		}
		fprintf(outf, "yuck_add_inter([%s])\n", x->s);
	}
	return;
}


static enum {
	UNKNOWN,
	SET_INTER,
	SET_UMBCMD,
	SET_OPTION,
}
snarf_ln(char *line, size_t llen)
{
	static unsigned int st;

	switch (st) {
	case UNKNOWN:
	case SET_UMBCMD:
	usage:
		/* first keep looking for Usage: lines */
		if (usagep(line, llen)) {
			st = SET_UMBCMD;
			break;
		} else if (st == SET_UMBCMD) {
			/* reset state, go on with option parsing */
			st = UNKNOWN;
			goto option;
		}
	case SET_OPTION:
	option:
		/* check them option things */
		if (optionp(line, llen)) {
			st = SET_OPTION;
			break;
		} else if (st == SET_OPTION) {
			/* reset state, go on with usage parsing */
			st = UNKNOWN;
			goto usage;
		}
	case SET_INTER:
		/* check for some intro texts */
		if (interp(line, llen)) {
			st = SET_INTER;
			break;
		} else {
			/* reset state, go on with option parsing */
			st = UNKNOWN;
		}
	default:
		break;
	}
	return UNKNOWN;
}

static int
snarf_f(FILE *f)
{
	char *line = NULL;
	size_t llen = 0U;
	ssize_t nrd;

	while ((nrd = getline(&line, &llen, f)) > 0) {
		if (*line == '#') {
			continue;
		}
		snarf_ln(line, nrd);
	}
	/* drain */
	snarf_ln(NULL, 0U);

	free(line);
	return 0;
}


#if defined BOOTSTRAP
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

int
main(int argc, char *argv[])
{
	int rc = 0;
	FILE *yf;

	if (UNLIKELY((yf = get_fn(argc, argv)) == NULL)) {
		rc = -1;
	} else {
		/* always use stdout */
		outf = stdout;

		fputs("\
changequote([,])dnl\n\
divert([-1])\n", outf);

		/* let the snarfing begin */
		rc = snarf_f(yf);

		fputs("\n\
changecom([//])\n\
divert[]dnl\n", outf);

		/* clean up */
		fclose(yf);
	}

	return -rc;
}
#endif	/* BOOTSTRAP */


#if !defined BOOTSTRAP
#if !defined PATH_MAX
# define PATH_MAX	(256U)
#endif	/* !PATH_MAX */
static char dslfn[PATH_MAX];
static char gencfn[PATH_MAX];
static char genhfn[PATH_MAX];

static bool
aux_in_path_p(const char *aux, const char *path, size_t pathz)
{
	char fn[PATH_MAX];
	char *restrict fp = fn;
	struct stat st[1U];

	fp += xstrlncpy(fn, sizeof(fn), path, pathz);
	*fp++ = '/';
	xstrlcpy(fp, aux, sizeof(fn) - (fp - fn));

	if (stat(fn, st) < 0) {
		return false;
	}
	return S_ISREG(st->st_mode);
}

static ssize_t
get_myself(char *restrict buf, size_t bsz)
{
	ssize_t off;
	char *mp;

	if ((off = readlink("/proc/self/exe", buf, bsz)) < 0) {
		return -1;
	}
	/* go back to the dir bit */
	for (mp = buf + off - 1U; mp > buf && *mp != '/'; mp--);
	/* should be bin/, go up one level */
	*mp = '\0';
	for (; mp > buf && *mp != '/'; mp--);
	/* check if we're right */
	if (UNLIKELY(strcmp(++mp, "bin"))) {
		/* oh, it's somewhere but not bin/? */
		return -1;
	}
	/* now just use share/yuck/ */
	xstrlcpy(mp, "share/yuck/", bsz - (mp - buf));
	mp += sizeof("share/yuck");
	return mp - buf;
}

static int
find_aux(char *restrict buf, size_t bsz, const char *aux)
{
	/* look up path relative to binary position */
	static char pkgdatadir[PATH_MAX];
	static ssize_t pkgdatalen;
	static const char *tmplpath;
	static ssize_t tmplplen;
	const char *path;
	size_t plen;

	/* start off by snarfing the environment */
	if (tmplplen == 0U) {
		if ((tmplpath = getenv("YUCK_TEMPLATE_PATH")) != NULL) {
			tmplplen = strlen(tmplpath);
		} else {
			/* just set it to something non-0 to indicate initting
			 * and that also works with the loop below */
			tmplplen = -1;
			tmplpath = (void*)0x1U;
		}
	}

	/* snarf pkgdatadir */
	if (pkgdatalen == 0U) {
		pkgdatalen = get_myself(pkgdatadir, sizeof(pkgdatadir));
	}

	/* go through the path first */
	for (const char *pp = tmplpath, *ep, *const end = tmplpath + tmplplen;
	     pp < end; pp = ep + 1U) {
		ep = strchr(pp, ':') ?: end;
		if (aux_in_path_p(aux, pp, ep - pp)) {
			path = pp;
			plen = ep - pp;
			goto bang;
		}
	}
	/* no luck with the env path then aye */
	if (pkgdatalen > 0 && aux_in_path_p(aux, pkgdatadir, pkgdatalen)) {
		path = pkgdatadir;
		plen = pkgdatalen;
		goto bang;
	}
#if defined YUCK_TEMPLATE_PATH
	path = YUCK_TEMPLATE_PATH;
	plen = sizeof(YUCK_TEMPLATE_PATH);
	if (plen-- > 0U && aux_in_path_p(aux, path, plen)) {
		goto bang;
	}
#endif	/* YUCK_TEMPLATE_PATH */
	/* not what we wanted at all, must be christmas */
	return -1;

bang:
	with (size_t z) {
		z = xstrlncpy(buf, bsz, path, plen);
		buf[z++] = '/';
		xstrlcpy(buf + z, aux, bsz - z);
	}
	return 0;
}

static int
find_auxs(void)
{
	int rc = 0;

	rc += find_aux(dslfn, sizeof(dslfn), "yuck.m4");
	rc += find_aux(gencfn, sizeof(gencfn), "yuck-coru.m4c");
	rc += find_aux(genhfn, sizeof(genhfn), "yuck-coru.m4h");
	return rc;
}

static __attribute__((noinline)) int
run_m4(const char *outfn, ...)
{
	static char *m4_cmdline[6U] = {
		"m4", dslfn,
	};
	va_list vap;
	pid_t m4p;

	switch ((m4p = vfork())) {
	case -1:
		/* i am an error */
		error("vfork for m4 failed");
		return -1;

	default:;
		/* i am the parent */
		int rc;
		int st;

		while (waitpid(m4p, &st, 0) != m4p);
		if (WIFEXITED(st)) {
			rc = WEXITSTATUS(st);
		}
		return rc;

	case 0:;
		/* i am the child */
		break;
	}

	/* child code here */
	va_start(vap, outfn);
	for (size_t i = 2U;
	     i < countof(m4_cmdline) &&
		     (m4_cmdline[i] = va_arg(vap, char*)) != NULL; i++);
	va_end(vap);

	if (outfn != NULL) {
		/* --output given */
		const int outfl = O_RDWR | O_CREAT | O_TRUNC;
		int outfd;

		if ((outfd = open(outfn, outfl, 0666)) < 0) {
			/* bollocks */
			error("cannot open outfile `%s'", outfn);
			goto bollocks;
		}

		/* really redir now */
		dup2(outfd, STDOUT_FILENO);
	}

	execvp("m4", m4_cmdline);
	error("execvp(m4) failed");
bollocks:
	_exit(EXIT_FAILURE);
}
#endif	/* !BOOTSTRAP */


#if !defined BOOTSTRAP
#include "yuck.yucc"

static int
cmd_gen(struct yuck_s argi[static 1U])
{
	static const char deffn[] = "yuck.m4i";
	int rc = 0;

	/* deal with the output first */
	if (UNLIKELY((outf = fopen(deffn, "w")) == NULL)) {
		error("cannot open intermediate file `%s'", deffn);
		return -1;
	}

	fputs("\
changequote([,])dnl\n\
divert([-1])\n", outf);

	if (argi->nargs == 0U) {
		if (snarf_f(stdin) < 0) {
			error("gen command failed on stdin");
			rc = 1;
		}
	}
	for (unsigned int i = 0U; i < argi->nargs && rc == 0; i++) {
		const char *fn = argi->args[i];
		FILE *yf;

		if (UNLIKELY((yf = fopen(fn, "r")) == NULL)) {
			error("cannot open file `%s'", fn);
			rc = 1;
			break;
		} else if (snarf_f(yf) < 0) {
			error("gen command failed on `%s'", fn);
			rc = 1;
		}

		/* clean up */
		fclose(yf);
	}
	/* special directive for the header */
	if (argi->gen.header_arg != NULL) {
		const char *hdr = argi->gen.header_arg;

		/* massage the hdr bit a bit */
		if (strcmp(hdr, "/dev/null")) {
			/* /dev/null just means ignore the header aye? */
			const char *hp;

			if ((hp = strrchr(hdr, '/')) == NULL) {
				hp = hdr;
			} else {
				hp++;
			};
			fprintf(outf, "\ndefine([YUCK_HEADER], [%s])\n", hp);
		}
	}
	/* make sure we close the outfile */
	fputs("\n\
changecom([//])\n\
divert[]dnl\n", outf);
	fclose(outf);
	/* only proceed if there has been no error yet */
	if (rc) {
		goto out;
	} else if (find_auxs() < 0) {
		/* error whilst finding our DSL and things */
		error("cannot find yuck dsl and template files");
		rc = 2;
		goto out;
	}
	/* now route that stuff through m4 */
	with (const char *outfn = argi->gen.output_arg, *hdrfn) {
		if ((hdrfn = argi->gen.header_arg) != NULL) {
			/* run a special one for the header */
			if ((rc = run_m4(hdrfn, deffn, genhfn, NULL))) {
				break;
			}
			/* now run the whole shebang for the beef code */
			rc = run_m4(outfn, deffn, gencfn, NULL);
			break;
		}
		/* standard case: pipe directives, then header, then code */
		rc = run_m4(outfn, deffn, genhfn, gencfn, NULL);
	}
out:
	if (!0/*argi->keep_intermediate*/) {
		unlink(deffn);
	}
	return rc;
}

int
main(int argc, char *argv[])
{
	struct yuck_s argi[1U];
	int rc = 0;

	if (yuck_parse(argi, argc, argv) < 0) {
		rc = 1;
		goto out;
	}

	switch (argi->cmd) {
	default:
		fputs("\
No valid command specified.\n\
See --help to obtain a list of available commands.\n", stderr);
		rc = 1;
		goto out;
	case yuck_gen:
		if ((rc = cmd_gen(argi)) < 0) {
			rc = 1;
		}
		break;
	}

out:
	yuck_free(argi);
	return rc;
}
#endif	/* !BOOTSTRAP */

/* yuck.c ends here */
