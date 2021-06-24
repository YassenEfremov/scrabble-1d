#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>

#include "../libs/trie.h"

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"

#include "./dictionary_handling/dict_to_trie.h"
#include "./dictionary_handling/add_word_to_dict.h"

#include "./ui/exit_app.h"
#include "./ui/message_box.h"


// ============================================================================================= //
/* Structures, Global variables, Function declarations */


void startingMenu();								// Starts the entire application
//void item_func(int curr_item_index);						// Executes the function associated with the passed item name


// ============================================================================================= //


int main() {
    
   //int letters = 10;
   //int rounds = 10;
    
   //letters_from_file(&letters);
   //rounds_from_file(&rounds);
    
   startingMenu();	// Start the application

   return 0;
}


// ============================================================================================= //
/* Function definitoins */


void startingMenu() {

	int letters = 10; // default
	int rounds = 10; // default

	// Start ncurses

	// Start main window
	initscr();

	// Define colors
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);		// Menu green
	init_pair(2, COLOR_WHITE, COLOR_BLACK);		// Menu background
	init_pair(99, COLOR_MAGENTA, COLOR_GREEN);	// For debugging

	// Other settings
	raw();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);



	// Predefined data used by ncurses

	int rows, cols;
	getmaxyx(stdscr, rows, cols);	// get the dimentions of the main screen

	char *options_list[] = {
		"New Game",
		"Settings",
		"Add word",
		"Exit"
	};
	int num_of_options = sizeof(options_list)/sizeof(options_list[0]);

	ITEM *curr_item;
	int key;
	int curr_item_index;

	

	// Main menu

	// Title
	char *mtitle = "SCRABBLE";
	char *mline = "------------";

	attron(A_BOLD);
	attron(A_ITALIC);
	attron(COLOR_PAIR(1));
	mvaddstr(rows/3, cols/2 - strlen(mtitle)/2, mtitle);	// centered
	attroff(COLOR_PAIR(1));
	mvaddstr(rows/3 + 1, cols/2 - strlen(mline)/2, mline);	// centered
	attroff(A_ITALIC);
	attroff(A_BOLD);

	// Options
	ITEM **options = (ITEM **)calloc(num_of_options + 1, sizeof(ITEM *));
	for(int i = 0; i < num_of_options; i++) {
		options[i] = new_item(options_list[i], "");		// Name each option
	}
	options[num_of_options] = NULL;

	// Menu
	WINDOW *main_menu_win = derwin(stdscr, num_of_options, 12, rows/3 + 3, cols/2 - 6);
	MENU *main_menu = new_menu((ITEM **)options);

	// Menu settings	
	set_menu_sub(main_menu, main_menu_win);
	set_menu_mark(main_menu, "> ");
	set_menu_fore(main_menu, A_BOLD);
	
	post_menu(main_menu);

	

	// Message window

	// This window is used to display messages ingame
	msg_win = derwin(stdscr, 1, MSG_LEN, rows/2 + 6, cols/2 - MSG_LEN/2);

	// Navigation guide message
	char *nav_guide_msg = "(Use arrow keys to navigate)";
	char *soon_msg = "Coming soon!";
	message_log(nav_guide_msg);

	refresh();



	// Navigation
	do {
		key = getch();
		werase(msg_win);
		curr_item = current_item(main_menu);

		switch(key) {
			case KEY_DOWN:
				menu_driver(main_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				menu_driver(main_menu, REQ_UP_ITEM);
				break;

			case 10:	// Enter key
				curr_item_index = item_index(curr_item);

				switch(curr_item_index) {
					case 0:
						message_log(soon_msg);
						break;

					case 1:
						unpost_menu(main_menu);		// hide the main menu
						gameSettings();
						post_menu(main_menu);		// unhide the main menu
						break;

					case 2:
						message_log(soon_msg);
						break;

					case 3:
						exitMenu(&main_menu, &options, num_of_options);
						endwin();
						exit(EXIT_SUCCESS);
				}
				break;
		}
		wrefresh(msg_win);

	}while(1);
}


// ============================================================================================= //
