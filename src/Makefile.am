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
CLEANFILES =


EXTRA_DIST += version.c.in
CLEANFILES += version.c

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
CLEANFILES += yuck.m4i yuck.yucc

pkgdata_DATA += yuck.m4
pkgdata_DATA += yuck-scmver.m4
pkgdata_DATA += yuck-coru.m4c
pkgdata_DATA += yuck-coru.m4h
pkgdata_DATA += yuck.m4man
EXTRA_DIST += $(pkgdata_DATA)


## version rules
version.c: version.c.in .version "$(builddir)/yuck-scmver"
	$(AM_V_GEN) "$(builddir)/yuck-scmver" $^ | m4 - $< > $@

## bootstrapping rules
yuck.m4i: yuck.yuck "$(builddir)/yuck-bootstrap" .version
	$(AM_V_GEN) "$(builddir)/yuck-bootstrap" $< > $@

yuck.yucc: yuck.m4 yuck.m4i yuck-coru.m4h yuck-coru.m4c
	$(AM_V_GEN) $${M4:-m4} $+ > $@ || { rm -f -- $@; false; }

## for dist or dist check we need binaries, so help2man can work
dist-hook: $(bin_PROGRAMS)

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<

## Makefile.am ends here
