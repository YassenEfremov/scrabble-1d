/* Definitions for functions declared in game_logic.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>		// atoi
#include "game_logic.h"

#include "libs/jRead.h"
#include "libs/dict_handling/dict_handling.h"


// ============================================================================================= //


int check_trie(char *word) {

	FILE *trie_json = fopen("../../json/trie.json", "r");
	char *json_string = strfcpy(trie_json);
	fclose(trie_json);

	int element;	// stores the value of isEndOfWord
    
    char *ending = "isEndOfWord'";
    int length = strlen(word);
    char *bigger_word = (char *)malloc(5 * sizeof(char));	// 4 + 1 byte for '\0'
	//int new_word_len = 5;
    
	// Create the query string using the entered word
    int j = 0;
    for(int i = 0; i <= 4 * length; i+= 4, j++) {

        bigger_word[i] = '{';
        bigger_word[i+1] = '\'';	// char ' (single quote)
        bigger_word[i+2] = word[j];
        bigger_word[i+3] = '\'';	//char ' (single quote)
		bigger_word[i+4] = '\0';

        char *new_word = realloc(bigger_word, (strlen(bigger_word) + 5) * sizeof(char));
		bigger_word = new_word;
    }
    
    char *final_word = realloc(bigger_word, (strlen(bigger_word) + strlen(ending) + 1) * sizeof(char)); 
    strcat(final_word, ending);

	element = jRead_int(json_string, final_word, NULL);

	free(json_string);
	free(final_word);

	return element;
}

/*
int check_trie_temp(char *word) {
	// DON'T DELETE THIS CODE, it might be needed in the future
	// Check if the word is in the trie structure (not the json file)

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
}
*/


// --------------------------------------------------------------------------------------------- //


int enter_and_check(char rand_letters[], int letters, int* points) {

	char word[letters];
	printf("\nEnter word (Enter 9 to skip round):  ");
	scanf("%s", word);
	
	for(int i = 0; i < strlen(word); i++){
		word[i] = tolower(word[i]);
	}
	
	// If we enter 9 => end round
	if(strcmp(word, "9") == 0){
		printf("> Skipped\n");
		return -1;
	}

	// Copy the random letters to a temporary array so the original one won't change 
	char temp[letters + 1];
	strcpy(temp, rand_letters);



	// Check if the word is composed of the available letters
	int count = 0;
	for(int i = 0; word[i] != '\0'; i++){

		int flag = 0;
	
		for(int j = 0; temp[j] != '\0'; j++){
			if(word[i] == temp[j]){
				count++;
				temp[j] = '-';	// set the checked letter to '-' so double letters won't cause problems
				flag = 1;
				break;
			}
		}
		
		if(flag == 0){
			count = 0;
			printf("Try again(or skip) \n");	
			return 0;
		}
	}


	// Check if the entered word is in the dict_trie
	if(check_trie(word) == 0) {
		// If it isn't => round points are 0
		count = 0;
		printf("This word is not in the dictionary!\n");
		printf("Try again(or skip)\n");
		return 0;
	}


	*points += count;
	printf("\n\nTotal points: %d\n", *points);

	return 1;
}


// --------------------------------------------------------------------------------------------- //


//  Function that generates random letters for one round and prints them
void letter_generation(int letters, int *points) {
    char random_letter;
    char array[letters + 1];
    char vowels[] = {'a','e','i','o','u','y'};
	
    srand(time(0));	// set the seed for the random number generation
    
    random_letter = vowels[rand() % 6];
    array[0] = random_letter;	// the first letter is a random vowel
	printf("\n %c ", array[0]);
	
    for(int i = 0; i < letters - 1; i++){
        // formula for generating a random letter -> (rand() % (upper - lower + 1)) + lower;
        random_letter = (rand() % (122 - 97 + 1)) + 97; 
        array[i+1] = random_letter;
        printf(" %c ", array[i+1]);
    }
	printf("\n");
    
	array[letters + 1] = '\0';
	
	int flag;
	do{
	
		flag = enter_and_check(array, letters, points);
		
	}while(flag == 0);
}


// --------------------------------------------------------------------------------------------- //


void startGame(int letters, int rounds) {

	int points = 0;
		
	for(int i = 0; i < rounds; i++){
		printf("\n\n-- Round %d --\n", i+1);
        letter_generation(letters, &points);
	}
	
	// At the end of the game
	printf(
		"\n\n\n"
		"#-----------------------#"
		"\n"
		"Game Over! Total points: %d\n", points
		);
	printf("Press ENTER to exit...");
	// getchar is called twice because the first enter is taken from the last word submition
	getchar();
	getchar();
}
