AC_PREREQ([2.69])
AC_INIT([egt-benchmark],[1.3])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AC_USE_SYSTEM_EXTENSIONS
AC_CANONICAL_TARGET
PKG_PROG_PKG_CONFIG

AM_INIT_AUTOMAKE([1.10 foreign subdir-objects])

# Enable quiet compiles on automake 1.11.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT([win32-dll])

AC_LANG(C++)
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([17], [noext])
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_UINT32_T

AX_PTHREAD([LIBS="$PTHREAD_LIBS $LIBS"
            CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
            CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"],
            AC_MSG_ERROR(Can not find pthreads.  This is required.))

PKG_CHECK_MODULES(LIBEGT, [libegt >= 1.11], [], [
   AC_MSG_ERROR(libegt not found.  This is required.)
])

PKG_CHECK_MODULES(LIBINPUT, [libinput >= 1.6.3], [], [
   AC_MSG_ERROR(libinput not found.  This is required.)
])

PKG_CHECK_MODULES(CAIRO, [cairo >= 1.14.6], [], [
   AC_MSG_ERROR(cairo not found.  This is required.)
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
