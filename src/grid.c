/* Console Minesweeper
 * grid.c
 * Grid structure and associated functions */ 
#include <stdlib.h>
#include "grid.h"

/* Wrapper around calloc that aborts on failure */
static void *xcalloc(size_t nelem, size_t elsize)
{
	void *ptr = calloc(nelem, elsize); /* Allocate memory */
	if (!ptr) /* Abort on failure */
		exit(1);
	return ptr; /* Successful allocation */
}

/* Makes new grid (Aborts on failure) */
struct grid *mk_grid(int wd, int ht, int nmines)
{
	/* Allocates grid structure and cell data; abort on failure */
	struct grid *grid = xcalloc(1, sizeof(struct grid));
	struct cell **pcells = xcalloc(wd, sizeof(struct cell *)); /* Allocate column pointers */

	for (int i = 0; i < wd; ++i) {
		struct cell *pcol = xcalloc(ht, sizeof(struct cell)); /* Allocate column */
		pcells[i] = pcol; /* Assign to column pointer */
	}

	/* Setup grid struct */
	grid->wd = wd;
	grid->ht = ht;
	grid->x = 0;
	grid->y = 0;
	grid->nmines = nmines;
	grid->nempty = (wd * ht) - nmines;
	grid->nflags = 0;
	grid->move = 0;
	grid->pcells = pcells;

	/* Place bombs */
	for (int i = 0; i < nmines; ++i) {
		int row = rand() % wd;
		int col = rand() % ht;

		/* Bomb is already at specified cell */
		while (grid->pcells[row][col].is_mine) {
			row = rand() % wd;
			col = rand() % ht;
		}

		/* Modify cell state */
		grid->pcells[row][col].is_mine = true;
	}

	return grid;
}

/* Frees grid from memory */
void del_grid(struct grid *grid)
{
	free(grid->pcells); /* Free cell data and metadata */
	free(grid);
}

/* Returns number of neighbors that are bombs */
int neighbors(struct grid *grid, int x, int y)
{
	int count = 0;

	for (int i = -1; i <= 1; ++i) { /* East/West */
		for (int j = -1; j <= 1; ++j) { /* North/South */
			if (i == 0 && j == 0) /* Cell itself */
				continue;
			if (y + i < 0 || y + i >= grid->wd
				|| x + j < 0 || x + j >= grid->ht) /* Edge cases */
				continue;
			if (grid->pcells[y + i][x + j].is_mine) /* Check for bomb */
				count++;
		}
	}

	return count;
}

/* Moves the cursor left/right/up/down */
void grid_move(struct grid *grid, enum direction direction)
{
	switch (direction) {
	case LEFT:
		if (grid->x > 0) /* Do not veer offscreen */
			grid->x--;
		break;
	case RIGHT:
		if (grid->x < grid->ht - 1)
			grid->x++;
		break;
	case UP:
		if (grid->y > 0)
			grid->y--;
		break;
	case DOWN:
		if (grid->y < grid->wd - 1)
			grid->y++;
		break;
	}
}

/* Reveals tile */
void reveal(struct grid *grid, int x, int y)
{
	/* Prevent flagged cells from being revealed */
	if (grid->pcells[y][x].state == FLAGGED)
		return;
	if (grid->pcells[y][x].is_mine 
		&& grid->move == 0 && grid->nempty > 0) { /* Prevent mine on first move */
		int new_x = rand() % grid->wd;
		int new_y = rand() % grid->ht;

		/* Find empty replacement mine */
		while (grid->pcells[new_y][new_x].is_mine) {
			new_x = rand() % grid->wd;
			new_y = rand() % grid->ht;
		}

		/* Move mine to different cell */
		grid->pcells[y][x].is_mine = false;
		grid->pcells[new_y][new_x].is_mine = true;
	}
	
	grid->pcells[y][x].state = UNCOVERED;

	if (!grid->pcells[y][x].is_mine) { /* Cell was not a mine */
		if (neighbors(grid, x, y) == 0) { /* No nearby mines; reveal subsequent tiles */
			for (int i = -1; i <= 1; ++i) { /* East/West */
				for (int j = -1; j <= 1; ++j) { /* North/South */
					if (i == 0 && j == 0) /* Cell itself */
						continue;
					if (y + i < 0 || y + i >= grid->wd
						|| x + j < 0 || x + j >= grid->ht) /* Ensure index is valid before read */
						continue;
					if (grid->pcells[y + i][x + j].state == COVERED)
						reveal(grid, x + j, y + i); /* Reveal subsequent tiles */
				}
			}
		}
	}

	grid->move++; /* Increment move */
}

/* Toggles flag on tile */
void flag(struct grid *grid, int x, int y)
{
	switch (grid->pcells[y][x].state) {
	case COVERED: /* Flag the tile */
		grid->pcells[y][x].state = FLAGGED;
		grid->nflags++;
		break;
	case FLAGGED: /* Unflag the tile */
		grid->pcells[y][x].state = COVERED;
		grid->nflags--;
		break;
	default: /* Uncovered; ignore */
		break;
	}
}

/* Evaluates board for win/loss conditions */
enum game_state eval(struct grid *grid)
{
	/* Number of uncovered, empty tiles.
	 * Win triggers if nuncovered = nempty */
	int nuncovered = 0;

	for (int i = 0; i < grid->wd; ++i) {
		for (int j = 0; j < grid->ht; ++j) {
			if (grid->pcells[i][j].state == UNCOVERED) {
				if (grid->pcells[i][j].is_mine) /* Bomb uncovered */
					return LOSS;
				else
					nuncovered++;
			}
		}
	}

	if (nuncovered == grid->nempty && grid->nempty > 0) /* All empty tiles uncovered */
		return WIN;
	
	/* Game not finished */
	return CONTINUE;
}
