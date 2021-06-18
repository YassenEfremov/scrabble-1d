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


void startingMenu();	// Starts the entire application


// ============================================================================================= //


int main() {
    
   //int letters;
   //int rounds;
    
   //letters_from_file(&letters);
   //rounds_from_file(&rounds);
    
   startingMenu();

   return 0;
}


// ============================================================================================= //
/* Function definitoins */


void startingMenu() {

	//char menu;
	int letters = 10; // default
    int rounds = 10; // default
	//int value;

	int rows, cols;
	int num_of_options = 4;

	ITEM **my_options;
	MENU *main_menu;
	ITEM *curr_item;
	int c;

	char *options[] = {
		"New Game",
		"Settings",
		"Add word",
		"Exit"
	};

	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);


	getmaxyx(stdscr, rows, cols);
	attron(A_BOLD);
	mvaddstr(rows/3, cols/2 - 4, "SCRABBLE");
	attroff(A_BOLD);


	// COPIED
	// initialize options
	my_options = (ITEM **)calloc(num_of_options + 1, sizeof(ITEM *));
	for(int i = 0; i < num_of_options; i++) {
		my_options[i] = new_item(options[i], "");
		my_options[num_of_options] = (ITEM *)NULL;
	}

	// initialize menu
	main_menu = new_menu((ITEM **)my_options);
	mvprintw(rows - 2, 0, "F1 to exit");
	set_menu_sub(main_menu, derwin(stdscr, 0, 0, rows/2, cols/2 - 4));
	post_menu(main_menu);

	refresh();

	// menu navigation
	while ((c = getch()) != KEY_F(1)) {
		switch(c) {
			case KEY_DOWN:
				menu_driver(main_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(main_menu, REQ_UP_ITEM);
				break;
		}
	}

	free_item(my_options[0]);
	free_item(my_options[1]);
	free_menu(main_menu);

	endwin();

/*
	system("clear");
	
	do {
		
		do {
			//to add a check if string is longer than 1
			printf(
				"\n"
				"\n"
				"			SCRABBLE\n" 
				"		  --------------------\n"
				"\n"
				"		(1)	 New Game\n"
    			"		(2)	 Settings\n" 
    			"		(3)	 Add word\n"
    			"		(4)	 Exit	\n"
				"\n"
				"\n"
				"______________________________________________________\n"
			);
			printf("> ");
			scanf("%s", menu);

			//turn char value into int
			value = (menu[0] - '0');

			system("clear");
			if(value < 1 || value > 4) printf("Invalid, try again.");

		}while(value < 1 || value > 4);

	    switch(value){

	    	case 1:
				system("clear");
				dictToTrie();	// TEMPORARY
                startGame(letters, rounds); 	// start a game
				trie_delete(&dict_trie_root);	// TEMPORARY
				break;
			
	    	case 2:
				system("clear");
				openSettings();		// open game settings
				system("clear");
				break;
			
	    	case 3:
				system("clear");
				addWordToDict();	// add word to the dictionary
				dictToTrie();		// generate trie from dictionary
				break;
			
	    	case 4:
                system("clear"); 
                exit(EXIT_SUCCESS);	// exit the game
	    		break;
			
	    	default:
				// invalid option
				system("clear");
	    		printf("An Error has appiered!\n");
	    		break;
	    }
	
	}while(1);
	*/
}

// ============================================================================================= //
