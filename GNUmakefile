# This GNUmakefile is used only if you run GNU Make.

# If the user runs GNU make but has not yet run ./configure,
# give them a diagnostic.
_gl-Makefile := $(wildcard [M]akefile)
ifneq ($(_gl-Makefile),)

_dist-target_p ?= $(filter-out %clean, $(filter dist%,$(MAKECMDGOALS)))

include Makefile

DIST_ARCHIVES := $(PACKAGE)-*.tar.gz $(PACKAGE)-*.tar.xz

# update the included makefile snippet which sets VERSION variables
version.mk: version.mk.in FORCE
	$(AM_V_GEN) if test -f $(top_builddir)/src/yuck; then \
		YUCK_TEMPLATE_PATH="$(abs_top_srcdir)/src" \
		$(top_builddir)/src/yuck scmver \
			--reference yuck.version -o $@ $< \
	; elif test -n "$(_dist-target_p)"; then \
		echo "WARNING: you're running a dist target with wrong version information)" >&2 \
		touch yuck.version \
	; else \
		touch yuck.version \
	; fi

else

.DEFAULT_GOAL := abort-due-to-no-makefile
$(MAKECMDGOALS): abort-due-to-no-makefile

abort-due-to-no-makefile:
	@echo There seems to be no Makefile in this directory.   1>&2
	@echo "You must run ./configure before running 'make'." 1>&2
	exit 1

endif

.PHONY: FORCE
