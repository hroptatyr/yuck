include $(top_builddir)/version.mk

LANG = C
LC_ALL = C

ACLOCAL_AMFLAGS = -I m4
aclocaldir = $(datadir)/aclocal

SUBDIRS =
EXTRA_DIST = $(BUILT_SOURCES)
BUILT_SOURCES =
DISTCLEANFILES =
CLEANFILES =

SUBDIRS += src
SUBDIRS += info
SUBDIRS += test

DISTCLEANFILES += version.mk
EXTRA_DIST += version.mk.in
BUILT_SOURCES += .version

aclocal_DATA = m4/yuck.m4

doc_DATA = README.md
EXTRA_DIST += $(doc_DATA)

.version:
	$(AM_V_GEN) echo "v$(VERSION)" > $@

## make sure .version is read-only in the dist
dist-hook:
	chmod ugo-w $(distdir)/.version
