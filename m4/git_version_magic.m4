
AC_DEFUN([GIT_VERSION_MAGIC], [dnl
## initially generate version.mk and yuck.version here
## because only GNU make can do this at make time
	AC_MSG_CHECKING([for stipulated version files])
	if test -f "${srcdir}/.version"; then
		## do bugger all
		REFERENCE_VERSION=`head -n 1 "${srcdir}/.version"`
		case "${REFERENCE_VERSION}" in
		("v"*)
			## redefine VERSION
			VERSION="${REFERENCE_VERSION#v}"
			;;
		("V"*)
			## redefine VERSION
			VERSION="${REFERENCE_VERSION#V}"
			;;
		esac
		AC_MSG_RESULT([.version -> ${VERSION}])
	else
		echo "v${VERSION}" > .version
		AC_MSG_RESULT([none])
	fi
	if test -f "${srcdir}/version.mk"; then
		## do bugger all
		:
	else
		${M4:-m4} -DYUCK_SCMVER_VERSION="${VERSION}" \
			"${srcdir}/version.mk.in" > version.mk
	fi
])
