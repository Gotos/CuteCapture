AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
CFLAGS = --pedantic -Wall -std=c99 -O2 -s
LDFLAGS =

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = Cute3DSCapture

Cute3DSCapture_SOURCES = main.cpp 3dscapture.h 3dscapture.c
if STATIC
LDADD = @staticsfmldir@/libsfml-graphics-s.a @staticsfmldir@/libsfml-window-s.a @staticsfmldir@/libsfml-system-s.a
endif

EXTRA_DIST = README.md 95-3dscapture.rules Cute3DSCapture.png Cute3DSCapture.desktop
