#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/trie.h"

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"

#include "./dictionary_handling/dict_to_trie.h"
#include "./dictionary_handling/add_word_to_dict.h"


// ============================================================================================= //
/* Structures, Global variables, Function declarations */


void startingMenu();	// Starts a game of scrabble


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

	char menu[1];
	int letters = 10; // default
    int rounds = 10; // default
	int value;

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
}


// ============================================================================================= //
