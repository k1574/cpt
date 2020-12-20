/* See LICENSE file for license details. */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <err.h>
#include <curses.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>

char **slides, **slidefiles; /* the slides */
int nslides, currentslide;

void
cleanup(int sig)
{
	int i;

	for (i = 0; i < nslides; i++)
		munmap(slides[i], 0x1000);

	endwin(); /* restore terminal */
	exit(1);
}

void
reload(char **argv, int i)
{
	struct stat statbuf;
	int fd;

	if (slides[i] != NULL) {
		if (munmap(slides[i], 0x1000) < 0)
			err(1, "munmap: %s", slidefiles[i]);
	}

	fd = open(slidefiles[i], O_RDONLY, 0);
	if (fd < 0)
		err(1, "open: %s", slidefiles[i]);
	if (fstat(fd, &statbuf) < 0)
		err(1, "fstat: %s", slidefiles[i]);
	slides[i] = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (slides[i] == MAP_FAILED)
		err(1, "mmap");
	close(fd);
}

void
reloadcurrentslide(int sig)
{
	reload(slidefiles, currentslide);

	if (sig == SIGHUP) {
		clear();
		refresh();
		printw("%s", slides[currentslide]);
	}
}

void
setsignal()
{
	signal(SIGHUP, reloadcurrentslide);

	signal(SIGINT, cleanup);
	signal(SIGQUIT, cleanup);
	signal(SIGABRT, cleanup);
	signal(SIGKILL, cleanup);
	signal(SIGTERM, cleanup);
}

int
main(int argc, char *argv[])
{
	int c;

	if (argc == 1)
		errx(1, "usage: %s file ...", argv[0]);
	slidefiles = ++argv;
	nslides = --argc;

	setsignal();
	setlocale(LC_ALL, "");

	slides = calloc(nslides, sizeof(char *));

	/* map files to mem */
	for (currentslide = 0; slidefiles[currentslide] != NULL;
			currentslide++) {
		reload(slidefiles, currentslide);
	}
	/* start */
	currentslide = 0;

	/* init curses */
	initscr();
	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	curs_set(FALSE); /* hide cursor */

show:
	/* display slide */
	clear();
	refresh();
	printw("%s", slides[currentslide]);

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
		if (currentslide < nslides - 1) {
			currentslide++;
			goto show;
		}
		goto again;
	/* prev */
	case 'h':
	case 'k':
	case KEY_LEFT:
	case KEY_UP:
	case KEY_PPAGE:
		if (currentslide > 0) {
			currentslide--;
			goto show;
		}
		goto again;
	/* shortcut from powerpoint. Needed for remote presenters. */
	case '.':
	/* first */
	case 'u':
	case KEY_BEG:
	case KEY_HOME:
		currentslide = 0;
		goto show;
	/* last */
	case 'i':
	case KEY_END:
		currentslide = nslides - 1;
		goto show;
	/* reload */
	case 'r':
		reloadcurrentslide(0);
		goto show;
	default:
		/* printf("key pressed = '%d'\n", c); */
		goto again;
	}

	/* unmap mem */
	cleanup(0);

	return 0;
}

