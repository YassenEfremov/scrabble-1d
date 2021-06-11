#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/trie.h"


// ============================================================================================= //


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

    buffer[dict_size] = '\0';   // last char is terminating zero



    // Start constructing the trie
    struct trie_t dict_trie;
    trie_init(&dict_trie);

    // While splitting the initial string into words, start inserting into the tree
    char *token = strtok(buffer, ",");

    while(token != NULL) {
        // Insert every seperate word
        trie_insert(&dict_trie);
        token = strtok(NULL, ",");
    }
    


    // Write the tree into a binary file


    trie_delete(&dict_trie);
    free(buffer);
}
