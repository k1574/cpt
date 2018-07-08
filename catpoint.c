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
#include <signal.h>

char **p; /* the slides */
int n; /* the number of slides */

void 
cleanup(int s)
{
	int i;
	for (i = 0; i<n; i++)
		munmap(p[i], 0x1000);

	endwin(); /* restore terminal */
	exit(1);
}


void
setsignal()
{
	signal(SIGHUP, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGQUIT, cleanup);
	signal(SIGABRT, cleanup);
	signal(SIGKILL, cleanup);
	signal(SIGTERM, cleanup);
}


int
main(int argc, char *argv[])
{
	int c, i, fd;

	if (argc == 1)
		errx(1, "usage: %s file ...", argv[0]);
	argv++;
	argc--;

	setsignal();
	setlocale(LC_ALL, "");

	p = calloc(argc, sizeof(char *));
	n = argc;

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
	/* powerpoint remote presenter shortcuts */
	case 27:
	case KEY_F(5):
	/* end presentation */
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
	/* shortcut from powerpoint. Needed for remote presenters. */
	case '.':
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
		/* printf("key pressed = '%d'\n", c); */
		goto again;
	}

	/* unmap mem */
	cleanup(0);	

	return (0);
}
