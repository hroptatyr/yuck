include $(top_builddir)/version.mk

# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea
LANG=C
LC_ALL=C

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_BSD_SOURCE

EXTRA_DIST = $(TESTS)
check_PROGRAMS =
TESTS =
TEST_EXTENSIONS =
BUILT_SOURCES =
CLEANFILES =

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
CLEANFILES += $(BUILT_SOURCES)

TESTS += plain_01.clit
TESTS += plain_02.clit
TESTS += plain_03.clit
TESTS += plain_04.clit
EXTRA_DIST += plain_01.yuck
EXTRA_DIST += plain_02.yuck
EXTRA_DIST += plain_03.yuck
EXTRA_DIST += plain_04.yuck

TESTS += subcmd_01.clit
EXTRA_DIST += subcmd_01.yuck
TESTS += subcmd_02.clit
EXTRA_DIST += subcmd_02.yuck
TESTS += subcmd_03.clit
EXTRA_DIST += subcmd_03.yuck
TESTS += subcmd_04.clit
EXTRA_DIST += subcmd_04.yuck

TESTS += option_01.clit
EXTRA_DIST += option_01.yuck
TESTS += option_02.clit
EXTRA_DIST += option_02.yuck
TESTS += option_03.clit
EXTRA_DIST += option_03.yuck
TESTS += option_04.clit
EXTRA_DIST += option_04.yuck
TESTS += option_05.clit
EXTRA_DIST += option_05.yuck
TESTS += option_06.clit
EXTRA_DIST += option_06.yuck
TESTS += option_07.clit
EXTRA_DIST += option_07.yuck
TESTS += option_08.clit
EXTRA_DIST += option_08.yuck

TESTS += optarg_01.clit
EXTRA_DIST += optarg_01.yuck
TESTS += optarg_02.clit
EXTRA_DIST += optarg_02.yuck
TESTS += optarg_03.clit
EXTRA_DIST += optarg_03.yuck
TESTS += optarg_04.clit
EXTRA_DIST += optarg_04.yuck

TESTS += mularg_01.clit
EXTRA_DIST += mularg_01.yuck
TESTS += mularg_02.clit
EXTRA_DIST += mularg_02.yuck
TESTS += mularg_03.clit
EXTRA_DIST += mularg_03.yuck
TESTS += mularg_04.clit
EXTRA_DIST += mularg_04.yuck

TESTS += muloptarg_01.clit
EXTRA_DIST += muloptarg_01.yuck
TESTS += muloptarg_02.clit
EXTRA_DIST += muloptarg_02.yuck
TESTS += muloptarg_03.clit
EXTRA_DIST += muloptarg_03.yuck
TESTS += muloptarg_04.clit
EXTRA_DIST += muloptarg_04.yuck

TESTS += noauto_01.clit
TESTS += noauto_02.clit
EXTRA_DIST += noauto_01.yuck


## yuck rule
%.yucc: %.yuck $(top_builddir)/src/yuck
	$(AM_V_GEN) $(TESTS_ENVIRONMENT) $(top_builddir)/src/yuck gen $< > $@

## Makefile.am ends here
