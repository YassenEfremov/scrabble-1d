#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/trie.h"
#include "../libs/file_contents_to_string.h"

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"

#include "./dictionary_handling/dict_to_trie.h"
#include "./dictionary_handling/add_word_to_dict.h"


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
	struct jReadElement letters2;
	struct jReadElement rounds2;
	
	FILE *fp = fopen("../json/settings.json", "r");
	
	char* jstring = copyFileContentsToString(&fp);
	
	jRead(jstring, "{'letters'", &letters2);
	jRead(jstring, "{'rounds'", &rounds2);
	
	letters = letters2.pValue;
	rounds = rounds2.pValue;
	
	fclose(fp);
}

void startingMenu() {

	char menu[30];
	int letters = 10; // Default
    int rounds = 10; // Default
	int value;
	
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

			//turn char value into int
			value = atoi(menu);
			system("clear");

	    switch(value){

	    	case 1:
				system("clear");
				dictToTrie();	// TEMPORARY
                startGame(letters, rounds); 	// start a game
				trie_delete(&dict_trie_root);	// TEMPORARY
				break;
			
	    	case 2:
				system("clear");
				gameSettings(&letters, &rounds);		// open game settings
				read_settings(&letters, &rounds);
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
