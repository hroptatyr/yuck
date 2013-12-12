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

# do_subst, inspired by the automake project
do_subst = ( sed \
  -e 's,[@]VERSION[@],$(VERSION),g' \
  -e 's,[@]PACKAGE_VERSION[@],$(PACKAGE_VERSION),g' \
  -e 's,[@]PACKAGE_STRING[@],$(PACKAGE_STRING),g' \
  | $(SHELL) ./config.status --file=- \
  )

yuck.pc: yuck.pc.in
yuck.pc: yuck.pc.in version.mk
	$(do_subst) <$< >$@

dist-hook:
	echo $(VERSION) > $(distdir)/version \
	&& touch -r $(top_builddir)/version.mk $(distdir)/version
