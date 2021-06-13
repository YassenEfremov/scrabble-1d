#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs/trie.h"

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"

#include "./dictionary_handling/dict_to_trie.h"
#include "./dictionary_handling/add_word_to_dict.h"


// ============================================================================================= //
/* Structures, Global variables, Function declarations */


void startingMenu();


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

	int menu;
	int letters = 10; // default
    int rounds = 10; // default

	system("clear");
	
	do {
        printf("\n--------------------------------------------- \n");
	    printf("	(1)	New Game  \n");
	    printf("	(2)	Settings  \n");
	    printf("	(3)	Enter word in dictionary  \n");
	    printf("	(4)	Exit  \n");
	    printf("--------------------------------------------- \n");
	    scanf("%d", &menu);
	
	    switch(menu) {

	    	case 1:
				system("clear");
				dictToTrie();
                startGame(letters, rounds); // funkciqta za zapochvane na IGRATA
				trie_delete(&dict_trie_root);
				break;
			
	    	case 2:
				system("clear");
                gameSettings(&letters, &rounds); // fynkicq za settings
				system("clear");
				break;
			
	    	case 3:
				system("clear");
				addWordToDict();		// dobavqne na dum v rechnika	
				dictToTrie();			// generirane na durvo ot rechnika
				break;
			
	    	case 4:
                system("clear"); 
                exit(EXIT_SUCCESS); // izlizame ot igrata
	    		break;
			
	    	default:
				system("clear");
	    		printf("Nevalidno, probvai pak\n");
	    		break;
	    }
	
	} while (1);
}


// ============================================================================================= //
