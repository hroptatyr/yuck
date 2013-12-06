include $(top_builddir)/version.mk

# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea
LANG=C
LC_ALL=C

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_BSD_SOURCE

EXTRA_DIST = $(TESTS)
EXTRA_DIST += $(BUILT_SOURCES)
check_PROGRAMS =
TESTS =
TEST_EXTENSIONS =
BUILT_SOURCES =

EXTRA_DIST += xmpl.c xmpl.yuck

TEST_EXTENSIONS += .clit
TESTS_ENVIRONMENT = root=$(top_srcdir) YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src"
CLIT_LOG_COMPILER = $(builddir)/clitoris
AM_CLIT_LOG_FLAGS = -v --builddir $(top_builddir)/src
AM_LOG_COMPILER = false

## our friendly helpers
check_PROGRAMS += clitoris
clitoris_SOURCES = clitoris.c clitoris.yuck
clitoris_CPPFLAGS = $(AM_CPPFLAGS) -D_GNU_SOURCE
clitoris_LDFLAGS = -static
BUILT_SOURCES += clitoris.yucc

## yuck rule
%.yucc: %.yuck $(top_builddir)/src/yuck
	$(AM_V_GEN) $(TESTS_ENVIRONMENT) $(top_builddir)/src/yuck gen $< > $@

## Makefile.am ends here
