/* Console Minesweeper
 * ui.h
 * User interface */
#ifndef UI_H
#define UI_H

#include "grid.h"

/* Different text colors */
enum colors {
	RED = 1, BLUE, GREEN, CYAN, BLACK
};

/* Wrapper around mk_grid; queries
 * the user for settings.
 * Program should not be in curses mode */
struct grid *get_grid(void);

/* Inputs character from keyboard and
 * processes it */
void input(struct grid *grid);

/* Prints grid onto screen;
 * program should be in curses mode */
void draw(struct grid *grid, enum game_state state);

#endif
