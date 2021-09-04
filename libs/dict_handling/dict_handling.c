/* Definitions for functions declared in dict_handling.h */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>      // isspace
#include <glib.h>
#include <glib/gstdio.h>

#include "libs/file_paths.h"
#include "libs/jRead.h"
#include "libs/jWrite.h"
#include "libs/trie.h"
#include "libs/ui_util/ui_util.h"

#include "dict_handling.h"


/* ============================================================================================= */


char *strfcpy(FILE *file) {
    if(!file) {
        message_log("strfcpy: file doesn't exist");
        return NULL;
    }

    // Get the number of letters in the file
    fseek(file, 0L, SEEK_END);   // set file pos. indicator at the end
    long file_size = ftell(file);   // get the value of the pos. indicator = bytes in the file
    rewind(file);  // reset pos. indicator

    // Save them in a buffer
    char *buffer = (char *)g_malloc((file_size + 1) * sizeof(char));
    fread(buffer, file_size, 1, file);

    buffer[file_size] = '\0';   // last char is terminating zero

    return buffer;
}


/* --------------------------------------------------------------------------------------------- */


void strrmspaces(char **str) {

    if(strcmp(*str, "") == 0) return;    // the given string is empty

    // remove all spaces in the front
    while(isspace(**str)) (*str)++;

    // remove all spaces in the back
    char *str_end = *str + strlen(*str) - 1;
    while(str_end > *str && isspace(*str_end)) str_end--;

    // terminating null
    *(str_end + 1) = '\0';
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


struct node_t *dictToTrie(void) {

    gchar *dict_file_path =  g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, DICT_NAME, (char *)NULL);
    FILE *dict = g_fopen(dict_file_path, "r");
    if(!dict) {
        message_log("Error: Dictionary missing!");
        g_free(dict_file_path);
        return NULL;
    }

    char *dict_contents = strfcpy(dict);    // Copy the dictionary contents to a string buffer

    fclose(dict);

    // Generate the trie from the buffer
    struct node_t *temp_trie_root = trieGenerate(dict_contents);

    g_free(dict_contents);
    g_free(dict_file_path);

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

    gchar *dict_json_path =  g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, DICT_JSON_NAME, (char *)NULL);
    FILE *trie_json = g_fopen(dict_json_path, "w");

    // Treverse the trie and write it to a json file

	// First create a json string buffer
    char json_string[300000];   		//! the size of the array sometimes causes problems when writing to file

    jwOpen(json_string, sizeof(json_string), JW_OBJECT, JW_PRETTY);	// Open the root json node
    trieWriteJson(trie_root);							// Write the trie to the json string
    int err_code = jwClose();										// Close the root json node (return error code)

    fwrite(json_string, sizeof(json_string), 1, trie_json);	// Write the json string to the json file

    fclose(trie_json);
    g_free(dict_json_path);

    return err_code;
}


/* ============================================================================================= */


int checkTrie(char *word) {

    if(strlen(word) < 2) return -1;

    gchar *data_dir = g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, (char *)NULL);
    gchar *dict_json_path =  g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, DICT_JSON_NAME, (char *)NULL);

    //if(!g_file_test(dict_json_path, G_FILE_TEST_EXISTS)) return 2;  // If it doesn't exist AGAIN

    FILE *trie_json = g_fopen(dict_json_path, "r");
	char *json_string = strfcpy(trie_json);
	fclose(trie_json);


	int is_end_of_word;	// stores the value of isEndOfWord
    
    char *ending = "isEndOfWord'";
    int length = strlen(word);
    char *bigger_word = (char *)g_malloc(5 * sizeof(char));	// 4 + 1 byte for '\0'
	//int new_word_len = 5;
    
	// Create the query string using the entered word
    int j = 0;
    for(int i = 0; i <= 4 * length; i+= 4, j++) {

        bigger_word[i] = '{';
        bigger_word[i+1] = '\'';	// char ' (single quote)
        bigger_word[i+2] = word[j];
        bigger_word[i+3] = '\'';	//char ' (single quote)
		bigger_word[i+4] = '\0';

        char *new_word = g_realloc(bigger_word, (strlen(bigger_word) + 5) * sizeof(char));
		bigger_word = new_word;
    }
    
    char *final_word = g_realloc(bigger_word, (strlen(bigger_word) + strlen(ending) + 1) * sizeof(char)); 
    strcat(final_word, ending);

    is_end_of_word = jRead_int(json_string, final_word, NULL);

	g_free(json_string);
	g_free(final_word);

	return is_end_of_word;
}
