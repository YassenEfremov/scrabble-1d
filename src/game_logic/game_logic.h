#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../../libs/jRead.h"

#include "../../libs/trie.h"
#include "../../libs/file_contents_to_string.h"


// ============================================================================================= //


int check_trie(char *word) {

	FILE* trie_json = fopen("../json/trie.json", "r");
	char* json_string = copyFileContentsToString(&trie_json);

	int element;
	
    
    char* ending = "isEndOfWord'";
    int lenght = strlen(word);
    char* new_word = (char *)malloc(4*sizeof(char));
    char* symbol_holder = "'";
    
    int j = 0;
    for(int i = 0; i <= 4 * lenght; i+= 4,j++) {

        new_word[i] = '{';
        new_word[i+1] = symbol_holder[0];
        new_word[i+2] = word[j];
        new_word[i+3] = symbol_holder[0];

		int new_word_len = strlen(new_word);
        new_word = realloc(new_word, (4 + new_word_len)*sizeof(char));
    }
    
    new_word = realloc(new_word, (strlen(ending) + strlen(new_word))*sizeof(char)); 

    strcat(new_word, ending);

	element = jRead_int(json_string, new_word, NULL);

	free(new_word);
	return element;
}


// int check_trie_temp(char *word) {
//     // TEMPORARY
//     // Check if the word is in the trie structure (not the json file)

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

// }

int enter_and_check(char rand_letters[], int letters, int* points) {

	char word[letters];
	printf("\nEnter word (or enter 9 to skip level):  ");
	scanf("%s", word);
	
	for(int n =0; word[n] != '\0'; n++){
		word[n] = tolower(word[n]);
	}
	
	char quit_word[] = "9";

	// If we enter 9 => end round	
	if(strcmp(word, quit_word) == 0){
		printf("> Skipped\n");
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
			printf("Try again(or skip) \n");	
			return 0;
		}
	}


	// Check if the entered word is in the dict_trie

	if(check_trie(word) == 0) {
		// If it isn't => round points are 0
		count = 0;
		printf("This word is not in the dictionary\n");
		printf("Try again(or skip) \n");
		return 0;
	}


	*points += count;
	printf("Total points: %d\n", *points);

	return 1;
}


//  Function that generates random letters for one round and prints them
void letter_generation(int letters, int* points) {
    int random_letter;
    char array[letters + 1];
    char vowels[] = {'a','e','i','o','u','y'};
	
    srand(time(0));	// set the seed for the random number generation
    
    random_letter = vowels[rand() % 6];
    array[0] = random_letter;
	printf("\n %c ", array[0]);
	
    for(int i = 0; i < letters - 1; i++){
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

extern void startGame(int letters, int rounds) {
	int points = 0;
		
	for(int i = 0; i < rounds; i++){
        letter_generation(letters, &points);
	}

	system("clear");	
	printf("Your score is:  %d", points);
}
