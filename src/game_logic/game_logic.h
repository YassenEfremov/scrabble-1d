#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//  Funkciq za generirane na bykwi za edin round i printiraneto im
extern void letter_generation(int letters){
    int random_letter;
    int array[letters + 1];
    
    srand(time(0)); // generira mi random chislo, ot koeto zavisqt random chislata
    
    for(int i=0; i < letters; i++){
        // izpolzvam formula za generiraneto na slychaina bykwa -> (rand() % (upper - lower + 1)) + lower;
        random_letter = (rand() % (122 - 97 + 1)) + 97; 
        array[i] = random_letter;
        printf("%c  | ", array[i]);
    }
}

extern void enter_and_check(char ran_letters[]){

	char *word;
	int flag = 0;
	
	printf("Enter word:  ");
	scanf("%s", &word);
	
	
		for(int i=0;  !=  ; i++){
		printf("2\n");
			for(int j=0; j < b - 1; j++){
			printf("3\n");
				if(word[i] == ran_letters[j]){
					ran_letters[j] = ''; // za da mahna slychai v koito dymata na potrebitelq 									  izpolzva edna vykwa dva puti
					flag = 1;
					printf("4\n");
			}
				printf("1234\n");
		}
	}
}


extern void startGame(int letters, int rounds) {
    letter_generation(letters);
    //enter_and_check(letters);
}
