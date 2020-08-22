#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(Cute3DSCapture, 1.4, cutecapture@alinas-chaos.de)
AC_CONFIG_SRCDIR([main.cpp])
#AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

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

AC_ARG_WITH([staticsfml],
    AS_HELP_STRING([--with-staticsfml], [Enable static sfml linking]))
AC_ARG_WITH([staticsfmldir],
  [AS_HELP_STRING([--with-staticsfmldir=DIR], [SFML static libary dir])],
  [staticsfmldir=$withval],
  [staticsfmldir="\${libdir}"])
AC_SUBST([staticsfmldir], [$staticsfmldir])
AS_IF([test "x$with_staticsfml" = "xyes"], [
  	AC_DEFINE(SFML_STATIC,,[SFML_STATIC])
	AC_CHECK_LIB([sfml-system-s],[main])#,,AC_MSG_ERROR([ *** Unable to find static SFML library (http://www.sfml-dev.org/)]))
	AC_CHECK_LIB([sfml-window-s],[main])#,,AC_MSG_ERROR([ *** Unable to find static SFML window library (http://www.sfml-dev.org/)]))
	AC_CHECK_LIB([sfml-graphics-s],[main])#,,AC_MSG_ERROR([ *** Unable to find static SFML graphics library (http://www.sfml-dev.org/)]))
	AC_CHECK_LIB([X11],[main],,AC_MSG_ERROR([ *** Unable to find X11 library]))
	AC_CHECK_LIB([X11-xcb],[main],,AC_MSG_ERROR([ *** Unable to find X11-xcb library]))
	AC_CHECK_LIB([xcb],[main],,AC_MSG_ERROR([ *** Unable to find xcb library]))
	AC_CHECK_LIB([xcb-randr],[main],,AC_MSG_ERROR([ *** Unable to find xcb-randr library]))
	AC_CHECK_LIB([xcb-image],[main],,AC_MSG_ERROR([ *** Unable to find xcb-image library]))
	AC_CHECK_LIB([udev],[main],,AC_MSG_ERROR([ *** Unable to find udev library]))
	AC_CHECK_LIB([pthread],[main],,AC_MSG_ERROR([ *** Unable to find pthread library]))
	AC_CHECK_LIB([freetype],[main],,AC_MSG_ERROR([ *** Unable to find freetype library]))
	AC_CHECK_LIB([jpeg],[main],,AC_MSG_ERROR([ *** Unable to find jpeg library]))
	AC_CHECK_LIB([GL],[main],,AC_MSG_ERROR([ *** Unable to find GL library]))
], [
	AC_CHECK_LIB([sfml-system],[main],,AC_MSG_ERROR([ *** Unable to find SFML library (http://www.sfml-dev.org/)]))
	AC_CHECK_LIB([sfml-window],[main],,AC_MSG_ERROR([ *** Unable to find SFML window library (http://www.sfml-dev.org/)]))
	AC_CHECK_LIB([sfml-graphics],[main],,AC_MSG_ERROR([ *** Unable to find SFML graphics library (http://www.sfml-dev.org/)]))
])
AM_CONDITIONAL(STATIC, test "x$with_staticsfml" = "xyes")

AC_CHECK_LIB([usb-1.0],[main],,AC_MSG_ERROR([ *** Unable to find libusb1.0]))

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_OUTPUT(Makefile)
