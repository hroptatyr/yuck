/*** yuck-version.c -- snarf versions off project cwds
 *
 * Copyright (C) 2013-2014 Sebastian Freundt
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
#include <sys/wait.h>
#include <sys/stat.h>
#include <yuck-version.h>

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

static char*
xdirname(char *restrict fn, const char *fp)
{
/* find next dir in FN from FP backwards */
	if (fp == NULL) {
		fp = fn + strlen(fn);
	}

	while (--fp >= fn && *fp != '/');
	while (fp >= fn && *fp-- == '/');
	if (fp >= fn) {
		/* replace / by \nul and return pointer */
		char *dp = fn + (fp - fn);
		*++dp = '\0';
		return dp;
	}
	/* return \nul */
	return NULL;
}

static unsigned int
hextou(const char *sp, char **ep)
{
	register unsigned int res = 0U;
	size_t i;

	if (UNLIKELY(sp == NULL)) {
		return 0U;
	} else if (*sp == '\0') {
		goto out;
	}
	for (i = 0U; i < sizeof(res) * 8U / 4U; sp++, i++) {
		register unsigned int this;
		switch (*sp) {
		case '0' ... '9':
			this = *sp - '0';
			break;
		case 'a' ... 'f':
			this = *sp - 'a' + 10U;
			break;
		case 'A' ... 'F':
			this = *sp - 'A' + 10U;
			break;
		default:
			goto fucked;
		}

		res <<= 4U;
		res |= this;
	}
fucked:
	for (; i < sizeof(res) * 8U / 4U; i++, res <<= 4U);
out:
	if (ep != NULL) {
		*ep = (char*)0U + (sp - (char*)0U);
	}
	return res;
}


/* version snarfers */
static __attribute__((noinline)) pid_t
run(int *fd, ...)
{
	static char *cmdline[16U];
	va_list vap;
	pid_t p;
	/* to snarf off traffic from the child */
	int intfd[2];

	va_start(vap, fd);
	for (size_t i = 0U;
	     i < countof(cmdline) &&
		     (cmdline[i] = va_arg(vap, char*)) != NULL; i++);
	va_end(vap);

	if (pipe(intfd) < 0) {
		error("pipe setup to/from %s failed", cmdline[0U]);
		return -1;
	}

	switch ((p = vfork())) {
	case -1:
		/* i am an error */
		error("vfork for %s failed", cmdline[0U]);
		return -1;

	default:
		/* i am the parent */
		close(intfd[1]);
		if (fd != NULL) {
			*fd = intfd[0];
		} else {
			close(intfd[0]);
		}
		return p;

	case 0:
		/* i am the child */
		break;
	}

	/* child code here */
	close(intfd[0]);
	dup2(intfd[1], STDOUT_FILENO);

	execvp(cmdline[0U], cmdline);
	error("execvp(%s) failed", cmdline[0U]);
	_exit(EXIT_FAILURE);
}

static int
fin(pid_t p)
{
	int rc = 2;
	int st;

	while (waitpid(p, &st, 0) != p);
	if (WIFEXITED(st)) {
		rc = WEXITSTATUS(st);
	}
	return rc;
}

static yuck_scm_t
find_scm(char *restrict fn, size_t fz, const char *path)
{
	struct stat st[1U];
	char *restrict dp = fn;

	/* make a copy so we can fiddle with it */
	if (path == NULL ||
	    (dp += xstrlcpy(fn, path, fz)) == fn) {
	cwd:
		/* just use "." then */
		*dp++ = '.';
		*dp = '\0';
	}
again:
	if (stat(fn, st) < 0) {
		return YUCK_SCM_ERROR;
	} else if (UNLIKELY((size_t)(dp - fn) + 5U >= fz)) {
		/* not enough space */
		return YUCK_SCM_ERROR;
	} else if (!S_ISDIR(st->st_mode)) {
		/* not a directory, get the dir bit and start over */
		if ((dp = xdirname(fn, dp)) == NULL) {
			dp = fn;
			goto cwd;
		}
		goto again;
	}

scm_chk:
	/* now check for .git, .bzr, .hg */
	xstrlcpy(dp, "/.git", fz - (dp - fn));
	if (stat(fn, st) == 0 && S_ISDIR(st->st_mode)) {
		/* yay it's a .git */
		*dp = '\0';
		return YUCK_SCM_GIT;
	}

	xstrlcpy(dp, "/.bzr", fz - (dp - fn));
	if (stat(fn, st) == 0 && S_ISDIR(st->st_mode)) {
		/* yay it's a .git */
		*dp = '\0';
		return YUCK_SCM_BZR;
	}

	xstrlcpy(dp, "/.hg", fz - (dp - fn));
	if (stat(fn, st) == 0 && S_ISDIR(st->st_mode)) {
		/* yay it's a .git */
		*dp = '\0';
		return YUCK_SCM_HG;
	}
	/* nothing then, traverse upwards */
	if (*fn != '/') {
		/* make sure we don't go up indefinitely
		 * comparing the current inode to ./.. */
		with (ino_t curino) {
			*dp = '\0';
			if (stat(fn, st) < 0) {
				return YUCK_SCM_ERROR;
			}
			/* memorise inode */
			curino = st->st_ino;
			/* go upwards by appending /.. */
			dp += xstrlcpy(dp, "/..", fz - (dp - fn));
			/* check inode again */
			if (stat(fn, st) < 0) {
				return YUCK_SCM_ERROR;
			} else if (st->st_ino == curino) {
				break;
			}
			goto scm_chk;
		}
	} else if ((dp = xdirname(fn, dp)) != NULL) {
		goto scm_chk;
	}
	return YUCK_SCM_TARBALL;
}


static int
git_version(struct yuck_version_s v[static 1U])
{
	pid_t chld;
	int fd[1U];
	int rc = 0;

	if ((chld = run(fd, "git", "describe",
			"--match=v[0-9]*",
			"--abbrev=8", "--dirty", NULL)) < 0) {
		return -1;
	}
	/* shouldn't be heaps, so just use a single read */
	with (char buf[256U]) {
		const char *vtag;
		const char *dist;
		char *bp;
		ssize_t nrd;

		if ((nrd = read(*fd, buf, sizeof(buf))) <= 0) {
			/* no version then aye */
			rc = -1;
			break;
		}
		buf[nrd - 1U/* for \n*/] = '\0';
		/* parse buf */
		bp = buf;
		if (*bp++ != 'v') {
			/* weird, we req'd v-tags though */
			rc = -1;
			break;
		} else if ((bp = strchr(vtag = bp, '-')) != NULL) {
			/* tokenise sting */
			*bp++ = '\0';
		}
		/* bang vtag */
		xstrlcpy(v->vtag, vtag, sizeof(v->vtag));

		/* snarf distance */
		if (bp == NULL) {
			break;
		} else if ((bp = strchr(dist = bp, '-')) != NULL) {
			/* tokenize */
			*bp++ = '\0';
		}
		/* read distance */
		v->dist = strtoul(dist, &bp, 10);

		if (*++bp == 'g') {
			/* read scm revision */
			v->rvsn = hextou(++bp, &bp);
		}
		if (*bp == '\0') {
			break;
		} else if (*bp == '-') {
			bp++;
		}
		if (!strcmp(bp, "dirty")) {
			v->dirty = 1U;
		}
	}
	close(*fd);
	if (fin(chld) != 0) {
		rc = -1;
	}
	return rc;
}

static int
hg_version(struct yuck_version_s v[static 1U])
{
	pid_t chld;
	int fd[1U];
	int rc = 0;

	if ((chld = run(fd, "hg", "log",
			"--rev", ".",
			"--template",
			"{latesttag}\t{latesttagdistance}\t{node|short}\n",
			NULL)) < 0) {
		return -1;
	}
	/* shouldn't be heaps, so just use a single read */
	with (char buf[256U]) {
		const char *vtag;
		const char *dist;
		char *bp;
		ssize_t nrd;

		if ((nrd = read(*fd, buf, sizeof(buf))) <= 0) {
			/* no version then aye */
			break;
		}
		buf[nrd - 1U/* for \n*/] = '\0';
		/* parse buf */
		bp = buf;
		if (*bp++ != 'v') {
			/* technically we could request the latest v-tag
			 * but i'm no hg buff so fuck it */
			rc = -1;
			break;
		} else if ((bp = strchr(vtag = bp, '\t')) != NULL) {
			/* tokenise */
			*bp++ = '\0';
		}
		/* bang vtag */
		xstrlcpy(v->vtag, vtag, sizeof(v->vtag));

		if (UNLIKELY(bp == NULL)) {
			/* huh? */
			rc = -1;
			break;
		} else if ((bp = strchr(dist = bp, '\t')) != NULL) {
			/* tokenise */
			*bp++ = '\0';
		}
		/* bang distance */
		v->dist = strtoul(dist, NULL, 10);

		/* bang revision */
		v->rvsn = hextou(bp, NULL);
	}
	close(*fd);
	if (fin(chld) != 0) {
		rc = -1;
	}
	return rc;
}

static int
bzr_version(struct yuck_version_s v[static 1U])
{
	pid_t chld;
	int fd[1U];
	int rc = 0;

	/* first get current revision number */
	if ((chld = run(fd, "bzr", "revno", NULL)) < 0) {
		return -1;
	}
	/* shouldn't be heaps, so just use a single read */
	with (char buf[256U]) {
		ssize_t nrd;

		if ((nrd = read(*fd, buf, sizeof(buf))) <= 0) {
			/* no version then aye */
			break;
		}
		v->rvsn = strtoul(buf, NULL, 10);
	}
	close(*fd);
	if (fin(chld) != 0) {
		return -1;
	}

	if ((chld = run(fd, "bzr", "tags",
			"--sort=time", NULL)) < 0) {
		return -1;
	}
	/* could be a lot, we only need the last line though */
	with (char buf[4096U]) {
		const char *vtag;
		size_t bz;
		char *bp;
		ssize_t nrd;

		bp = buf;
		bz = sizeof(buf);
		while ((nrd = read(*fd, bp, bz)) == bz) {
			/* find last line */
			char *lp = bp + bz;
			while (--lp >= buf && *lp != '\n');
			bz = sizeof(buf) - (++lp - buf);
			bp = buf + bz;
			memmove(buf, lp, bz);
		}
		if (nrd <= 0) {
			/* no version then aye */
			break;
		}
		bp[nrd - 1U/* for \n*/] = '\0';
		/* find last line */
		bp += nrd;
		while (--bp >= buf && *bp != '\n');

		/* parse buf */
		if (*++bp != 'v') {
			/* we want v tags, we could go back and see if
			 * there are any */
			rc = -1;
			break;
		} else if ((bp = strchr(vtag = ++bp, ' ')) != NULL) {
			/* tokenise */
			*bp++ = '\0';
		}
		/* bang vtag */
		xstrlcpy(v->vtag, vtag, sizeof(v->vtag));

		if (bp == NULL) {
			break;
		}
		/* read over all the whitespace to find the tag's revno */
		while (*++bp == ' ');
		with (unsigned int rno = strtoul(bp, NULL, 10)) {
			v->dist = v->rvsn - rno;
		}
	}
	close(*fd);
	if (fin(chld) != 0) {
		rc = -1;
	}
	return rc;
}


/* public api */
#if !defined PATH_MAX
# define PATH_MAX	(256U)
#endif	/* !PATH_MAX */

int
yuck_version(struct yuck_version_s v[static 1U], const char *path)
{
	char cwd[PATH_MAX];
	char fn[PATH_MAX];
	int rc;

	/* initialise result structure */
	memset(v, 0, sizeof(*v));

	if (getcwd(cwd, sizeof(cwd)) < 0) {
		return -1;
	}

	switch ((v->scm = find_scm(fn, sizeof(fn), path))) {
	case YUCK_SCM_ERROR:
		return -1;
	case YUCK_SCM_TARBALL:
	default:
		/* can't determine version numbers in tarball, can we? */
		rc = 0;
		break;
	case YUCK_SCM_GIT:
	case YUCK_SCM_BZR:
	case YUCK_SCM_HG:
		chdir(fn);
		switch (v->scm) {
		case YUCK_SCM_GIT:
			rc = git_version(v);
			break;
		case YUCK_SCM_BZR:
			rc = bzr_version(v);
			break;
		case YUCK_SCM_HG:
			rc = hg_version(v);
			break;
		default:
			break;
		}
		chdir(cwd);
		break;
	}
	return rc;
}

/* yuck-version.c ends here */
