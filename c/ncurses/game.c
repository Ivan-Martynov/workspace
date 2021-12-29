#include <ncurses.h>

typedef struct _win_border_struct {
	chtype ls, rs, ts, bs,
		   tl, tr, bl, br;
}WIN_BORDER;

typedef struct _WIN_struct {
	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;

void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win);
void create_box(WIN *win, bool flag);

int main(int argc, char *argv[])
{
	WIN win;
	int ch;

	/* initialize ncurses */
	initscr();
	cbreak(); /* disable line buffering */
	keypad(stdscr, TRUE); /* access to special keys */
	noecho();
	curs_set(FALSE);

	start_color(); /* start colour functionality */
	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	/* initialize window params */
	init_win_params(&win);
	print_win_params(&win);

	attron(COLOR_PAIR(1));

	create_box(&win, TRUE);

	while ((ch = getch()) != KEY_F(1)) {
		switch(ch) {
			case KEY_LEFT:
				if (win.startx > 0) {
					create_box(&win, FALSE);
					--win.startx;	
					create_box(&win, TRUE);
				}
				break;
			case KEY_RIGHT:
				if (win.startx < COLS - 1) {
					create_box(&win, FALSE);
					++win.startx;	
					create_box(&win, TRUE);
				}
				break;
			case KEY_UP:
				if (win.starty > 0) {
					create_box(&win, FALSE);
					--win.starty;	
					create_box(&win, TRUE);
				}
				break;
			case KEY_DOWN:
				if (win.starty < LINES - 1) {
					create_box(&win, FALSE);
					++win.starty;	
					create_box(&win, TRUE);
				}
				break;
		}
	}
	endwin();

	return 0;
}

void init_win_params(WIN *p_win)
{
	p_win->height = 3;
	p_win->width = 10;
	p_win->starty = (LINES - p_win->height) / 2;
	p_win->startx = (COLS - p_win->width) / 2;

	p_win->border.ls = '|';
	p_win->border.rs = '|';
	p_win->border.ts = '-';
	p_win->border.bs = '-';
	p_win->border.tl = '+';
	p_win->border.tr = '+';
	p_win->border.bl = '+';
	p_win->border.br = '+';
}

void print_win_params(WIN *p_win)
{
#ifdef _DEBUG
	mvprintw(25, 0, "%d %d %d %d", p_win->startx, p_win->starty, p_win->width, p_win->height);
	refresh();
#endif
}

void create_box(WIN *p_win, bool flag)
{
	int x, y;

	x = p_win->startx;
	y = p_win->starty;

	if (flag == TRUE)
		mvwaddch(stdscr, y, x, ACS_DIAMOND);
	else
		mvaddch(y, x, ' ');
	refresh();
}
