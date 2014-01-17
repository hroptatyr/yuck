include $(top_builddir)/version.mk

LANG = C
LC_ALL = C

ACLOCAL_AMFLAGS = -I m4

SUBDIRS =
SUBDIRS += src
SUBDIRS += info
SUBDIRS += test

EXTRA_DIST =
DISTCLEANFILES =
CLEANFILES =

EXTRA_DIST += version.mk.in
EXTRA_DIST += version.mk
EXTRA_DIST += .version

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA =
pkgconfig_DATA += yuck.pc
EXTRA_DIST += yuck.pc.in
CLEANFILES += yuck.pc

yuck.pc: .version yuck.pc.in
	$(AM_V_GEN) YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src" \
		"$(top_builddir)/src/yuck" scmver -n --reference $^ | \
		$(SHELL) "$(builddir)/config.status" --file=- >$@
