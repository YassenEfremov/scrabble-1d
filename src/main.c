#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./game_logic/game_logic.h"
#include "./game_settings/game_settings.h"


// ============================================================================================= //
/* Structures, Global variables, Function declarations */


void startingMenu();


// ============================================================================================= //


int main() {
    
   int letters;
   int rounds;
    
   letters_from_file(&letters);
   rounds_from_file(&rounds);
    
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
	
	MAIN_MENU:
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
                //printf("zapochva igrata");
                startGame(letters, rounds); // funkciqta za IGRATA
	    	goto MAIN_MENU;
			
	    	case 2:
                //printf("otivame v settings");
                gameSettings(&letters, &rounds); // fynkicq za settings
                goto MAIN_MENU;
			
	    	case 3:
                printf("trqbva da vuvedem dyma v rechnika"); // Tyk trqbva fynkciq za dobavqne na dyma v rechnika
                goto MAIN_MENU;
			
	    	case 4:
                system("clear"); 
                exit(EXIT_SUCCESS); // izlizame ot igrata
	    		//break;
			
	    	default:
                system("clear");
	    		printf("Nevalidno, probvai pak\n");
	    		break;
	    }
	
	} while (menu < 1 || menu > 4);
	
	system("clear");
}


// ============================================================================================= //
