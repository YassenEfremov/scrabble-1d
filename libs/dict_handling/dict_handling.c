/* Definitions for functions declared in dict_handling.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>      // atoi
#include "dict_handling.h"

#include "libs/jWrite.h"
#include "libs/trie.h"


// ============================================================================================= //


char *strfcpy(FILE *file) {
    if(!file) {
        printf("strfpcy: file doesn't exist");
        return NULL;
    }

    // Get the number of letters in the file
    fseek(file, 0L, SEEK_END);   // set file pos. indicator at the end
    long file_size = ftell(file);   // get the value of the pos. indicator = bytes in the file
    rewind(file);  // reset pos. indicator

    // Save them in a buffer
    char *buffer = (char *)malloc((file_size + 1) * sizeof(char));
    fread(buffer, file_size, 1, file);

    buffer[file_size] = '\0';   // last char is terminating zero

    return buffer;
}


/* --------------------------------------------------------------------------------------------- */


int addWordToDict() {

    // Open the dictionary for appending and reading
    FILE *dict = fopen("../../config/dictionary.txt", "a+");

    if(!dict) {
        // Catch any exeptions
        printf("\nError: Dictionary missing!");
        return 2;
    }

    // Copy its contetnts into a buffer
    char *dict_string = strfcpy(dict);



    // Take user input

    char word[46];      // Longest english word is 45 letters! 
    printf("New word (Enter 9 to cancel): ");
    scanf("%s", word);

    // If we enter 9 => end round
    if(strcmp(word, "9") == 0){
        fclose(dict);
        free(dict_string);
        system("clear");
		return -1;
	}


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
        scanf("%s", word);  // the MAXIMUM word lengh is 45
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


/* --------------------------------------------------------------------------------------------- */


struct node_t *trieGenerate(char *dict_contents) {

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


/* --------------------------------------------------------------------------------------------- */


struct node_t *dictToTrie() {

    FILE *dict = fopen("../../config/dictionary.txt", "r");

    if(!dict) {
        // Catch any exeptions
        printf("\nError: Dictionary missing!");
        return NULL;
    }

    // Copy the dictionary contents to a string buffer
    char *dict_contents = strfcpy(dict);

    fclose(dict);   // we don't need the file anymore, close it

    // Generate the trie from the buffer
    struct node_t *temp_trie_root = trieGenerate(dict_contents);


    free(dict_contents);   // free the momory for the buffer

    return temp_trie_root;  // return the generated trie
}


/* ============================================================================================= */


void trieWriteJson(struct node_t *root) {

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
            trieWriteJson(root->children[i]);
            jwEnd();
        }
    }
}


/* --------------------------------------------------------------------------------------------- */


int trieToJson(struct node_t *trie_root) {

    FILE *trie_json = fopen("../../json/trie.json", "w");

    if(!trie_json) {
        // Catch any exeptions
        printf("\nError: Trie json file missing!");
        return 2;
    }



    // Treverse the trie and write it to a json file

	// First create a json string buffer
    char json_string[300000];   		//! the size of the array sometimes causes problems when writing to file

    jwOpen(json_string, sizeof(json_string), JW_OBJECT, JW_PRETTY);	// Open the root json node
    trieWriteJson(trie_root);							// Write the trie to the json string
    int err_code = jwClose();										// Close the root json node (return error code)

    fwrite(json_string, sizeof(json_string), 1, trie_json);	// Write the json string to the json file


    fclose(trie_json);

    return err_code;
}
