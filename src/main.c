/* Console Minesweeper
 * main.c
 * Main entry point */
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "grid.h"
#include "ui.h"

/* Completes one instance of the game */
void play_game(void)
{
	/* Make new grid based on user settings */
	struct grid *grid = get_grid();

	/* Continue until end condition is reached */
	while (eval(grid) == CONTINUE) {
		draw(grid, CONTINUE); /* Print grid onto screen */
		input(grid); /* Handle keypress */
	}

	/* Display final board and print win/loss message */
	draw(grid, eval(grid));

	del_grid(grid); /* Free grid from memory */
}

int main(void)
{
	srand(time(NULL)); /* Setup rand() */
	initscr(); /* Enter into curses mode */

	/* Setup colors */
	start_color();
	init_pair(RED, COLOR_RED, COLOR_BLACK); /* Red text on black screen */
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK); /* Blue text on black screen */
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK); /* Green text on black screen */
	init_pair(CYAN, COLOR_CYAN, COLOR_BLACK); /* Cyan text on black screen */
	init_pair(BLACK, COLOR_BLACK, COLOR_WHITE); /* Black text on white screen */

	/* Complete one instance of the game */
	play_game();

	/* Prompt to play again */
	printw("\nPlay again? (y/n): ");
	char play_again;
	scanw(" %c", &play_again);

	while (play_again == 'y' || play_again == 'Y') {
		clear(); /* Clear screen */
		play_game(); /* Complete another instance */

		/* Prompt to play again */
		printw("\nPlay again? (y/n): ");
		scanw(" %c", &play_again);
	}
	
	endwin(); /* Leave curses mode and exit gracefully */
}
