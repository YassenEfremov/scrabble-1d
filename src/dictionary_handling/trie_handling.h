#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/trie.h"


extern void trieGenerate(char *word) {
    // generate trie from string
}


extern void extractSringFromDict() {

    // Open the dictionary file for reading 
    FILE *dict  = fopen("./dictionary_handling/dictionary.csv", "r");

    if(!dict) {
        // Catch any exeptions
        printf("\nDictionary doesn't exist!");
        return;
    }


    // Get the number of letters in the file
    fseek(dict, 0, SEEK_END);   // set file position indicator at the end
    long dict_size = ftell(dict);   // get the value of the pos indicator = bytes in the file
    fseek(dict, 0, SEEK_SET);   // reset pos indicator

    // Save them in a buffer
    char *buffer = (char *)malloc(dict_size + 1);
    fread(buffer, dict_size, 1, dict);

    fclose(dict);

    buffer[dict_size] = '\0';


    // Split the huge string in the buffer into words
    char *token = strtok(buffer, ",");

    while(token != NULL) {
        //> Add to trie
        //printf("%s\n", token);
        token = strtok(NULL, ",");
        
    }

    free(buffer);
}
