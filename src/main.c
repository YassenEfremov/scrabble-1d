#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/trie.h"

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"

#include "./dictionary_handling/add_word_to_dict.h"
#include "./dictionary_handling/dict_to_trie.h"
#include "./dictionary_handling/trie_to_json.h"


// ============================================================================================= //
/* Structures, Global variables, Function declarations */


void startingMenu();	// Starts a game of scrabble


// ============================================================================================= //


int main() {
    
   startingMenu();

   return 0;
}


// ============================================================================================= //
/* Function definitoins */


void startingMenu() {

	char menu[30];
	int letters = 10; // default
    int rounds = 10; // default
	int value;
	int to_free = 0;	// flag

	struct node_t *trie_root;

	system("clear");
	
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

		//turn char value into int
		value = atoi(menu);
		system("clear");

	    switch(value){

	    	case 1:
				system("clear");
				//> Check if the dictionary has changed while the game hasn't been runnging		// TO DO
				//> if it has => update the json trie, flag to_free = 1
                startGame(letters, rounds); 	// start a game
				system("clear");
				break;
			
	    	case 2:
				system("clear");
				gameSettings(&letters, &rounds);		// open game settings
				system("clear");
				break;
			
	    	case 3:
				system("clear");
				addWordToDict();				// add word to the dictionary
				trie_root = dictToTrie();		// generate trie from dictionary
				trieToJson(trie_root);			// write generated trie to json
				trie_delete(trie_root);			// free the memory for trie
				to_free = 1;
				break;
			
	    	case 4:
                system("clear"); 
                exit(EXIT_SUCCESS);	// exit the game
	    		break;
	    		
	    	default:
                // invalid option
                system("clear");
				if(to_free) free(trie_root);	// if we have done anything withe the tire => free its root
                printf("Invalid, try again!\n");
                break;
	    }
	
	}while(1);
}
