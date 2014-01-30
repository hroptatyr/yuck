include $(top_builddir)/version.mk

LANG = C
LC_ALL = C

ACLOCAL_AMFLAGS = -I m4
aclocaldir = $(datadir)/aclocal

SUBDIRS =
SUBDIRS += src
SUBDIRS += info
SUBDIRS += test

EXTRA_DIST =
DISTCLEANFILES =
CLEANFILES =
BUILT_SOURCES =

EXTRA_DIST += version.mk.in
EXTRA_DIST += version.mk
EXTRA_DIST += .version
BUILT_SOURCES += .version
DISTCLEANFILES += version.mk

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA =
pkgconfig_DATA += yuck.pc
EXTRA_DIST += yuck.pc.in
CLEANFILES += yuck.pc

aclocal_DATA = m4/yuck.m4

.version:
	$(AM_V_GEN) echo "v$(VERSION)" > $@

yuck.pc: .version yuck.pc.in
	$(AM_V_GEN) YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src" \
		$(top_builddir)/src/yuck scmver -n \
			--reference .version $(srcdir)/yuck.pc.in | \
		$(SHELL) "$(builddir)/config.status" --file=- >$@
