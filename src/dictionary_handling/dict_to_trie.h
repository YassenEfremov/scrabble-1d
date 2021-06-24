#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/jWrite.h"

#include "../../libs/trie.h"
#include "../../libs/file_contents_to_string.h"


// ============================================================================================= //
//*

void write_call(struct node_t *root, FILE **trie_json, char *buffer) {

    // jwObj_string("a", "oooo");
    for(int i = 0; i < 26; i++){
            
        char letter[2];
        letter[0] = ('a' + i);
        
        if(root->children[i] != NULL){
            
            jwObj_object(letter);
            write_call(root->children[i], trie_json, buffer);
            if(root->children[i]->isEndOfWord == 1){
                jwObj_int("isEndOfWord", 1);
            }
            else{
                jwObj_int("isEndOfWord", 0);
            }
            jwEnd();

        }
    
    }
    return;

}

int trieWriteJson(struct node_t *root, FILE **trie_json) {

    char buffer[300000];   // the size of the array causes problems when writing to file

    jwOpen(buffer, sizeof(buffer), JW_OBJECT, JW_PRETTY);

    write_call(root, trie_json, buffer);

    
    int err_code = jwClose();

    fwrite(buffer, sizeof(buffer), 1, *trie_json);

    return err_code;
}


void trieGenerate(char *buffer) {

    // Start constructing the trie
    //struct node_t dict_trie_root;     // FOR THE TIME BEING, THE TRIE IS DEFINED GLOBALLY IN trie.h
    // Set the default values for root
    for(int i = 0; i < 26; i++) dict_trie_root.children[i] = NULL;
    dict_trie_root.isEndOfWord = 0;

    // While splitting the initial string into words, start inserting into the tree
    char *token = strtok(buffer, "\n");
    
    while(token != NULL) {
        // Insert every seperate word
        trie_insert(&dict_trie_root, token);
        token = strtok(NULL, "\n");  // get the next word
    }


    // TO BE CHANGED !                                      // TO DO

    // Write the tree into a json file

    FILE *trie_json = fopen("../json/trie.json", "w");

    if(!trie_json) {
        // Catch any exeptions
        printf("\nTrie json doesn't exist!");
        return;
    }

    // Treverse the trie and write it to a json file
    trieWriteJson(&dict_trie_root, &trie_json);


    fclose(trie_json);
    //trie_delete(&dict_trie_root);    // free the memory for the trie      // TEMPORARILY COMMENTED
}


extern void dictToTrie() {

    // Open the dictionary for reading 
    FILE *dict  = fopen("./dictionary_handling/dictionary.txt", "r");

    if(!dict) {
        // Catch any exeptions
        printf("\nDictionary doesn't exist!");
        return;
    }

    // Copy its contetnts into a buffer
    char *buffer = copyFileContentsToString(&dict);

    fclose(dict);

    // Pass the buffer to generate the tree
    trieGenerate(buffer);                                   // TEMPORARY

    free(buffer);   // free the momory for the buffer
}
