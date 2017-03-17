# catpoint - simple presentation software
# See LICENSE file for copyright and license details.

include config.mk

SRC = catpoint.c
OBJ = ${SRC:.c=.o}

all: options catpoint

options:
	@echo catpoint build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

config.h:
	cp config.def.h config.h

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

catpoint: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f catpoint ${OBJ} catpoint-${VERSION}.tar.gz

dicatpoint: clean
	@echo creating dist tarball
	@mkdir -p catpoint-${VERSION}
	@cp -R LICENSE Makefile README.md config.mk ${SRC} catpoint-${VERSION}
	@tar -cf catpoint-${VERSION}.tar catpoint-${VERSION}
	@gzip catpoint-${VERSION}.tar
	@rm -rf catpoint-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f catpoint ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/catpoint
	@#echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@#mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@#sed "s/VERSION/${VERSION}/g" < catpoint.1 > ${DESTDIR}${MANPREFIX}/man1/catpoint.1
	@#chmod 644 ${DESTDIR}${MANPREFIX}/man1/catpoint.1

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/catpoint
	#@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	#@rm -f ${DESTDIR}${MANPREFIX}/man1/catpoint.1

.PHONY: all options clean dist install uninstall

