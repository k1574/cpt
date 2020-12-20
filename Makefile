# catpoint - simple presentation software
# See LICENSE file for copyright and license details.

.POSIX:

NAME = catpoint
VERSION = 1.0

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# use system flags.
CATPOINT_CFLAGS = ${CFLAGS}
CATPOINT_CPPFLAGS = ${CPPFLAGS}
CATPOINT_LDFLAGS = ${LDFLAGS} -lncursesw

# Gentoo
#CATPOINT_LDFLAGS = ${LDFLAGS} -lncursesw -ltinfow

SRC = ${NAME}.c
MAN1 = ${NAME}.1
BIN = ${NAME}
OBJ = ${SRC:.c=.o}

all: catpoint

options:
	@echo ${NAME} build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

config.h:
	cp config.def.h config.h

.c.o:
	${CC} -c ${CATPOINT_CFLAGS} ${CATPOINT_CPPFLAGS} $<

${OBJ}:

catpoint: ${OBJ}
	${CC} -o $@ ${OBJ} ${CATPOINT_LDFLAGS}

clean:
	rm -f ${BIN} ${OBJ} ${NAME}-${VERSION}.tar.gz

dist:
	mkdir -p ${NAME}-${VERSION}
	cp -R LICENSE Makefile README.md TOOLS PATH-MAX-LIMIT.md ${SRC} \
		${MAN1} showoff ${NAME}-${VERSION}
	tar -cf - "${NAME}-${VERSION}" | \
		gzip -c > ${NAME}-${VERSION}.tar.gz
	rm -rf ${NAME}-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${BIN} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/${BIN}
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	cp -f ${MAN1} ${DESTDIR}${MANPREFIX}/man1/${MAN1}
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/${MAN1}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${BIN}
	rm -f ${DESTDIR}${MANPREFIX}/man1/${MAN1}

.PHONY: all options clean dist install uninstall

