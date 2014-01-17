
AC_DEFUN([GIT_VERSION_MAGIC], [dnl
## initially generate version.mk and yuck.version here
## because only GNU make can do this at make time
	AC_MSG_CHECKING([for stipulated version files])
	if test -f "${srcdir}/.version"; then
		## transform reference version
		VERSION=`"${AWK}" -F'-' 'NR == 1 {
PRE = substr([$]1, 1, 1);
VER = substr([$]1, 2);
if (PRE == "v" || PRE == "V") {
	SCM = substr([$]3, 1, 1);
	if (SCM == "g") {
		VER = VER ".git" [$]2 "." substr([$]3, 2);
	}
	if ([$]4) {
		VER = VER "." [$]4;
	}
	print VER;
}
}' "${srcdir}/.version"`
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
