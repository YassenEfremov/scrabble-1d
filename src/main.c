#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/trie.h"
#include "../libs/file_contents_to_string.h"

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"

#include "./dictionary_handling/add_word_to_dict.h"
#include "./dictionary_handling/dict_to_trie.h"
#include "./dictionary_handling/trie_to_json.h"


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

	char menu[1];	// the size of this array DOESN'T cause problems because we directly exit the program
	int value;
	//int to_free = 0;	// flag

	int letters;
    int rounds;

	struct node_t *trie_root;
	

	read_settings(&letters, &rounds);
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

		//turn char value to int
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
				read_settings(&letters, &rounds);
				system("clear");
				break;
			
	    	case 3:
				system("clear");
				addWordToDict();				// add word to the dictionary

				trie_root = dictToTrie();		// generate trie from dictionary
				trieToJson(trie_root);			// write generated trie to json

				trie_delete(trie_root);			// free the memory for trie
				free(trie_root);
				//to_free = 1;
				break;
			
	    	case 4:
                system("clear"); 
                exit(EXIT_SUCCESS);	// exit the game
	    		break;
	    		
	    	default:
                // invalid option
                system("clear");
				//if(to_free) free(trie_root);	// if we have done anything with the tire => free its root
                printf("Invalid, try again!\n");
                break;
	    }
	
	}while(1);
}
