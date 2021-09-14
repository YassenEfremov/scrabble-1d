/*
 *	Dictionary handling functions
 *
 *  Copyright (C) 2021 Yassen Efremov
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "dict_handling.h"

#include "libs/file_paths.h"
#include "libs/trie.h"
#include "libs/ui_util/ui_util.h"

#include <glib.h>
#include <glib/gstdio.h>    // g_fopen
#include <json-glib/json-glib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>      // isspace


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

    buffer[file_size] = '\0';

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

    gchar *dict_file_path = g_build_filename(g_get_user_data_dir(), GAME_DIR, DICT_NAME, (char *)NULL);
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

    return temp_trie_root;
}


/* ============================================================================================= */


void trieBuildJson(struct node_t *root, JsonBuilder *builder) {

    for(int i = 0; i < 26; i++){
        // convert the current letter to a string
        char letter[2] = "";
        letter[0] = ('a' + i);
        
        if(root->children[i] != NULL) {
            
            json_builder_set_member_name(builder, letter);
            json_builder_begin_object(builder);
            if(root->children[i]->isEndOfWord == 1) {
                json_builder_set_member_name(builder, "isEndOfWord");  
                json_builder_add_int_value(builder, 1);
            }/*else {
                json_builder_set_member_name(builder, "isEndOfWord");  
                json_builder_add_int_value(builder, 0);
            }*/
            trieBuildJson(root->children[i], builder);  // do the same for the children
            json_builder_end_object(builder);
        }
    }
}


/* --------------------------------------------------------------------------------------------- */


int trieToJson(struct node_t *trie_root) {

    gchar *dict_json_path = g_build_filename(g_get_user_data_dir(), GAME_DIR, DICT_JSON_NAME, (char *)NULL);


    JsonBuilder *builder = json_builder_new();

    // Build the json structure
    json_builder_begin_object(builder);  // open root object
    trieBuildJson(trie_root, builder);
    json_builder_end_object(builder);  // close root object


    JsonGenerator *generator = json_generator_new();
    GError *dict_json_error = NULL;
    // generator settings
    json_generator_set_pretty(generator, 1);
    json_generator_set_indent(generator, 4);

    // Write it to the json file
    JsonNode *root = json_builder_get_root(builder);
    json_generator_set_root(generator, root);
    json_generator_to_file(generator, dict_json_path, &dict_json_error);


    json_node_free(root);
    g_object_unref(generator);
    g_object_unref(builder);
    g_free(dict_json_path);

    return 0;
}


/* ============================================================================================= */


int checkTrie(char *word) {

    gchar *dict_json_path = g_build_filename(g_get_user_data_dir(), GAME_DIR, DICT_JSON_NAME, (char *)NULL);


    int is_end_of_word, i;	// stores the value of isEndOfWord
    char letter[2] = "";

    JsonParser *parser = json_parser_new();
    GError *dict_json_error = NULL;

    json_parser_load_from_file(parser, dict_json_path, &dict_json_error);


    JsonReader *reader = json_reader_new(json_parser_get_root(parser));

    // go through the letters of the word and check if they exist in the json file
    for(i = 0; i < strlen(word); i++) {
        // convert the current letter to a string
        letter[0] = word[i];
        if(!json_reader_read_member(reader, letter)) {
            // the letter wasn't in the object -> the word isn't in the dictionary
            is_end_of_word = 0;
            break;
        }   
    }
    // finally, check if the word is actually valid
    json_reader_read_member(reader, "isEndOfWord");
    is_end_of_word = json_reader_get_boolean_value(reader);


    g_object_unref(reader);
    g_object_unref(parser);
    g_free(dict_json_path);

	return is_end_of_word;
}
