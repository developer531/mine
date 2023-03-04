/* Console Minesweeper
 * ui.c
 * User interface */
#include <stdio.h>
#include <ncurses.h>
#include "grid.h"
#include "ui.h"

#define W 87 /* Keycodes */
#define w 119
#define A 65
#define a 97
#define S 83
#define s 115
#define D 68
#define d 100

#define F 70
#define f 102
#define ENTER 10

/* Prints text in specified color */
#define COLOR_TEXT(str, color) \
	attron(COLOR_PAIR(color)); \
	printw(str); \
	attroff(COLOR_PAIR(color));

/* Wrapper around mk_grid; queries
 * the user for settings.
 * Program should not be in curses mode */
struct grid *get_grid(void)
{
	/* Print menu and move to starter position */
	printw("Console Minesweeper v1.0\n");
	printw("Created by Gavin Mutchler\n\n");
	printw("Enter difficulty:\n");
	printw("-> Beginner\n");
	printw("   Intermediate\n");
	printw("   Expert\n");
	printw("   Custom");
	move(4, 3);

	int cursor_pos = 0; /* Cursor position (accounting for menu) */

	int ch;
	while ((ch = getch()) != ENTER) { /* Wait for user to enter difficulty */
		erase(); /* Clear screen (Do not refresh) */

		/* Print menu */
		printw("Console Minesweeper v1.0\n");
		printw("Created by Gavin Mutchler\n\n");
		printw("Enter difficulty:\n");
		printw("   Beginner\n");
		printw("   Intermediate\n");
		printw("   Expert\n");
		printw("   Custom");

		/* Interpret key */
		switch (ch) { /* Only account for vertical keys (W & S) */
		case W:
		case w:
			cursor_pos--; /* Move up */
			break;
		case S:
		case s:
			cursor_pos++; /* Move down */
			break;
		}

		/* Calculate proper cursor position */
		if (cursor_pos < 0)
			cursor_pos = 0;
		else if (cursor_pos > 3)
			cursor_pos = 3;
		int pos = cursor_pos + 4;

		move(pos, 0); /* Move to start of line */
		printw("-> "); /* Print arrow key */
	}

	switch (cursor_pos) {
	case 0: /* Beginner */
		return mk_grid(10, 10, 10);
		break;
	case 1: /* Intermediate */
		return mk_grid(16, 16, 40);
		break;
	case 2: /* Expert */
		return mk_grid(16, 30, 99);
		break;
	}

	/* Custom */
	erase(); /* Clear screen to get rid of menu */

	printw("Enter width (1-100): "); /* Input width */
	int wd;
	scanw("%d", &wd);

	while (wd < 1 || wd > 100) {
		printw("Enter width (1-100): ");
		scanw("%d", &wd);
	}

	printw("Enter height (1-100): "); /* Input height */
	int ht;
	scanw("%d", &ht);

	while (ht < 1 || ht > 100) {
		printw("Enter height (1-100): ");
		scanw("%d", &ht);
	}

	printw("Enter # of mines (1-%d): ", wd * ht); /* Input number of mines */
	int nmines;
	scanw("%d", &nmines);

	while (nmines < 1 || nmines > wd * ht) {
		printw("Enter # of mines (1-%d): ", wd * ht);
		scanw("%d", &nmines);
	}

	return mk_grid(wd, ht, nmines);
}

/* Inputs character from keyboard and
 * processes it */
void input(struct grid *grid)
{
	int ch = getch(); /* Get singular character from keyboard */

	switch (ch) {
	case W:
	case w:
		grid_move(grid, UP);
		break;
	case A:
	case a:
		grid_move(grid, LEFT);
		break;
	case S:
	case s:
		grid_move(grid, DOWN);
		break;
	case D:
	case d:
		grid_move(grid, RIGHT);
		break;
	case F:
	case f:
		flag(grid, grid->x, grid->y);
		break;
	case ENTER:
		reveal(grid, grid->x, grid->y);
		break;
	}
}

/* Prints grid onto screen;
 * program should be in curses mode */
void draw(struct grid *grid, enum game_state state)
{
	move(0, 0); /* Move cursor to top-left corner */
	erase(); /* Clear screen */

	/* Draw top box */
	for (int i = 0; i < grid->ht * 2 + 3; ++i) {
		printw("#");
	}
	printw("\n"); /* Start new line */

	/* Print every cell */
	for (int i = 0; i < grid->wd; ++i) {
		printw("# "); /* Left side of box */

		for (int j = 0; j < grid->ht; ++j) {
			switch (grid->pcells[i][j].state) {
			case COVERED:
				printw("- ");
				break;
			case UNCOVERED:
				if (grid->pcells[i][j].is_mine) {
					COLOR_TEXT("X ", RED);
				} else {
					switch (neighbors(grid, j, i)) {
					case 0: /* Blank */
						printw("  ");
						break;
					case 1: /* Blue 1 */
						COLOR_TEXT("1 ", BLUE);
						break;
					case 2: /* Green 2 */
						COLOR_TEXT("2 ", GREEN);
						break;
					case 3: /* Red 3 */
						COLOR_TEXT("3 ", RED);
						break;
					case 4: /* Blue 4 */
						COLOR_TEXT("4 ", BLUE);
						break;
					case 5: /* Red 5 */
						COLOR_TEXT("5 ", RED);
						break;
					case 6: /* Cyan 6 */
						COLOR_TEXT("6 ", CYAN);
						break;
					case 7: /* Black 7 */
						COLOR_TEXT("7", BLACK);
						printw(" ");
						break;
					case 8: /* White 8 */
						printw("8 ");
						break;
					}
				}
				break;
			case FLAGGED:
				COLOR_TEXT("P ", RED);
				break;
			}
		}

		printw("#"); /* Right side of box */
		printw("\n"); /* End of row; start a new line */
	}

	/* Draw bottom box */
	for (int i = 0; i < grid->ht * 2 + 3; ++i) {
		printw("#");
	}

	if (state == CONTINUE) { /* Game is not finished; print mines remaining */
		/* Draw number of mines remaining */
		int rem = grid->nmines - grid->nflags;
		printw("\n\nMines remaining: %d", (rem > 0) ? rem : 0);

		/* Move cursor back to original position and apply changes */
		move(grid->y + 1, grid->x * 2 + 2); /* Account for surrounding box and spacing */
	} else {
		if (state == WIN)
			printw("\n\nYOU WIN !!!");
		else
			printw("\n\nYOU LOSE");
	}

	refresh();
}
