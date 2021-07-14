/* Functions for interacting with the dictionary files and trie structure */


/* ============================================================================================= */

#ifndef FILE_CONTENTS_TO_STRING		// Include guard
#define FILE_CONTENTS_TO_STRING

/* --------------------------------------------------------------------------------------------- */
/* Utility functions */


/* Copy the contents of a file to a string.

   Note: The returned pointer must be freed MANUALLY!

- address of file pointer must be given as an argument
- returns a DYNAMIC pointer to char containing the contents of the given file */
char *strfcpy(FILE *file);


/* --------------------------------------------------------------------------------------------- */
/* Scrabble: dictionary and trie handling functions */


/*
 * Scrabble: Add a new word to the dictionary file.
 * - returns 0 on success
 */
int addWordToDict();


/*
 * > Called by dictToTrie:
 * Create a trie structure from the given string.
 */
struct node_t *trieGenerate(char *dict_contents);
/* Create a trie structure from the dictionary */
struct node_t *dictToTrie();

/* 
 * > Called by trieToJson
 * Create a json string from the given trie structure
 */
void trieWriteJson(struct node_t *root);
/* Write the given trie structure to the json file */
int trieToJson(struct node_t *trie_root);


/*
 * Check if the given word is in the json trie.
 * - return 1 if it is
 * - return 0 if it isn't
 */
int checkTrie(char *word);


/* --------------------------------------------------------------------------------------------- */

#endif		// End of include guard
