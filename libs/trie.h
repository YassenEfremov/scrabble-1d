#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ============================================================================================= //


struct node_t {
	struct node_t *children[26];
	int isEndOfWord;
};

/*
struct trie_t {
	int size;
	struct node_t *root;
};
*/

// ============================================================================================= //

/*
extern void trie_init(struct trie_t *trie) {

	trie->size = 0;
	trie->root = NULL;
}
*/

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

extern void trie_insert(struct node_t *root, char *word) {
/*
    if(trie->root == NULL) {
        // Trie is empty
        trie->root = trie_create_node();

    }else {*/

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
    //}
}


extern void trie_delete(struct node_t *root) {

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
