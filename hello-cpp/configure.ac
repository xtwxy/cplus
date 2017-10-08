#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([hello-cpp], [1.0.0], [https://github.com/xtwxy/cplus])
AM_INIT_AUTOMAKE
AM_PROG_CC_C_O
AM_PROG_AR
AC_CONFIG_SRCDIR([src/hello.cpp])
AC_CONFIG_HEADERS([include/config.h])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
