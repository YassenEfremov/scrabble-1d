#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/trie.h"
#include "../../libs/file_contents_to_string.h"


// ============================================================================================= //


void trieWriteBinary(struct node_t *root, FILE **trie_bin) {
    // (recursive)

    // Base case
    if(root == NULL) return;

    for(int i = 0; i < 26; i++) {
        // Go through every child of every node
        if(root->children[i] != NULL) {
            trieWriteBinary(root->children[i], trie_bin);
            
            // Write the node to the file
            fwrite(root, sizeof(struct node_t), 1, *trie_bin);
        }
    }
}


void trieGenerate(char *buffer) {

    // Start constructing the trie
    //struct node_t dict_trie_root;
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


    // Write the tree into a binary file
    FILE *trie_bin = fopen("../bin/trie.bin", "wb");

    // Treverse the trie and write it to a binary file (recursively)
    trieWriteBinary(&dict_trie_root, &trie_bin);


    fclose(trie_bin);
    //trie_delete(&dict_trie_root);    // free the memory for the trie      // TEMPORARY
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
