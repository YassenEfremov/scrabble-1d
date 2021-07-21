/* Definitions for functions declared in trie.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"


/* ============================================================================================= */


struct node_t *trie_create_node() {

    struct node_t *new_node = (struct node_t *)malloc(sizeof(struct node_t));;
    
    for(int i = 0; i < 26; i++) new_node->children[i] = NULL;
    new_node->isEndOfWord = 0;
        
    return new_node;
}

/*
bool search(struct node_t *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct node_t *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);

        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}
*/
/*
int check_trie_temp(char *word) {
	// DON'T DELETE THIS CODE, it might be needed in the future
	// Check if the word is in the trie structure (not the json file)

    struct node_t *temp = &dict_trie_root;
    int letter_index;

    for(int level = 0; level < strlen(word); level++) {
        letter_index = (int)(word[level] - 'a');

        if(temp->children[letter_index] == NULL) return 0;
        printf("%p ", temp->children[letter_index]);
        temp = temp->children[letter_index];
    }

    printf("(%d)", (temp != NULL && temp->isEndOfWord));
    return (temp != NULL && temp->isEndOfWord);
}
*/


/* --------------------------------------------------------------------------------------------- */


void trie_insert(struct node_t *root, char *word) {

    int word_length = strlen(word);
    struct node_t *temp = root;

    for(int level = 0; level < word_length; level++) {
        // Go down the trie and search for the letters in the word

        int letter_index = (int)word[level] - (int)'a';    // this gets the letters index in the alphabet

        if(temp->children[letter_index] == NULL) {
            // The letter isn't in the tree => insert it
            temp->children[letter_index] = trie_create_node();
        }
        temp = temp->children[letter_index];    // move to the next letter
    }

    temp->isEndOfWord = 1;
}


/* --------------------------------------------------------------------------------------------- */


void trie_delete(struct node_t *root) {

    // Base case
    if(root == NULL) return;

    for(int i = 0; i < 26; i++) {
        // Go through every child of every node
        if(root->children[i] != NULL) {
            trie_delete(root->children[i]);
            free(root->children[i]);
        }
    }
}
