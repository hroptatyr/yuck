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

DISTCLEANFILES += version.mk
DISTCLEANFILES += yuck.version

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA =
pkgconfig_DATA += yuck.pc
EXTRA_DIST += yuck.pc.in
CLEANFILES += yuck.pc

yuck.pc: yuck.pc.in
yuck.pc: yuck.pc.in $(top_builddir)/src/yuck yuck.version
	$(AM_V_GEN) YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src" \
		$(top_builddir)/src/yuck scmver -n --reference yuck.version $< | \
		$(SHELL) $(builddir)/config.status --file=- >$@

dist-hook: version.mk yuck.version
	cp -p $^ $(distdir)/
