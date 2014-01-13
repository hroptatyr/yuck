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

EXTRA_DIST += build-aux/git-version-gen
EXTRA_DIST += version.m4

DISTCLEANFILES += version.mk

pkgconfigdir = @libdir@/pkgconfig
pkgconfig_DATA =
pkgconfig_DATA += yuck.pc
EXTRA_DIST += yuck.pc.in
CLEANFILES += yuck.pc

yuck.pc: yuck.pc.in
yuck.pc: yuck.pc.in $(top_builddir)/src/yuck-scmver version.mk
	$(AM_V_GEN) $(top_builddir)/src/yuck-scmver $< | \
		m4 - $< | \
		$(SHELL) $(builddir)/config.status --file=- >$@

dist-hook:
	echo $(VERSION) > $(distdir)/version \
	&& touch -r $(top_builddir)/version.mk $(distdir)/version
