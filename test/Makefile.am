# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea
LANG=C
LC_ALL=C

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
AM_CPPFLAGS += -D_ALL_SOURCE
AM_CPPFLAGS += -D_NETBSD_SOURCE
AM_CPPFLAGS += -D_DEFAULT_SOURCE
AM_CPPFLAGS += -UHAVE_CONFIG_H -UHAVE_VERSION_H

EXTRA_DIST = $(TESTS)
check_PROGRAMS =
TESTS =
TEST_EXTENSIONS =
BUILT_SOURCES =
CLEANFILES = $(BUILT_SOURCES)

## summon our helper
include clitoris.am
AM_CLIT_LOG_FLAGS = -v --builddir $(top_builddir)/src
TESTS_ENVIRONMENT = root=$(top_srcdir) YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src"


TESTS += plain_01.clit
TESTS += plain_02.clit
TESTS += plain_03.clit
TESTS += plain_04.clit
EXTRA_DIST += plain_01.yuck
EXTRA_DIST += plain_02.yuck
EXTRA_DIST += plain_03.yuck
EXTRA_DIST += plain_04.yuck

TESTS += plain_05.clit
TESTS += plain_06.clit
EXTRA_DIST += plain_05.yuck
EXTRA_DIST += plain_06.yuck

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

BUILT_SOURCES += xmpl-command.yucc
check_PROGRAMS += xmpl-command
EXTRA_DIST += xmpl-command.c xmpl-command.yuck
TESTS += xmpl-command-01.clit

BUILT_SOURCES += xmpl-posarg-with-angles.yucc
check_PROGRAMS += xmpl-posarg-with-angles
EXTRA_DIST += xmpl-posarg-with-angles.c xmpl-posarg-with-angles.yuck
TESTS += xmpl-posarg-with-angles-01.clit

TESTS += scmver_01.clit
TESTS += scmver_02.clit
TESTS += scmver_03.clit
EXTRA_DIST += scmver_02.tmpl
TESTS += scmver_04.clit
TESTS += scmver_05.clit
EXTRA_DIST += scmver_04.ver
EXTRA_DIST += scmver_05.ver
TESTS += scmver_06.clit
TESTS += scmver_07.clit
EXTRA_DIST += scmver_06.ver
EXTRA_DIST += scmver_07.ver
TESTS += scmver_08.clit
TESTS += scmver_09.clit

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

check_PROGRAMS += post-hook_01
post_hook_01_SOURCES = post-hook.c
post_hook_01_CPPFLAGS = $(AM_CPPFLAGS)
post_hook_01_CPPFLAGS += -DYUCC_FILE='"plain_05.yucc"'
BUILT_SOURCES += plain_05.yucc
TESTS += post-hook_01.clit
TESTS += post-hook_02.clit

TESTS += escape_01.clit
TESTS += escape_02.clit
EXTRA_DIST += escape_01.yuck
check_PROGRAMS += generic-escape_02
generic_escape_02_SOURCES = generic.c
generic_escape_02_CPPFLAGS = $(AM_CPPFLAGS)
generic_escape_02_CPPFLAGS += -DYUCC_FILE='"escape_01.yucc"'
BUILT_SOURCES += escape_01.yucc
TESTS += escape_03.clit
TESTS += escape_04.clit
EXTRA_DIST += escape_03.yuck
check_PROGRAMS += generic-escape_04
generic_escape_04_SOURCES = generic.c
generic_escape_04_CPPFLAGS = $(AM_CPPFLAGS)
generic_escape_04_CPPFLAGS += -DYUCC_FILE='"escape_03.yucc"'
BUILT_SOURCES += escape_03.yucc

check_PROGRAMS += generic-plain_05
generic_plain_05_SOURCES = generic.c
generic_plain_05_CPPFLAGS = $(AM_CPPFLAGS)
generic_plain_05_CPPFLAGS += -DYUCC_FILE='"plain_05.yucc"'
BUILT_SOURCES += plain_05.yucc
TESTS += generic-plain_05.clit

check_PROGRAMS += generic-plain_06
generic_plain_06_SOURCES = generic.c
generic_plain_06_CPPFLAGS = $(AM_CPPFLAGS)
generic_plain_06_CPPFLAGS += -DYUCC_FILE='"plain_06.yucc"'
BUILT_SOURCES += plain_06.yucc
TESTS += generic-plain_06.clit


check_PROGRAMS += brack_01
brack_01_SOURCES = generic.c
brack_01_CPPFLAGS = $(AM_CPPFLAGS)
brack_01_CPPFLAGS += -DYUCC_FILE='"brack_01.yucc"'
EXTRA_DIST += brack_01.yuck
BUILT_SOURCES += brack_01.yucc
TESTS += brack_01.clit

check_PROGRAMS += brack_02
brack_02_SOURCES = generic.c
brack_02_CPPFLAGS = $(AM_CPPFLAGS)
brack_02_CPPFLAGS += -DYUCC_FILE='"brack_02.yucc"'
EXTRA_DIST += brack_02.yuck
BUILT_SOURCES += brack_02.yucc
TESTS += brack_02.clit

check_PROGRAMS += paren_01
paren_01_SOURCES = generic.c
paren_01_CPPFLAGS = $(AM_CPPFLAGS)
paren_01_CPPFLAGS += -DYUCC_FILE='"paren_01.yucc"'
EXTRA_DIST += paren_01.yuck
BUILT_SOURCES += paren_01.yucc
TESTS += paren_01.clit

check_PROGRAMS += paren_02
paren_02_SOURCES = generic.c
paren_02_CPPFLAGS = $(AM_CPPFLAGS)
paren_02_CPPFLAGS += -DYUCC_FILE='"paren_02.yucc"'
EXTRA_DIST += paren_02.yuck
BUILT_SOURCES += paren_02.yucc
TESTS += paren_02.clit

check_PROGRAMS += underq_01
underq_01_SOURCES = generic.c
underq_01_CPPFLAGS = $(AM_CPPFLAGS)
underq_01_CPPFLAGS += -DYUCC_FILE='"underq_01.yucc"'
EXTRA_DIST += underq_01.yuck
BUILT_SOURCES += underq_01.yucc
TESTS += underq_01.clit

check_PROGRAMS += underq_02
underq_02_SOURCES = generic.c
underq_02_CPPFLAGS = $(AM_CPPFLAGS)
underq_02_CPPFLAGS += -DYUCC_FILE='"underq_02.yucc"'
EXTRA_DIST += underq_02.yuck
BUILT_SOURCES += underq_02.yucc
TESTS += underq_02.clit

check_PROGRAMS += underq_03
underq_03_SOURCES = generic.c
underq_03_CPPFLAGS = $(AM_CPPFLAGS)
underq_03_CPPFLAGS += -DYUCC_FILE='"underq_03.yucc"'
EXTRA_DIST += underq_03.yuck
BUILT_SOURCES += underq_03.yucc
TESTS += underq_03.clit

check_PROGRAMS += underq_04
underq_04_SOURCES = generic.c
underq_04_CPPFLAGS = $(AM_CPPFLAGS)
underq_04_CPPFLAGS += -DYUCC_FILE='"underq_04.yucc"'
EXTRA_DIST += underq_04.yuck
BUILT_SOURCES += underq_04.yucc
TESTS += underq_04.clit

check_PROGRAMS += underq_05
underq_05_SOURCES = generic.c
underq_05_CPPFLAGS = $(AM_CPPFLAGS)
underq_05_CPPFLAGS += -DYUCC_FILE='"underq_05.yucc"'
EXTRA_DIST += underq_05.yuck
BUILT_SOURCES += underq_05.yucc
TESTS += underq_05.clit

check_PROGRAMS += underq_06
underq_06_SOURCES = generic.c
underq_06_CPPFLAGS = $(AM_CPPFLAGS)
underq_06_CPPFLAGS += -DYUCC_FILE='"underq_06.yucc"'
EXTRA_DIST += underq_06.yuck
BUILT_SOURCES += underq_06.yucc
TESTS += underq_06.clit

check_PROGRAMS += auto-dashdash
auto_dashdash_SOURCES = auto-dashdash.c
auto_dashdash_CPPFLAGS = $(AM_CPPFLAGS)
auto_dashdash_CPPFLAGS += -DYUCC_FILE='"auto-dashdash.yucc"'
EXTRA_DIST += auto-dashdash.yuck
BUILT_SOURCES += auto-dashdash.yucc
TESTS += auto-dashdash_01.clit
TESTS += auto-dashdash_02.clit

check_PROGRAMS += lone-cmd_01
lone_cmd_01_SOURCES = generic.c
lone_cmd_01_CPPFLAGS = $(AM_CPPFLAGS)
lone_cmd_01_CPPFLAGS += -DYUCC_FILE='"lone-cmd_01.yucc"'
EXTRA_DIST += lone-cmd_01.yuck
BUILT_SOURCES += lone-cmd_01.yucc
TESTS += lone-cmd_01.clit

check_PROGRAMS += lone-cmd_02
lone_cmd_02_SOURCES = generic.c
lone_cmd_02_CPPFLAGS = $(AM_CPPFLAGS)
lone_cmd_02_CPPFLAGS += -DYUCC_FILE='"lone-cmd_02.yucc"'
EXTRA_DIST += lone-cmd_02.yuck
BUILT_SOURCES += lone-cmd_02.yucc
TESTS += lone-cmd_02.clit

check_PROGRAMS += dashdesc_01
dashdesc_01_SOURCES = generic.c
dashdesc_01_CPPFLAGS = $(AM_CPPFLAGS)
dashdesc_01_CPPFLAGS += -DYUCC_FILE='"dashdesc_01.yucc"'
EXTRA_DIST += dashdesc_01.yuck
BUILT_SOURCES += dashdesc_01.yucc
TESTS += dashdesc_01.clit


TESTS += scmver_11.clit
TESTS += scmver_12.clit
TESTS += scmver_13.clit
TESTS += scmver_14.clit
TESTS += scmver_15.clit
TESTS += scmver_16.clit
TESTS += scmver_17.clit
TESTS += scmver_18.clit
TESTS += scmver_21.clit
TESTS += scmver_22.clit
TESTS += scmver_23.clit
TESTS += scmver_24.clit
TESTS += scmver_25.clit
TESTS += scmver_26.clit
TESTS += scmver_27.clit
TESTS += scmver_28.clit
EXTRA_DIST += rudi_01.version
EXTRA_DIST += rudi_02.version
EXTRA_DIST += rudi_03.version
EXTRA_DIST += rudi_04.version
EXTRA_DIST += rudi_05.version
EXTRA_DIST += rudi_06.version
EXTRA_DIST += rudi_07.version
EXTRA_DIST += rudi_08.version
EXTRA_DIST += rudi_09.version
EXTRA_DIST += rudi_10.version
EXTRA_DIST += rudi_11.version
EXTRA_DIST += rudi_12.version
EXTRA_DIST += rudi_13.version
EXTRA_DIST += rudi_14.version
EXTRA_DIST += rudi_15.version
EXTRA_DIST += rudi_16.version

TESTS += man-01.clit
EXTRA_DIST += man-01.yuck


## just to get the tests and yucc files rebuilt
$(BUILT_SOURCES): $(top_builddir)/src/yuck$(EXEEXT)

## yuck rule
SUFFIXES = .yuck
SUFFIXES += .yucc
.yuck.yucc:
	$(AM_V_GEN) $(TESTS_ENVIRONMENT) $(top_builddir)/src/yuck gen $< > $@

## Makefile.am ends here
