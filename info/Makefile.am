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

VPATH += $(top_builddir) $(top_srcdir)

## help2man helpers
%.man: .version $(top_srcdir)/src/%.yuck
	$(AM_V_GEN) YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src" \
		$(top_builddir)/src/yuck genman \
			-o $@ --version-file $^

## Makefile.am ends here
