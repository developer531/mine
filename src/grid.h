/* Console Minesweeper
 * grid.h
 * Grid structure and associated functions */
#ifndef GRID_H
#define GRID_H

#include <stdbool.h>

enum game_state { /* State of the game */
	WIN, LOSS, CONTINUE
};

enum cell_state { /* States of a cell */
	COVERED, UNCOVERED, FLAGGED
};

enum direction { /* Directions to move */
	LEFT, RIGHT, UP, DOWN
};

struct cell { /* Single cell */
	bool is_mine; /* False=empty; True=mine */
	enum cell_state state; /* Covered, uncovered, or flagged */
};

struct grid { /* Game grid */
	int wd, ht; /* Width and height */
	int x, y; /* Position of cursor */
	int nmines; /* Number of mines */
	int nempty; /* Number of empty tiles */
	int nflags; /* Number of flags placed */
	int move; /* Move number */
	struct cell **pcells; /* Cell data */
};

struct grid *mk_grid(int wd, int ht, int nmines); /* Makes new grid (Aborts on failure) */
void del_grid(struct grid *grid); /* Frees grid from memory */

int neighbors(struct grid *grid, int x, int y); /* Returns number of neighbors that are bombs */

void grid_move(struct grid *grid, enum direction direction); /* Moves the cursor left/right/up/down */
void reveal(struct grid *grid, int x, int y); /* Reveals tile */
void flag(struct grid *grid, int x, int y); /* Toggles flag on tile */

enum game_state eval(struct grid *grid); /* Evaluates board for win/loss conditions */

#endif
