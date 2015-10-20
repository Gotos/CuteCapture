#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(Cute3DSCapture, 1.3.1, cutecapture@alinas-chaos.de)
AM_INIT_AUTOMAKE(Cute3DSCapture, 1.3.1)
AC_CONFIG_SRCDIR([main.cpp])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

AC_CHECK_LIB([usb-1.0],[main],,AC_MSG_ERROR([ *** Unable to find libusb1.0]))
AC_CHECK_LIB([sfml-system],[main],,AC_MSG_ERROR([ *** Unable to find SFML library (http://www.sfml-dev.org/)]))
AC_CHECK_LIB([sfml-window],[main],,AC_MSG_ERROR([ *** Unable to find SFML window library (http://www.sfml-dev.org/)]))
AC_CHECK_LIB([sfml-graphics],[main],,AC_MSG_ERROR([ *** Unable to find SFML graphics library (http://www.sfml-dev.org/)]))

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_OUTPUT(Makefile)
