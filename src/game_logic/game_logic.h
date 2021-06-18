#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//#include ""	// include the json parser

#include "../../libs/trie.h"


// ============================================================================================= //


int check_trie_json(char *string) {
	//> Check if the entered word is in the trie.json							// TO DO
	//> return 1 if it is
	//> return 0 if it isn't

	FILE *trie_bin = fopen("../json/trie.json", "r");

	//fread();

	fclose(trie_bin);

	return 1;
}


int check_trie(char *word) {
    // TEMPORARY
    // Check if the word is in the trie structure (not the json file)

    int level;
    int length = strlen(word);
    int index;
    struct node_t *pCrawl = &dict_trie_root;

    for (level = 0; level < length; level++)
    {
        index = (int)word[level] - (int)'a';

        if (!pCrawl->children[index])
            return 0;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);

/*
    struct node_t *temp = &dict_trie_root;
    int letter_index;

    for(int level = 0; level < strlen(word); level++) {
        letter_index = (int)(word[level] - 'a');

        if(temp->children[letter_index] == NULL) return 0;
        printf("%p ", temp->children[letter_index]);
        temp = temp->children[letter_index];
    }

    printf("(%d)", (temp != NULL && temp->isEndOfWord));
    return (temp != NULL && temp->isEndOfWord);
    */

}


extern int enter_and_check(char rand_letters[], int letters, int* points){

	char word[letters];
	printf("\nEnter word (or enter 9 to skip level):  ");
	scanf("%s", word);
	
	for(int n =0; word[n] != '\0'; n++){
		word[n] = tolower(word[n]);
	}
	
	char quit_word[] = "9";

	// If we enter 9 => end round	
	if(strcmp(word, quit_word) == 0){
		printf(">Skipped\n");
		return -1;
	}

	// Check if the word is composed of the available letters
	int count = 0;
	for(int i = 0; word[i] != '\0'; i++){

		int flag = 0;
	
		for(int j = 0; rand_letters[j] != '\0'; j++){
			if(rand_letters[j] == word[i]){
				count++;
				flag = 1;
				rand_letters[j] = '-';
				break;
			}
		}
		
		if(flag == 0){
			count = 0;
			return 0;
		}
	}

	// Check if the entered word is in the dict_trie
	if(check_trie(word) == 0) {
		// If it isn't => round points are 0
		count = 0;
		printf("Try again(or skip) \n");
		return 0;
	}

	*points += count;
	printf("Total points: %d\n", *points);

	return 1;
}


//  Function that generates random letters for one round and prints them
extern void letter_generation(int letters, int* points){
    int random_letter;
    char array[letters + 1];
    char vowels[] = {'a','e','i','o','u','y'};
	
    srand(time(0));	// set the seed for the random number generation
    
    random_letter = vowels[rand() % 6];
    array[0] = random_letter;
	printf("\n %c ", array[0]);
	
    for(int i=0; i < letters - 1; i++){
        // formula for generating a random letter -> (rand() % (upper - lower + 1)) + lower;
        random_letter = (rand() % (122 - 97 + 1)) + 97; 
        array[i+1] = random_letter;
        printf("| %c ", array[i+1]);
    }
	array[letters + 1] = '\0';
	
	int flag;
	do{

		flag = enter_and_check(array, letters, points);
		
	}while(flag == 0);
}



extern void startGame(int letters, int rounds){
	int points = 0;	
	for(int i = 0; i < rounds; i++){
        letter_generation(letters, &points);
	}

	system("clear");	
	printf("Your score is:  %d", points);
}
