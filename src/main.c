/*
 *	Scrabble 1D - main program
 *
 *  Copyright (C) 2021 Yassen Efremov
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "game_logic/game_logic.h"
#include "game_settings/game_settings.h"
#include "add_new_word/add_new_word.h"

#include "libs/file_paths.h"
#include "libs/file_checks.h"
#include "libs/ui_util/ui_util.h"

#include <glib.h>
#include <ncurses.h>
#include <menu.h>

#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/* ============================================================================================= */
/* Private functions */


/* Refresh the main menu screen. (use on resize of terminal) */
static void refresh_main_menu(WINDOW *main_menu_win, int num_of_items) {

	// If the size of the terminal is smaller than (39, 5) everything glitches out!				// TO DO

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, term_rows, term_cols);	// get the new dimentions of the main screen

	mvwin(main_menu_win, term_rows/2 - num_of_items/2, term_cols/2 - 6);
	mvwin(title_win, main_menu_win->_begy - 4 - 3, term_cols/2 - 19);
	mvwin(msg_win, main_menu_win->_begy + num_of_items + 3, term_cols/2 - MSG_LEN/2);

/*	WHEN THE SCREEN IS TOO SMALL SOME ELEMENTS DON'T APPEAR									// TO DO
	if(term_rows < 18) {
		mvwin(main_menu_win, term_rows/2 - num_of_items/2 + 18/term_rows, term_cols/2 - 6);
		mvwin(title_win, main_menu_win->_begy - 4 - 3, term_cols/2 - 19);
	}
*/
	// Refresh the necessary elements
	refresh();
	wrefresh(title_win);
}


/* ============================================================================================= */
/* Public functions */


int main(int argc, const char *argv[]) {

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
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);	// "Amazing!" magenta
	init_pair(99, COLOR_BLACK, COLOR_GREEN);	// For debugging

	// Other settings
	raw();
	noecho();
	keypad(stdscr, TRUE);	// The standard screen is used ONLY for reading input!
	curs_set(0);
	refresh();

	getmaxyx(stdscr, term_rows, term_cols);	// get the dimentions of the terminal


	// Variables

	int letters, rounds;	// The in-game letters and rounds variables!

	char *items_list[] = {
		"New Game",
		"Settings",
		"Add word",
		"Exit"
	};
	int num_of_items = sizeof(items_list)/sizeof(items_list[0]);

	int key;
	ITEM *curr_item;
	int curr_item_index;

	int conf_err = 0, dict_err = 0, json_err = 0;


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
	title_win = newwin(5, 38, main_menu_win->_begy - 4 - 3, term_cols/2 - 19);

	char *mtitle =
		" ____ ____ ___    _  __  __   __  ____"
		"/ __//  _/| _ \\  / || _\\| _\\ / / | __/"
		"\\__ \\| (_ | ` / / ^|| _(| _(/ /_ | _|"
    	" /___/\\___||_|\\_/_/_||__/|__/|___/|___/"
		;
	//char *mline = "------------------------";
	char *mtitle_text = "One-Dimentional";

	wattron(title_win, A_BOLD);
	wattron(title_win, COLOR_PAIR(1));
	mvwaddstr(title_win, 0, 0, mtitle);	// centered
	wattroff(title_win, COLOR_PAIR(1));
	//mvaddstr(term_rows/3 + 1, term_cols/2 - strlen(mline)/2, mline);	// centered
	wattroff(title_win, A_BOLD);
	wattron(title_win, A_ITALIC);
	mvwaddstr(title_win, 4, 22, mtitle_text);
	wattroff(title_win, A_ITALIC);


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

				switch(curr_item_index) {

					case 0:
						/* ------------- Start game ------------- */

						// Game file checks (order is important!)
						conf_err = check_config_file(&letters, &rounds);
						if(conf_err) {
							message_log(strerror(conf_err));
							break;
						}
						dict_err = check_dict_file();
						if(dict_err) {
							if(dict_err == ENOENT) {
								message_log("Error: Default dictionary not installed!");	// default dictionary missing
							}else {
								message_log(strerror(dict_err));
							}
							break;
						}
						json_err = check_trie_json_file();
						if(json_err) {
							message_log(strerror(json_err));
							break;
						}

						unpost_menu(main_menu);  		  // hide the main menu
						wrefresh(main_menu_win);
						startGame(letters, rounds);
						post_menu(main_menu);  			  // unhide the main menu
						break;

					case 1:
						/* --------- Open game settings --------- */

						// Game file checks
						conf_err = check_config_file(&letters, &rounds);
						if(conf_err) {
							message_log(strerror(conf_err));
							break;
						}

						unpost_menu(main_menu);  		  // hide the main menu
						wrefresh(main_menu_win);
						gameSettings(&letters, &rounds);
						post_menu(main_menu);  			  // unhide the main menu
						break;

					case 2:
						/* ---- Add a word to the dictionary ---- */

						// Game file checks (order is important!)
						dict_err = check_dict_file();
						if(dict_err) {
							if(dict_err == ENOENT) {
								message_log("Error: Default dictionary not installed!");	// default dictionary missing
							}else {
								message_log(strerror(dict_err));
							}
							break;
						}
						json_err = check_trie_json_file();
						if(json_err) {
							message_log(strerror(json_err));
							break;
						}

						unpost_menu(main_menu);  		  // hide the main menu
						wrefresh(main_menu_win);
						addNewWord();
						post_menu(main_menu);  			  // unhide the main menu
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
				 * NOTE:
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
