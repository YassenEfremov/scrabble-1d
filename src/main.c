#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>
#include <signal.h>
#include <errno.h>

#include "libs/jRead.h"
#include "libs/trie.h"
#include "libs/dict_handling/dict_handling.h"
#include "libs/ui_util/ui_util.h"

#include "game_logic/game_logic.h"
#include "game_settings/game_settings.h"


/* ============================================================================================= */
/* Private functions */


/*
 * Read the game settings from the config file and update them in-game.
 */
static int get_settings(int* letters, int* rounds) {
	
	FILE *settings_json = fopen("../config/game_settings.json", "r");
	if(!settings_json){
		message_log("Error: game_settings.json missing! ");
		return 2;
	}

	char* json_string = strfcpy(settings_json);
	fclose(settings_json);
	
	*letters = jRead_int(json_string, "{'letters'", NULL);
	*rounds = jRead_int(json_string, "{'rounds'", NULL);
	
	free(json_string);
}


/*
 * Refresh the main menu screen. (use on resize of terminal)
 */
void refresh_main_menu(int rows, int cols, WINDOW *main_menu_win, int num_of_items) {

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, rows, cols);	// get the new dimentions of the main screen
	//printw("%d %d", rows, cols);	

	// If the size of the terminal is smaller than (39, 5) everything glitches out!				// TO DO
	mvwin(main_menu_win, rows/2 - num_of_items/2, cols/2 - 6);

	mvwin(title_win, main_menu_win->_begy - num_of_items - 3, cols/2 - 19);
	mvwin(msg_win, main_menu_win->_begy + num_of_items + 3, cols/2 - MSG_LEN/2);

	// Refresh the necessay elements
	refresh();
	wrefresh(title_win);
}


/* ============================================================================================= */
/* Public functions */


int main(int argc, char *argv[]) {

	int letters;
	int rounds;

	struct node_t *trie_root;

	// Start ncurses

	// Start main window
	initscr();

	// Colors
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);		// Menu title green
	init_pair(2, COLOR_WHITE, COLOR_BLACK);		// Menu background
	init_pair(99, COLOR_MAGENTA, COLOR_GREEN);	// For debugging

	// Other settings
	cbreak();//raw();
	noecho();
	keypad(stdscr, TRUE);	// The standard screen is used ONLY for reading input!
	curs_set(0);
	refresh();

	get_settings(&letters, &rounds);


	// Predefined data used by ncurses

	int rows, cols;
	getmaxyx(stdscr, rows, cols);	// get the dimentions of the main screen

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
	WINDOW *main_menu_win = newwin(num_of_items, 12, rows/2 - num_of_items/2, cols/2 - 6);
	MENU *main_menu = new_menu((ITEM **)items);

	set_menu_win(main_menu, main_menu_win);
	set_menu_sub(main_menu, main_menu_win);

	// Menu settings
	set_menu_mark(main_menu, "> ");
	set_menu_fore(main_menu, A_BOLD);


	/* --------------------------------------------------------------------------------------------- */
	/* Title */

	// Title window (defined globally!)
	title_win = newwin(4, 38, main_menu_win->_begy - 4 - 3, cols/2 - 19);

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
	//mvaddstr(rows/3 + 1, cols/2 - strlen(mline)/2, mline);	// centered
	wattroff(title_win, A_BOLD);


	/* --------------------------------------------------------------------------------------------- */
	/* Message window */

	// This window is used to display messages in-game (defined globally!)
	msg_win = newwin(1, MSG_LEN, main_menu_win->_begy + num_of_items + 3, cols/2 - MSG_LEN/2);

	// Navigation guide message
	char *nav_guide_msg = "(Use arrow keys to navigate)";
	char *soon_msg = "Coming soon!";
	message_log(nav_guide_msg);


	/* --------------------------------------------------------------------------------------------- */


	// Post the menu
	post_menu(main_menu);

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

			case 10:
				// Enter key
				werase(msg_win);
				curr_item_index = item_index(curr_item);

				switch(curr_item_index) {
					case 0:
						// Start a game
						//startGame(letters, rounds);
						message_log(soon_msg);
						break;

					case 1:
						// Open game settings
						unpost_menu(main_menu);		// hide the main menu
						wrefresh(main_menu_win);
						get_settings(&letters, &rounds);
						gameSettings(&letters, &rounds);
						post_menu(main_menu);		// unhide the main menu
						break;

					case 2:
						// Add a word to the dictionary
						//addWordToDict();
						message_log(soon_msg);
						break;

					case 3:
						// Exit the app
						exitMenu(&main_menu, &items, num_of_items);
						curs_set(1);

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
				refresh_main_menu(rows, cols, main_menu_win, num_of_items);
				break;
		}

		// Refresh everything
		wrefresh(main_menu_win);
		wrefresh(msg_win);

	}while(1);

	return 0;
}
