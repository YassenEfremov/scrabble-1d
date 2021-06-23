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


// ============================================================================================= //
/* Structures, Global variables, Function declarations */

/*
struct start_game_t {
	void (*func)(int letters, int rounds);
};

struct open_settings_t {
	void (*func)();
};

struct add_word_to_dict_t {
	void (*func)();
};

struct exit_app_t {
	void (*func)(MENU **main_menu, ITEM ***options);
};
*/

void startingMenu();								// Starts the entire application
void item_func(int curr_item_index);					// Executes the function associated with the passed item name
void exitApp(MENU **main_menu, ITEM ***options);	// Closes the entire application

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


void exitApp(MENU **main_menu, ITEM ***options) {

	unpost_menu(*main_menu);
	free_menu(*main_menu);

	free_item((*options)[0]);
	free_item((*options)[1]);
	free_item((*options)[2]);
	free_item((*options)[3]);
	free(*options);

	endwin();
}

/*
void item_func(int curr_item_index) {

	switch(curr_item_index) {
		case 0: break;
		case 1: break;
		case 2: break;
		case 3: exitApp();
	}
}
*/

void startingMenu() {

	int letters = 10; // default
	int rounds = 10; // default

	// Predefined ncurses menu elements

	int rows, cols;

	char *options_list[] = {
		"New Game",
		"Settings",
		"Add word",
		"Exit"
	};
/*
	struct start_game_t start_game = {.func = startGame};
	struct open_settings_t open_settings = {.func = openSettings};
	struct add_word_to_dict_t add_word_to_dict = {.func = addWordToDict};
	struct exit_app_t exit_app = {.func = exitApp};
*/

	ITEM **options;
	ITEM *curr_item;
	MENU *main_menu;
	char *curr_item_name;
	int key;
	int curr_item_index;

	int num_of_options = sizeof(options_list)/sizeof(options_list[0]);


	// Start ncurses

	// Start main window
	initscr();

	// Start colors
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);		// Menu title

	// Other options
	raw();
	noecho();
	keypad(stdscr, TRUE);

	getmaxyx(stdscr, rows, cols);	// get the dimentions of the main screen



	// Main menu

	// Title
	char *mtitle = "-- SCRABBLE --";
	int mtitle_len = strlen(mtitle);

	attron(A_BOLD);
	attron(A_ITALIC);
	attron(COLOR_PAIR(1));
	mvaddstr(rows/3, cols/2 - mtitle_len/2, mtitle);	// centered
	attroff(COLOR_PAIR(1));
	attroff(A_ITALIC);
	attroff(A_BOLD);

	// Options
	options = (ITEM **)calloc(num_of_options, sizeof(ITEM *));
	for(int i = 0; i < num_of_options; i++) {
		options[i] = new_item(options_list[i], "");
	}
	/*
	set_item_userptr(options[0], &start_game);
	set_item_userptr(options[1], &open_settings);
	set_item_userptr(options[2], &add_word_to_dict);
	set_item_userptr(options[3], &exit_app);
	*/

	// Menu
	main_menu = new_menu((ITEM **)options);
	//mvprintw(rows - 2, 0, "F1 to exit");
	set_menu_sub(main_menu, derwin(stdscr, 0, 0, rows/3 + 3, cols/2 - 5));
	
	post_menu(main_menu);

	refresh();

	// Navigation
	do {
		key = getch();
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
				//item_func(curr_item_index);	// Execute the selected item function
				switch(curr_item_index) {
					case 0:
						mvprintw(rows/2 + 6, cols/2 - 8, "Coming soon^(tm)!");
						break;
					case 1:
						mvprintw(rows/2 + 6, cols/2 - 8, "Coming soon^(tm)!");
						//exitApp(&main_menu, &options);
						//openSettings();
						break;
					case 2:
						mvprintw(rows/2 + 6, cols/2 - 8, "Coming soon^(tm)!");
						break;
					case 3:
						exitApp(&main_menu, &options);
						exit(EXIT_SUCCESS);
				}
				break;
		}

	}while(1);

	//exitApp(&main_menu, &options);

}


// ============================================================================================= //
