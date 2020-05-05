# catpoint
VERSION = 1.0

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# libs
LIBS = -lncursesw
# Gentoo
#LIBS = -lncursesw -ltinfow

# flags
CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os
LDFLAGS = -s ${LIBS}

# compiler and linker
CC = cc

