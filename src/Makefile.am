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
noinst_LIBRARIES =
BUILT_SOURCES =
EXTRA_DIST = $(BUILT_SOURCES)
CLEANFILES =

CLEANFILES += version.c
EXTRA_DIST += version.c.in

noinst_LIBRARIES += libyuck.a
libyuck_a_SOURCES = version.c version.h


noinst_PROGRAMS += yuck
yuck_CPPFLAGS = $(AM_CPPFLAGS)
yuck_CPPFLAGS += -DBOOTSTRAP

noinst_PROGRAMS += test
EXTRA_test_SOURCES = beef.m4i beef.m4c yuck.yuck
BUILT_SOURCES += yuck.x yuck.h

yuck.h: yuck.yuck $(builddir)/yuck
	$(builddir)/yuck $< > $@ || rm -f -- $@

yuck.x: beef.m4i beef-co.m4c
	m4 $^ > $@ || rm -f -- $@

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
