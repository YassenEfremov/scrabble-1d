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
	    
   int flag = 0;
    do{
        flag = enter_and_check(array, letters);    // trqbva da go dovursha
    }while ()
}

extern int enter_and_check(char ran_letters, int letters){
    
    char word[letters];
            
    printf("Enter word to play or enter '9' to skip this level:    ");
    scanf("%s", &word);
    
    char quit_word[] = "9";
    int flag = 0;    
    
    if(strcmp(word, quit_word) == 0){
        return -1;
    }
    
    for(int i = 0; word[i] != '\0'; i++){
        for(int j = 0; ran_letters[j] != '\0'; j++){
            
            if(ran_letters[j] == word[i]){
                flag = 1;
                ran_letters[j] = ' ';
                break;
            }
        }
        
        if(flag == 0){
            return 0;
        }
    }
}

extern void startGame(int letters, int rounds) {
    letter_generation(letters);
}
