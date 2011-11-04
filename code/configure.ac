AC_INIT([Bina], [0.1], [jeremy.attali@gmail.com], [bina], [])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define foreign])
AC_CONFIG_HEADERS([config.h])

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

dnl Checks for libraries.

dnl Extra flags
CPPFLAGS=""
CXXFLAGS=""
LDFLAGS="$LDFLAGS"
AC_SUBST(CPPFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)

AC_CONFIG_FILES([Makefile
				 src/Makefile])
AC_OUTPUT
