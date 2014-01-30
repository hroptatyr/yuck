# Help the Developers and yourself.  Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea
LANG=C
LC_ALL=C

BUILT_SOURCES =
BUILT_MANS =
EXTRA_DIST = $(BUILT_SOURCES) $(BUILT_MANS)

man1_MANS =
man1_MANS += $(BUILT_MANS)

BUILT_MANS += yuck.man

VPATH += $(builddir):$(srcdir):$(top_builddir)/src:$(top_srcdir)/src

## help2man helpers
SUFFIXES = .yuck
SUFFIXES += .man
yuck.man: $(top_builddir)/.version
.yuck.man:
	$(AM_V_GEN) YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src" \
		$(top_builddir)/src/yuck genman \
			-o $@ --version-file $(top_builddir)/.version $<

## Makefile.am ends here
