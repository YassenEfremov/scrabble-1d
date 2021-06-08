#include <stdio.h>

// Here are the sittings for letters and rounds

extern void gameSettings(int *letters, int *rounds) {
	int choice;	
	int num_letters2;
	int num_rounds2;
	
	system("clear");
	
	do{
	
	printf(" 	(1)   Promenqne na broq na podavanite bykvi \n");
	printf(" 	(2)	  Promenqne na broq ryndove \n");
	scanf("%d", &choice);
	
	if(choice == 1){
		printf("Kolko bykwi jelaete: \n");
		scanf("%d", &num_letters2);
		
		if(num_letters2 < 0 || num_letters2 > 27){
			printf("nevalidno");
			return;
		}
		*letters = num_letters2;
		
	}else if(choice == 2){
		printf("Kolko runda jelaete:  \n");
		scanf("%d", &num_rounds2);
		
		if(num_rounds2 < 0 || num_rounds2 > 100){
			printf("Nevalidno");
			return;	
		}
			*rounds = num_rounds2;	
		}
		
	}while (choice < 0 || choice > 3);
}



