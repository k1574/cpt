/* $Id: catpoint.c,v 1.2 2013/03/28 12:00:48 lostd Exp $ */

#include <sys/types.h>
#include <sys/mman.h>

#include <err.h>
#include <curses.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

int
main(int argc, char *argv[])
{
	int c, i, fd;
	char **p; /* the slides */

	if (argc == 1)
		errx(1, "usage: %s file ...", argv[0]);
	argv++;
	argc--;

	setlocale(LC_ALL, "");

	p = calloc(argc, sizeof(char *));

	/* map files to mem */
	for (i = 0; argv[i] != NULL; i++) {
		fd = open(argv[i], O_RDONLY, 0);
		if (fd == -1)
			err(1, "open: %s", argv[i]);
		p[i] = mmap(NULL, 0x1000, PROT_READ, MAP_PRIVATE, fd, 0);
		if (p[i] == MAP_FAILED)
			err(1, "mmap");
		close(fd);
	}

	/* init curses */
	initscr();
	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	curs_set(FALSE); /* hide cursor */

	/* start */
	i = 0;
show:
	/* display slide */
	clear();
	printw("%s", p[i]);
again:
	c = getch();
	switch (c) {
	case 'q':
		break;
	/* next */
	case 'l':
	case 'j':
	case KEY_RIGHT:
	case KEY_DOWN:
	case KEY_NPAGE:
		if (i < argc - 1) {
			i++;
			goto show;
		}
		goto again;
	/* prev */
	case 'h':
	case 'k':
	case KEY_LEFT:
	case KEY_UP:
	case KEY_PPAGE:
		if (i > 0) {
			i--;
			goto show;
		}
		goto again;
	/* first */
	case 'u':
	case KEY_BEG:
	case KEY_HOME:
		i = 0;
		goto show;
	/* last */
	case 'i':
	case KEY_END:
		i = argc - 1;
		goto show;
	default:
		goto again;
	}

	/* unmap mem */
	for (i = 0; argv[i] != NULL; i++)
		munmap(p[i], 0x1000);

	endwin(); /* restore terminal */

	return (0);
}
