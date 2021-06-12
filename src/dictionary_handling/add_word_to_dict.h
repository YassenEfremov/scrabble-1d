#include <stdio.h>
#include <string.h>

#include "file_contents_to_string.h"

// ============================================================================================= //


extern void addWordToDict() {

    // Open the dictionary for appending and reading
    FILE *dict = fopen("./dictionary_handling/dictionary.csv", "a+");

    if(!dict) {
        // Catch any exeptions
        printf("\nDictionary doesn't exist!");
        return;
    }

    // Copy its contetnts into a buffer
    char *buffer = copyFileContentsToString(&dict);



    // Take user input
    char *word = (char *)malloc(sizeof(char));  
    printf("New word: ");
    scanf("%s", word);

    // Check if word is longer than allowed
    while(strlen(word) > 46) {
        printf("Longest word is 46 letters!\n");
        printf("New word: ");
        scanf("%s", word);  // the MAXIMUM word lengh is 46
    }

    // Check if the word is already in the dictionary
    
    // BUG: if the entered word is part of a bigger word, it's not valid!                               // TO FIX

    while(strstr(buffer, word) != NULL) {
        printf("The word is already in the dictionary!\n"); 
        printf("New word: ");
        scanf("%s", word);
    }



    // append the word
    fprintf(dict, "\n%s", word);

    printf("Insert successful!\n");


    // close the file and free the allocated memory
    fclose(dict);
    free(buffer);
    free(word);    
}
