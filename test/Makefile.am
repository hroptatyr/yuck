include $(top_builddir)/version.mk

# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea
LANG=C
LC_ALL=C

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
AM_CPPFLAGS += -UHAVE_CONFIG_H -UHAVE_VERSION_H

EXTRA_DIST = $(TESTS)
check_PROGRAMS =
TESTS =
TEST_EXTENSIONS =
BUILT_SOURCES =
CLEANFILES =

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

TESTS += option_09.clit
EXTRA_DIST += option_09.yuck
TESTS += option_10.clit
EXTRA_DIST += option_10.yuck
TESTS += option_11.clit
TESTS += option_12.clit

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

BUILT_SOURCES += xmpl.yucc
check_PROGRAMS += xmpl
EXTRA_DIST += xmpl.c xmpl.yuck
TESTS += xmpl-01.clit
TESTS += xmpl-02.clit
TESTS += xmpl-03.clit
TESTS += xmpl-04.clit

BUILT_SOURCES += xmpl-subcommands.yucc
check_PROGRAMS += xmpl-subcommands
EXTRA_DIST += xmpl-subcommands.c xmpl-subcommands.yuck
TESTS += xmpl-subcommands-01.clit
TESTS += xmpl-subcommands-02.clit
TESTS += xmpl-subcommands-03.clit
TESTS += xmpl-subcommands-04.clit

EXTRA_DIST += generic.c
check_PROGRAMS += generic-muloptarg_01
generic_muloptarg_01_SOURCES = generic.c
generic_muloptarg_01_CPPFLAGS = $(AM_CPPFLAGS)
generic_muloptarg_01_CPPFLAGS += -DYUCC_FILE='"muloptarg_01.yucc"'
BUILT_SOURCES += muloptarg_01.yucc
TESTS += generic-muloptarg_01.clit

check_PROGRAMS += generic-muloptarg_02
generic_muloptarg_02_SOURCES = generic.c
generic_muloptarg_02_CPPFLAGS = $(AM_CPPFLAGS)
generic_muloptarg_02_CPPFLAGS += -DYUCC_FILE='"muloptarg_02.yucc"'
BUILT_SOURCES += muloptarg_02.yucc
TESTS += generic-muloptarg_02.clit

check_PROGRAMS += generic-muloptarg_03
generic_muloptarg_03_SOURCES = generic.c
generic_muloptarg_03_CPPFLAGS = $(AM_CPPFLAGS)
generic_muloptarg_03_CPPFLAGS += -DYUCC_FILE='"muloptarg_03.yucc"'
BUILT_SOURCES += muloptarg_03.yucc
TESTS += generic-muloptarg_03.clit

check_PROGRAMS += generic-muloptarg_04
generic_muloptarg_04_SOURCES = generic.c
generic_muloptarg_04_CPPFLAGS = $(AM_CPPFLAGS)
generic_muloptarg_04_CPPFLAGS += -DYUCC_FILE='"muloptarg_04.yucc"'
BUILT_SOURCES += muloptarg_04.yucc
TESTS += generic-muloptarg_04.clit

check_PROGRAMS += generic-mularg_01
generic_mularg_01_SOURCES = generic.c
generic_mularg_01_CPPFLAGS = $(AM_CPPFLAGS)
generic_mularg_01_CPPFLAGS += -DYUCC_FILE='"mularg_01.yucc"'
BUILT_SOURCES += mularg_01.yucc
TESTS += generic-mularg_01.clit

check_PROGRAMS += generic-mularg_02
generic_mularg_02_SOURCES = generic.c
generic_mularg_02_CPPFLAGS = $(AM_CPPFLAGS)
generic_mularg_02_CPPFLAGS += -DYUCC_FILE='"mularg_02.yucc"'
BUILT_SOURCES += mularg_02.yucc
TESTS += generic-mularg_02.clit

check_PROGRAMS += generic-mularg_03
generic_mularg_03_SOURCES = generic.c
generic_mularg_03_CPPFLAGS = $(AM_CPPFLAGS)
generic_mularg_03_CPPFLAGS += -DYUCC_FILE='"mularg_03.yucc"'
BUILT_SOURCES += mularg_03.yucc
TESTS += generic-mularg_03.clit

check_PROGRAMS += generic-mularg_04
generic_mularg_04_SOURCES = generic.c
generic_mularg_04_CPPFLAGS = $(AM_CPPFLAGS)
generic_mularg_04_CPPFLAGS += -DYUCC_FILE='"mularg_04.yucc"'
BUILT_SOURCES += mularg_04.yucc
TESTS += generic-mularg_04.clit

check_PROGRAMS += generic-optarg_01
generic_optarg_01_SOURCES = generic.c
generic_optarg_01_CPPFLAGS = $(AM_CPPFLAGS)
generic_optarg_01_CPPFLAGS += -DYUCC_FILE='"optarg_01.yucc"'
BUILT_SOURCES += optarg_01.yucc
TESTS += generic-optarg_01.clit

check_PROGRAMS += generic-optarg_02
generic_optarg_02_SOURCES = generic.c
generic_optarg_02_CPPFLAGS = $(AM_CPPFLAGS)
generic_optarg_02_CPPFLAGS += -DYUCC_FILE='"optarg_02.yucc"'
BUILT_SOURCES += optarg_02.yucc
TESTS += generic-optarg_02.clit

check_PROGRAMS += generic-optarg_03
generic_optarg_03_SOURCES = generic.c
generic_optarg_03_CPPFLAGS = $(AM_CPPFLAGS)
generic_optarg_03_CPPFLAGS += -DYUCC_FILE='"optarg_03.yucc"'
BUILT_SOURCES += optarg_03.yucc
TESTS += generic-optarg_03.clit

check_PROGRAMS += generic-optarg_04
generic_optarg_04_SOURCES = generic.c
generic_optarg_04_CPPFLAGS = $(AM_CPPFLAGS)
generic_optarg_04_CPPFLAGS += -DYUCC_FILE='"optarg_04.yucc"'
BUILT_SOURCES += optarg_04.yucc
TESTS += generic-optarg_04.clit

check_PROGRAMS += generic-option_01
generic_option_01_SOURCES = generic.c
generic_option_01_CPPFLAGS = $(AM_CPPFLAGS)
generic_option_01_CPPFLAGS += -DYUCC_FILE='"option_01.yucc"'
BUILT_SOURCES += option_01.yucc
TESTS += generic-option_01.clit

check_PROGRAMS += generic-option_02
generic_option_02_SOURCES = generic.c
generic_option_02_CPPFLAGS = $(AM_CPPFLAGS)
generic_option_02_CPPFLAGS += -DYUCC_FILE='"option_02.yucc"'
BUILT_SOURCES += option_02.yucc
TESTS += generic-option_02.clit

check_PROGRAMS += generic-option_03
generic_option_03_SOURCES = generic.c
generic_option_03_CPPFLAGS = $(AM_CPPFLAGS)
generic_option_03_CPPFLAGS += -DYUCC_FILE='"option_03.yucc"'
BUILT_SOURCES += option_03.yucc
TESTS += generic-option_03.clit

check_PROGRAMS += generic-option_04
generic_option_04_SOURCES = generic.c
generic_option_04_CPPFLAGS = $(AM_CPPFLAGS)
generic_option_04_CPPFLAGS += -DYUCC_FILE='"option_04.yucc"'
BUILT_SOURCES += option_04.yucc
TESTS += generic-option_04.clit

check_PROGRAMS += generic-option_05
generic_option_05_SOURCES = generic.c
generic_option_05_CPPFLAGS = $(AM_CPPFLAGS)
generic_option_05_CPPFLAGS += -DYUCC_FILE='"option_05.yucc"'
BUILT_SOURCES += option_05.yucc
TESTS += generic-option_05.clit

check_PROGRAMS += generic-option_06
generic_option_06_SOURCES = generic.c
generic_option_06_CPPFLAGS = $(AM_CPPFLAGS)
generic_option_06_CPPFLAGS += -DYUCC_FILE='"option_06.yucc"'
BUILT_SOURCES += option_06.yucc
TESTS += generic-option_06.clit

check_PROGRAMS += generic-option_07
generic_option_07_SOURCES = generic.c
generic_option_07_CPPFLAGS = $(AM_CPPFLAGS)
generic_option_07_CPPFLAGS += -DYUCC_FILE='"option_07.yucc"'
BUILT_SOURCES += option_07.yucc
TESTS += generic-option_07.clit

check_PROGRAMS += generic-option_08
generic_option_08_SOURCES = generic.c
generic_option_08_CPPFLAGS = $(AM_CPPFLAGS)
generic_option_08_CPPFLAGS += -DYUCC_FILE='"option_08.yucc"'
BUILT_SOURCES += option_08.yucc
TESTS += generic-option_08.clit


## yuck rule
%.yucc: %.yuck $(top_builddir)/src/yuck
	$(AM_V_GEN) $(TESTS_ENVIRONMENT) $(top_builddir)/src/yuck gen $< > $@

## Makefile.am ends here
