# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea
include $(top_builddir)/version.mk

LANG = C
LC_ALL = C

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_BSD_SOURCE

bin_PROGRAMS =
noinst_PROGRAMS =
pkglib_LIBRARIES =
pkgdata_DATA =
noinst_LIBRARIES =
BUILT_SOURCES =
EXTRA_DIST = $(BUILT_SOURCES)
CLEANFILES =

CLEANFILES += version.c
EXTRA_DIST += version.c.in

noinst_LIBRARIES += libyuck.a
libyuck_a_SOURCES = version.c version.h


noinst_PROGRAMS += yuck-bootstrap
yuck_bootstrap_SOURCES = yuck.c
yuck_bootstrap_CPPFLAGS = $(AM_CPPFLAGS)
yuck_bootstrap_CPPFLAGS += -DBOOTSTRAP

bin_PROGRAMS += yuck
yuck_SOURCES = yuck.c yuck.yuck
BUILT_SOURCES += yuck.yucc
CLEANFILES += yuck.m4i yuck.yucc

pkgdata_DATA += yuck.m4
pkgdata_DATA += yuck-coru.m4c
pkgdata_DATA += yuck-coru.m4h
pkgdata_DATA += yuck.m4man
EXTRA_DIST += $(pkgdata_DATA)


## bootstrapping rules
yuck.m4i: yuck.yuck $(builddir)/yuck-bootstrap
	$(builddir)/yuck-bootstrap $< > $@

yuck.yucc: yuck.m4 yuck.m4i yuck-coru.m4h yuck-coru.m4c
	m4 $+ > $@ || rm -f -- $@


## generate version.c from .c.in
version.c: version.c.in $(top_builddir)/version.mk
	sed -e 's,[@]VERSION[@],$(VERSION),g' <$(srcdir)/$@.in >$@


## for dist or dist check we need binaries, so help2man can work
dist-hook: $(bin_PROGRAMS)

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<

## Makefile.am ends here
