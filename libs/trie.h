/* Functions for managing the trie structure */


/* ============================================================================================= */

#ifndef TRIE_H	// Include guard
#define TRIE_H

/* --------------------------------------------------------------------------------------------- */
/* Structure definitions */


/* Trie node structure.
   Contains:
- array of 26 pointers to trie nodes
- flag indicating whether the current node is an end of word */
struct node_t {
	struct node_t *children[26];
	int isEndOfWord;
};


/* --------------------------------------------------------------------------------------------- */
/* Trie utility functions */


/* > Called by trie_insert
   Create and return a new trie node.

   Note: Returns a DYNAMIC pointer! */
struct node_t *trie_create_node();

/* Insert a new node into the trie structure. */
void trie_insert(struct node_t *root, char *word);

/* Free the memory used by the trie structure.
   
   Note: This does NOT include the trie root node!
   It must be freed MANUALLY! */
void trie_delete(struct node_t *root);


/* --------------------------------------------------------------------------------------------- */

#endif		// End of include guard
