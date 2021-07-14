#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>
#include <errno.h>

#include "libs/trie.h"
#include "libs/dict_handling/dict_handling.h"

#include "game_logic/game_logic.h"
#include "game_settings/game_settings.h"


/* ============================================================================================= */
/* Private functions */


static int get_settings(int* letters, int* rounds) {

	config_t game_settings;

	config_init(&game_settings);
	if(config_read_file(&game_settings, "../config/game_settings.cfg") != CONFIG_TRUE) {	// TEMPORARY LOCATION
		printf("\nError: game_settings.json missing!");
		return 2;
	}

	config_lookup_int(&game_settings, "letters", letters);
	config_lookup_int(&game_settings, "rounds", rounds);

	config_destroy(&game_settings);
	
	return 0;
}


/* ============================================================================================= */
/* Public functions */


int main(int argc, char *argv[]) {

	char menu[1];	// the size of this array DOESN'T cause problems because we directly exit the program
	int value;
	//int to_free = 0;	// flag

	int letters;
    int rounds;

	struct node_t *trie_root;

	get_settings(&letters, &rounds);
	system("clear");
	


	/* Main program loop */

	do {
	
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

		//turn char value to int
		value = atoi(menu);
		system("clear");

	    switch(value){

			/* -- Main menu options -- */


			/* New Game */
	    	case 1:
				system("clear");
				//> Check if the dictionary has changed while the game hasn't been runnging		// TO DO
				//> if it has => update the json trie, flag to_free = 1
                startGame(letters, rounds); 	// start a game
				system("clear");
				break;
			
			/* Settings */
	    	case 2:
				system("clear");
				gameSettings(&letters, &rounds);		// open game settings
				get_settings(&letters, &rounds);
				system("clear");
				break;
			
			/* Add word */
	    	case 3:
				system("clear");
				addWordToDict();				// add word to the dictionary

				trie_root = dictToTrie();		// generate trie from dictionary
				trieToJson(trie_root);			// write generated trie to json

				trie_delete(trie_root);			// free the memory for trie
				free(trie_root);
				//to_free = 1;
				break;
			
			/* Exit */
	    	case 4:
                system("clear"); 
                return 0;	// exit the game
	    		break;
	    		
			/* Invalid option */
	    	default:
                system("clear");
				//if(to_free) free(trie_root);	// if we have done anything with the tire => free its root
                printf("Invalid, try again!\n");
                break;
	    }
	
	}while(1);

	return 0;
}
