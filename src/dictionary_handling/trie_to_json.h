#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../libs/jWrite.h"

#include "../../libs/trie.h"
#include "../../libs/file_contents_to_string.h"


// ============================================================================================= //


void trieWriteJson(struct node_t *root, FILE **trie_json) {

    // jwObj_string("a", "oooo");
    for(int i = 0; i < 26; i++){
            
        char letter[2] = "";
        letter[0] = ('a' + i);
        
        if(root->children[i] != NULL){
            
            jwObj_object(letter);
            if(root->children[i]->isEndOfWord == 1){
                jwObj_int("isEndOfWord", 1);
            }
            else{
                jwObj_int("isEndOfWord", 0);
            }
            trieWriteJson(root->children[i], trie_json);
            jwEnd();
        }
    }
}


int trieToJson(struct node_t *trie_root) {

    // Write the tree to a json file

    FILE *trie_json = fopen("../json/trie.json", "w");

    if(!trie_json) {
        // Catch any exeptions
        printf("\nError: Trie json file missing!");
        return 2;
    }



    // Treverse the trie and write it to a json file

	// First create a json string buffer
    char json_string[300000];   		//! the size of the array sometimes causes problems when writing to file

    jwOpen(json_string, sizeof(json_string), JW_OBJECT, JW_PRETTY);	// Open the root json node
    trieWriteJson(trie_root, &trie_json);							// Write the trie to the json string
    int err_code = jwClose();										// Close the root json node (return error code)

    fwrite(json_string, sizeof(json_string), 1, trie_json);	// Write the json string to the json file


    fclose(trie_json);

    return err_code;
}
