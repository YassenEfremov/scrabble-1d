#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>

#include "../libs/trie.h"
#include "../libs/file_contents_to_string.h"

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"

#include "./dictionary_handling/add_word_to_dict.h"
#include "./dictionary_handling/dict_to_trie.h"
#include "./dictionary_handling/trie_to_json.h"

#include "./ui/exit_element.h"
#include "./ui/message_box.h"


// ============================================================================================= //
/* Structures, Global variables, Function declarations */


void startingMenu();	// Starts a game of scrabble
void read_settings();

// ============================================================================================= //


int main() {

   startingMenu();

   return 0;
}


// ============================================================================================= //
/* Function definitoins */

void read_settings(int* letters, int* rounds){
	
	FILE *fp = fopen("../config/settings.json", "r");
	char* json_string = copyFileContentsToString(&fp);
	fclose(fp);
	
	*letters = jRead_int(json_string, "{'letters'", NULL);
	*rounds = jRead_int(json_string, "{'rounds'", NULL);
	
	free(json_string);
}


// --------------------------------------------------------------------------------------------- //


void startingMenu() {

	int letters = 10; // default
	int rounds = 10; // default

	struct node_t *trie_root;

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

	//int to_free = 0;	// flag


	read_settings(&letters, &rounds);
	system("clear");
	
	// --------------------------------------------------------------------------------------------- //
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
	ITEM **items = (ITEM **)calloc(num_of_items + 1, sizeof(ITEM *));
	for(int i = 0; i < num_of_items; i++) {
		items[i] = new_item(items_list[i], "");		// Name each option
	}
	items[num_of_items] = NULL;

	// Menu
	WINDOW *main_menu_win = derwin(stdscr, num_of_items, 12, rows/3 + 3, cols/2 - 6);
	MENU *main_menu = new_menu((ITEM **)items);

	// Menu settings	
	set_menu_sub(main_menu, main_menu_win);
	set_menu_mark(main_menu, "> ");
	set_menu_fore(main_menu, A_BOLD);


	// --------------------------------------------------------------------------------------------- //
	// Message window

	// This window is used to display messages ingame
	msg_win = derwin(stdscr, 1, MSG_LEN, rows/2 + 6, cols/2 - MSG_LEN/2);	// DEFINED GLOBALLY IN message_box.h

	// Navigation guide message
	char *nav_guide_msg = "(Use arrow keys to navigate)";
	char *soon_msg = "Coming soon!";
	message_log(nav_guide_msg);


	// Post the menu
	post_menu(main_menu);

	refresh();


	// --------------------------------------------------------------------------------------------- //
	// Navigation
	do {
		key = getch();
		werase(msg_win);
		curr_item = current_item(main_menu);

		switch(key) {
			// Navigation with arrow keys

			case KEY_DOWN:
				menu_driver(main_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				menu_driver(main_menu, REQ_UP_ITEM);
				break;

			case 10:
				// Enter key
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
						gameSettings();
						post_menu(main_menu);		// unhide the main menu
						break;

					case 2:
						// Add a word to the dictionary
						//addWordToDict();
						message_log(soon_msg);
						break;

					case 3:
						// Exit the whole app
						exitMenu(&main_menu, &items, num_of_items);
						endwin();
						exit(EXIT_SUCCESS);
				}
				break;
		}
		wrefresh(msg_win);

	}while(1);
}
