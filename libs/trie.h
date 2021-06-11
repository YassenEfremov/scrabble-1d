#include <stdio.h>
#include <stdlib.h>


// ============================================================================================= //


struct node_t {
	struct node_t *children[26];
	int isEndOfWord;
};


struct trie_t {
	int size;
	struct node_t *root;
};


// ============================================================================================= //


extern void trie_init(struct trie_t *trie) {
	trie->size = 0;
	trie->root = NULL;
}


extern void trie_insert(struct trie_t *trie) {

	// insert a string into the trie
}


extern void trie_delete(struct trie_t *trie) {

	// frees the memory allocated for the tree
}
