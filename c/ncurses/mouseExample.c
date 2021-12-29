#include <ncurses.h>
#include <string.h>
#include <stdio.h>

#define WIDTH 30
#define HEIGHT 10

int startx = 0, starty = 0;

char *choices[] = {
	"Choice 1",
	"Choice 2",
	"Choice 3",
	"Choice 4",
	"Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW *menu_win, int highlight);
void report_choice(int mouse_x, int mouse_y, int *p_choice);

int main(int argc, char *argv[])
{
	WINDOW *menu_win;
	MEVENT event;
	int choice = 0, c;

	/* initialize ncurses */
	initscr();
	clear();
	noecho();
	cbreak();

	startx = (80 - WIDTH) / 2;
	starty = (24 - HEIGHT) / 2;

	attron(A_REVERSE);
	mvprintw(23, 1, "Click on Exit to quit (works best in a virtual console)\n");
	refresh();
	attroff(A_REVERSE);

	/* Print the menu for the 1st time */
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	print_menu(menu_win, 1);
	/* Get all the mouse events */
	mousemask(ALL_MOUSE_EVENTS, NULL);
	
	while(1) {
		c = wgetch(menu_win);
		switch(c) {
			case KEY_MOUSE:
				if (getmouse(&event) == OK) {
					/* When the user clicks left mouse button */
					if (event.bstate && BUTTON1_PRESSED) {
						report_choice(event.x + 1, event.y + 1, &choice);
						if (choice == -1) // Exit was chosen
							goto end;
						mvprintw(22, 1, "Choice is: %d String chosen: \"%10s\"", choice, choices[choice - 1]);
						refresh();
					}
				}
				print_menu(menu_win, choice);
				break;
		}
	}
end:	
	endwin();
	return 0;
}

void print_menu(WINDOW *menu_win, int highlight)
{
	int x = 2, y = 2, i;

	box(menu_win, 0, 0);
	for (i = 0; i < n_choices; ++i)
	{
		if (highlight == i + 1) { /* highlight the present choice */
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		} else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

/* Report the choice according to mouse position */
void report_choice(int mouse_x, int mouse_y, int *p_choice)
{
	int i = startx + 2, j = starty + 3, choice;

	for (choice = 0; choice < n_choices; ++choice) {
		if (mouse_y == j + choice && mouse_x >= i && mouse_x <= i + strlen(choices[choice]))
		{
			if (choice == n_choices - 1)
				*p_choice = -1;
			else
				*p_choice = choice + 1;
			break;
		}
	}
}
