#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ncurses.h>
#include <menu.h>

#include "libs/trie.h"
#include "libs/dict_handling/dict_handling.h"
#include "libs/ui_util/ui_util.h"

#include "game_logic/game_logic.h"
#include "game_settings/game_settings.h"


/* ============================================================================================= */
/* Private functions */


/* Read the game settings from the config file and update them in-game. */
static int get_settings(int *letters, int *rounds) {

	GKeyFile *game_settings;
	GKeyFileFlags conf_flags;
	GError *conf_error = NULL;

	game_settings = g_key_file_new();
	conf_flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;	// set those 2 flags

	// Open config file and catch any errors			TEMPORARY LOCATION
    if(!g_key_file_load_from_file(game_settings, "../config/game_settings.cfg", conf_flags, &conf_error)) {
    	g_error("%s", conf_error->message);
      	return 2;
    }

	*letters = g_key_file_get_integer(game_settings, "Settings", "letters", &conf_error);
	*rounds = g_key_file_get_integer(game_settings, "Settings", "rounds", &conf_error);

	g_key_file_free(game_settings);

	return 0;
}


/* --------------------------------------------------------------------------------------------- */


/* Refresh the main menu screen. (use on resize of terminal) */
static void refresh_main_menu(WINDOW *main_menu_win, int num_of_items) {

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, term_rows, term_cols);	// get the new dimentions of the main screen
	//printw("%d %d", term_rows/2 - num_of_items/2, term_cols/2 - 6);	

	// If the size of the terminal is smaller than [39, 5] everything glitches out!				// TO DO
	mvwin(main_menu_win, term_rows/2 - num_of_items/2, term_cols/2 - 6);
	mvwin(title_win, main_menu_win->_begy - 4 - 3, term_cols/2 - 19);
	mvwin(msg_win, main_menu_win->_begy + num_of_items + 3, term_cols/2 - MSG_LEN/2);

/*	WHEN THE SCREEN IS TOO SMALL SOME ELEMENTS DON'T APPEAR									// TO DO
	if(term_rows < 18) {
		mvwin(main_menu_win, term_rows/2 - num_of_items/2 + 18/term_rows, term_cols/2 - 6);
		mvwin(title_win, main_menu_win->_begy - 4 - 3, term_cols/2 - 19);
	}
*/
	// Refresh the necessay elements
	refresh();
	wrefresh(title_win);
}


/* ============================================================================================= */
/* Public functions */


int main(int argc, const char *argv[]) {

	// Program variables

	int letters;
	int rounds;
	int to_clear = 1;

	struct node_t *trie_root;


	/* Start ncurses */

	// Start main window
	initscr();

	// Colors
	if(has_colors() == FALSE) {
		endwin();
		printf("Error: Your terminal doesn't support colors!\n");
		exit(EXIT_SUCCESS);
	}
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);		// Menu title green
	init_pair(2, COLOR_WHITE, COLOR_BLACK);		// Menu background
	init_pair(3, COLOR_CYAN, COLOR_BLACK);		// Final round blue
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);	// Game over yellow
	init_pair(5, COLOR_RED, COLOR_BLACK);		// Invalid input red
	init_pair(99, COLOR_BLACK, COLOR_GREEN);	// For debugging

	// Other settings
	cbreak();//raw();
	noecho();
	keypad(stdscr, TRUE);	// The standard screen is used ONLY for reading input!
	curs_set(0);
	refresh();

	getmaxyx(stdscr, term_rows, term_cols);	// get the dimentions of the main screen


	// Predefined data

	char *items_list[] = {
		"New Game",
		"Settings",
		"Add word",
		"Exit"
	};
	int num_of_items = sizeof(items_list)/sizeof(items_list[0]);

	ITEM *curr_item;
	int key;
	int curr_item_index;


	/* --------------------------------------------------------------------------------------------- */
	/* Main menu */

	// Options
	ITEM **items = (ITEM **)calloc(num_of_items + 1, sizeof(ITEM *));
	for(int i = 0; i < num_of_items; i++) {
		items[i] = new_item(items_list[i], "");		// Name each option
	}
	items[num_of_items] = NULL;

	// Menu
	WINDOW *main_menu_win = newwin(num_of_items, 12, term_rows/2 - num_of_items/2, term_cols/2 - 6);
	MENU *main_menu = new_menu((ITEM **)items);
	set_menu_win(main_menu, main_menu_win);
	set_menu_sub(main_menu, main_menu_win);

	// Menu settings
	set_menu_mark(main_menu, "> ");
	set_menu_fore(main_menu, A_BOLD);


	/* --------------------------------------------------------------------------------------------- */
	/* Title */

	// Title window (defined globally!)
	title_win = newwin(4, 38, main_menu_win->_begy - 4 - 3, term_cols/2 - 19);

	char *mtitle =
		" ____ ____ ___    _  __  __   __  ____"
		"/ __//  _/| _ \\  / || _\\| _\\ / / | __/"
		"\\__ \\| (_ | ` / / ^|| _(| _(/ /_ | _|"
    	" /___/\\___||_|\\_/_/_||__/|__/|___/|___/"
		;
	//char *mline = "------------------------";

	wattron(title_win, A_BOLD);
	wattron(title_win, COLOR_PAIR(1));
	mvwaddstr(title_win, 0, 0, mtitle);	// centered
	wattroff(title_win, COLOR_PAIR(1));
	//mvaddstr(term_rows/3 + 1, term_cols/2 - strlen(mline)/2, mline);	// centered
	wattroff(title_win, A_BOLD);


	/* --------------------------------------------------------------------------------------------- */
	/* Message window */

	// This window is used to display messages in-game (defined globally!)
	msg_win = newwin(1, MSG_LEN, main_menu_win->_begy + num_of_items + 3, term_cols/2 - MSG_LEN/2);

	// Navigation guide message
	char *nav_guide_msg = "(Use arrow keys to navigate)";
	char *soon_msg = "Coming soon!";

	// Line
	//move(msg_win->_begy - 1, msg_win->_begx);
	//hline(ACS_HLINE, MSG_LEN);


	/* --------------------------------------------------------------------------------------------- */


	// Post the menu
	post_menu(main_menu);

	message_log(nav_guide_msg);

	// Refresh everything
	wrefresh(main_menu_win);
	wrefresh(title_win);
	wrefresh(msg_win);


	/* --------------------------------------------------------------------------------------------- */
	/* Navigation */

	do {
		key = getch();	// read input from the stdscr
		curr_item = current_item(main_menu);

		switch(key) {
			// Arrow keys

			case KEY_DOWN:
				werase(msg_win);
				menu_driver(main_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				werase(msg_win);
				menu_driver(main_menu, REQ_UP_ITEM);
				break;

			case 10:  // Enter key
				werase(msg_win);
				curr_item_index = item_index(curr_item);
				get_settings(&letters, &rounds);	// refresh settings

				switch(curr_item_index) {
					// Select the current item

					case 0:
						/* ------------- Start game ------------- */

						unpost_menu(main_menu);  		  // hide the main menu
						wrefresh(main_menu_win);
						startGame(letters, rounds);
						post_menu(main_menu);  			  // unhide the main menu
						break;

					case 1:
						/* --------- Open game settings --------- */

						unpost_menu(main_menu);  		  // hide the main menu
						wrefresh(main_menu_win);
						gameSettings(&letters, &rounds);
						post_menu(main_menu);  			  // unhide the main menu
						break;

					case 2:
						/* ---- Add a word to the dictionary ---- */

						//addWordToDict();
						message_log(soon_msg);
						break;

					case 3:
						/* ------------ Exit the app ------------ */

						exitMenu(&main_menu, &items, num_of_items);
						delwin(title_win);
						delwin(main_menu_win);
						delwin(msg_win);

						endwin();
						exit(EXIT_SUCCESS);
				}

				/*
				 * The break statemant is omitted here because we want to refresh everything
				 * after we have returned from the selected action (but NOT after we have used the arrow keys!)
				 */

			case KEY_RESIZE:
				// On window resize
				refresh_main_menu(main_menu_win, num_of_items);
				break;
		}

		// Refresh everything
		wrefresh(main_menu_win);
		wrefresh(msg_win);

	}while(1);

	return 0;
}
