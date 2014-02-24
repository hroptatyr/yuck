# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea

LANG = C
LC_ALL = C

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_BSD_SOURCE
## for our friends on AIX
AM_CPPFLAGS += -D_ALL_SOURCE
## and Darwin
AM_CPPFLAGS += -D_DARWIN_C_SOURCE
## and NetBSD
AM_CPPFLAGS += -D_NETBSD_SOURCE

bin_PROGRAMS =
noinst_PROGRAMS =
pkglib_LIBRARIES =
pkgdata_DATA =
noinst_LIBRARIES =
BUILT_SOURCES =
EXTRA_DIST = $(BUILT_SOURCES)
DISTCLEANFILES =


EXTRA_DIST += version.c.in
DISTCLEANFILES += version.c

noinst_PROGRAMS += yuck-bootstrap
yuck_bootstrap_SOURCES = yuck.c
yuck_bootstrap_CPPFLAGS = $(AM_CPPFLAGS)
yuck_bootstrap_CPPFLAGS += -DBOOTSTRAP
yuck_bootstrap_CPPFLAGS += -DWITH_SCMVER

noinst_PROGRAMS += yuck-scmver
yuck_scmver_SOURCES = yuck-scmver.c yuck-scmver.h
yuck_scmver_CPPFLAGS = $(AM_CPPFLAGS)
yuck_scmver_CPPFLAGS += -DBOOTSTRAP

bin_PROGRAMS += yuck
yuck_SOURCES = yuck.c yuck.yuck
yuck_SOURCES += version.c version.h
yuck_SOURCES += yuck-scmver.c yuck-scmver.h
yuck_CPPFLAGS = $(AM_CPPFLAGS)
yuck_CPPFLAGS += -DHAVE_VERSION_H
yuck_CPPFLAGS += -DWITH_SCMVER
BUILT_SOURCES += yuck.yucc
DISTCLEANFILES += yuck.m4i yuck.yucc
## aix make wants all prereq's to exist
EXTRA_DIST += yuck.m4i

pkgdata_DATA += yuck.m4
pkgdata_DATA += yuck-scmver.m4
pkgdata_DATA += yuck-coru.c.m4
pkgdata_DATA += yuck-coru.c.m4
pkgdata_DATA += yuck.man.m4
EXTRA_DIST += $(pkgdata_DATA)

## version rules
version.c: version.c.in $(top_builddir)/.version yuck-scmver
	$(AM_V_GEN) $(builddir)/yuck-scmver \
		$(srcdir)/version.c.in | \
		$(M4) - $(srcdir)/version.c.in > $@

## bootstrapping rules
yuck.m4i: yuck.yuck
	$(MAKE) $(AM_MAKEFLAGS) yuck-bootstrap
	$(AM_V_GEN) $(builddir)/yuck-bootstrap $(srcdir)/yuck.yuck > $@

yuck.yucc: yuck.m4i yuck.m4 yuck-coru.h.m4 yuck-coru.c.m4
	$(AM_V_GEN) $(M4) $(srcdir)/yuck.m4 yuck.m4i \
		$(srcdir)/yuck-coru.h.m4 $(srcdir)/yuck-coru.c.m4 > $@ \
		|| { rm -f -- $@; false; }

## for dist or dist check we need binaries, so help2man can work
dist-hook: $(bin_PROGRAMS)

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<

## Makefile.am ends here
