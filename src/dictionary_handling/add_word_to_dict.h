#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../../libs/file_contents_to_string.h"


// ============================================================================================= //


int addWordToDict() {

    // Open the dictionary for appending and reading
    FILE *dict = fopen("../config/dictionary.txt", "a+");

    if(!dict) {
        // Catch any exeptions
        printf("\nError: Dictionary missing!");
        return 2;
    }

    // Copy its contetnts into a buffer
    char *dict_string = copyFileContentsToString(&dict);



    // Take user input

    // If the user presses a specific key => return to main menu            // TO DO

    char word[46];      // Longest english word is 45 letters! 
    printf("New word (Press <key> to cancel): ");
    scanf("%s", word);

    // Convert all letters to lowercase
    for(int i = 0; i < strlen(word); i++) {
        word[i] = tolower(word[i]);
    }
    word[46] = '\0';


    // Validation checks

    // Check if word is longer than allowed
    while(strlen(word) > 46) {
        printf("Longest word is 46 letters!\n");
        printf("New word: ");
        scanf("%s", word);  // the MAXIMUM word lengh is 46
    }

    // Check if the word is already in the dictionary
    
    char newline_word[strlen(word) + 2];     // a word with new lines at the front and back
    sprintf(newline_word, "\n%s\n", word);

    while(strstr(dict_string, newline_word) != NULL) {
        printf("This word is already in the dictionary!\n"); 
        printf("New word: ");
        scanf("%s", word);
        sprintf(newline_word, "\n%s\n", word);
    }



    // Append the word to the dictionary + a newline
    fprintf(dict, "%s\n", word);

    system("clear");
    printf("Insert successful!\n");


    // Close the file and free the allocated memory
    fclose(dict);
    free(dict_string);

    return 0;
}
