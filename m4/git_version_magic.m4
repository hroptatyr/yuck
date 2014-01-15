
AC_DEFUN([GIT_VERSION_MAGIC], [dnl
## initially generate version.mk and yuck.version here
## because only GNU make can do this at make time
	echo "v${PACKAGE_VERSION}" > yuck.version
	${M4:-m4} -DYUCK_SCMVER_VERSION="${PACKAGE_VERSION}" \
		"${srcdir}/version.mk.in" > version.mk
])
