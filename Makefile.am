include $(top_builddir)/version.mk

LANG = C
LC_ALL = C

ACLOCAL_AMFLAGS = -I m4

SUBDIRS =
SUBDIRS += src

EXTRA_DIST =
DISTCLEANFILES =

EXTRA_DIST += build-aux/git-version-gen

DISTCLEANFILES += version.mk

dist-hook:
	echo $(VERSION) > $(distdir)/version \
	&& touch -r $(top_builddir)/version.mk $(distdir)/version
