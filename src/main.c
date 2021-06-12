#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"

#include "./dictionary_handling/trie_handling.h"
#include "./dictionary_handling/add_word_to_dict.h"


// ============================================================================================= //
/* Structures, Global variables, Function declarations */


void startingMenu();


// ============================================================================================= //


int main() {
    
    startingMenu();

    return 0;
}


// ============================================================================================= //
/* Function definitoins */


void startingMenu() {

    system("clear");

	int menu;
	int letters = 10; // default
    int rounds = 10; // default

	do {
        printf("--------------------------------------------- \n");
	    printf("	(1)	New Game  \n");
	    printf("	(2)	Settings  \n");
	    printf("	(3)	Enter word in dictionary  \n");
	    printf("	(4)	Exit  \n");
	    printf("--------------------------------------------- \n");
	    scanf("%d", &menu);
	
	    switch(menu) {

	    	case 1:
                //printf("zapochva igrata");
                startGame(letters, rounds); // funkciqta za IGRATA
	    		break;
			
	    	case 2:
                //printf("otivame v settings");
                gameSettings(&letters, &rounds); // fynkicq za settings
	    		break;
			
	    	case 3:
                // printf("trqbva da vuvedem dyma v rechnika"); // Tyk trqbva fynkciq za dobavqne na dyma v rechnika

				dictToTrie();
				//addWordToDict();
				//extractSringFromDict();

	    		break;
			
	    	case 4:
                system("clear"); 
                exit(EXIT_SUCCESS); // izlizame ot igrata
	    		//break;
			
	    	default:
                system("clear");
	    		printf("Nevalidno, probvai pak\n");
	    		break;
	    }
	
	} while (menu < 0 || menu > 4);
	
	//system("clear");
}


// ============================================================================================= //
