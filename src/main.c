#include <stdio.h>


// ============================================================================================= //
/* Structures, Global variables, Function declarations */

int startingMenu();


// ============================================================================================= //


int main() {
    
    

    return 0;
}


// ============================================================================================= //
/* Function definitoins */

int startingMenu(){
	int menu;
	system("clear");
	
	do{
	printf("	(1)	New Game  \n");
	printf("	(2)	Settings  \n");
	printf("	(3)	Enter word in dictionary  \n");
	printf("	(4)	Exit  \n");
	printf("--------------------------------------------- \n");
	scanf("%d", &menu);
	
	switch(menu)
	{
		case 1: printf("zapochva igrata"); // Tyk trqbva da se dobavi funkciqta za IGRATA
			break;
			
		case 2: printf("otivame v settings"); // Tyk trqbva fynkicq za settings 
			break;
			
		case 3: printf("trqbva da vuvedem dyma v rechnika"); // Tyk trqbva fynkciq za dobavqne na dyma v rechnika
			break;
			
		case 4: printf("izlizame ot igrata");
			return 0;
			break;
			
		default: system("clear");
			printf("Nevalidno, probvai pak\n");
			break;
	}
	
	}while (menu < 0 || menu > 5);
	
	system("clear");
}

// ============================================================================================= //
