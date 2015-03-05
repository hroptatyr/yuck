include $(top_builddir)/version.mk

LANG = C
LC_ALL = C

ACLOCAL_AMFLAGS = -I m4
aclocaldir = $(datadir)/aclocal

SUBDIRS =
EXTRA_DIST =
DISTCLEANFILES =
CLEANFILES =

SUBDIRS += src
SUBDIRS += info
SUBDIRS += test

DISTCLEANFILES += version.mk
DISTCLEANFILES += .version
EXTRA_DIST += version.mk.in
EXTRA_DIST += version.mk
EXTRA_DIST += .version

aclocal_DATA = m4/yuck.m4

doc_DATA = README.md
EXTRA_DIST += $(doc_DATA)

.version:
	$(AM_V_GEN) echo "v$(VERSION)" > $@

## make sure .version is read-only in the dist
dist-hook:
	chmod ugo-w $(distdir)/.version
