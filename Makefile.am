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

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA =
pkgconfig_DATA += yuck.pc
EXTRA_DIST += yuck.pc.in
CLEANFILES += yuck.pc

aclocal_DATA = m4/yuck.m4

doc_DATA = README.md
EXTRA_DIST += $(doc_DATA)

.version:
	$(AM_V_GEN) echo "v$(VERSION)" > $@

## make sure .version is read-only in the dist
dist-hook:
	chmod ugo-w $(distdir)/.version

yuck.pc: .version yuck.pc.in
	$(AM_V_GEN) YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src" \
		$(top_builddir)/src/yuck scmver -n \
			--reference .version $(srcdir)/yuck.pc.in | \
		$(SHELL) "$(builddir)/config.status" --file=- >$@
