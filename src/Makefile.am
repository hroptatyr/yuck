# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea

LANG = C
LC_ALL = C

AM_CFLAGS = $(EXTRA_CFLAGS)

bin_PROGRAMS =
noinst_PROGRAMS =
pkglib_LIBRARIES =
pkgdata_DATA =
noinst_LIBRARIES =
BUILT_SOURCES =
EXTRA_DIST = $(BUILT_SOURCES)
DISTCLEANFILES =

include $(srcdir)/yuck.am

noinst_PROGRAMS += yuck-scmver
yuck_scmver_SOURCES = yuck-scmver.c yuck-scmver.h
yuck_scmver_CPPFLAGS = $(yuck_CPPFLAGS)
yuck_scmver_CPPFLAGS += -DBOOTSTRAP

## fiddle with yuck_CPPFLAGS
yuck_CPPFLAGS += -DHAVE_VERSION_H
## include version info
yuck_SOURCES += version.c version.h
## and make it an installed thing
bin_PROGRAMS += yuck

EXTRA_DIST += version.c.in
DISTCLEANFILES += version.c

pkgdata_DATA += yuck.m4
pkgdata_DATA += yuck-scmver.m4
pkgdata_DATA += yuck-coru.c.m4
pkgdata_DATA += yuck-coru.h.m4
pkgdata_DATA += yuck.man.m4
pkgdata_DATA += yuck.mk
pkgdata_DATA += yuck.am
EXTRA_DIST += $(pkgdata_DATA)

## version rules
version.c: version.c.in $(top_builddir)/.version
	$(AM_V_GEN) $(builddir)/yuck-scmver \
		$(srcdir)/version.c.in | \
		$(M4) - $(srcdir)/version.c.in > $@

## for dist or dist check we need binaries, so help2man can work
dist-hook: $(bin_PROGRAMS)

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<

## Makefile.am ends here
