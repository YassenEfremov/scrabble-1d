#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/trie.h"
#include "../../libs/file_contents_to_string.h"


// ============================================================================================= //


struct node_t *trieGenerate(char *dict_contents) {

    // Start constructing the trie

    struct node_t *temp_trie_root = trie_create_node();

    // While splitting the initial string into words, start inserting into the tree
    char *token = strtok(dict_contents, "\n");
    while(token != NULL) {
        // Insert every seperate word
        trie_insert(temp_trie_root, token);
        token = strtok(NULL, "\n");  // get the next word
    }

    return temp_trie_root;
}


struct node_t *dictToTrie() {

    FILE *dict  = fopen("../config/dictionary.txt", "r");

    if(!dict) {
        // Catch any exeptions
        printf("\nError: Dictionary missing!");
        return NULL;
    }

    // Copy the dictionary contents to a string buffer
    char *dict_contents = copyFileContentsToString(&dict);

    fclose(dict);   // we don't need the file anymore, close it

    // Generate the trie from the buffer
    struct node_t *temp_trie_root = trieGenerate(dict_contents);


    free(dict_contents);   // free the momory for the buffer

    return temp_trie_root;  // return the generated trie
}
