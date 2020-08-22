AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
CFLAGS = --pedantic -Wall -std=c99 -s
CXXFLAGS = -std=c++11

if DEBUG
CFLAGS += -g
else
CFLAGS += -O2
endif

LDFLAGS =

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = Cute3DSCapture CuteDSCapture

Cute3DSCapture_SOURCES = main.cpp 3dscapture.h 3dscapture.c

CuteDSCapture_SOURCES = main.cpp dscapture.h dscapture.c
CuteDSCapture_CPPFLAGS = -DDS

if STATIC
LDADD = @staticsfmldir@/libsfml-graphics-s.a @staticsfmldir@/libsfml-window-s.a @staticsfmldir@/libsfml-system-s.a
endif

EXTRA_DIST = README.md 95-3dscapture.rules 95-dscapture.rules Cute3DSCapture.png Cute3DSCapture.desktop CuteDSCapture.png CuteDSCapture.desktop
