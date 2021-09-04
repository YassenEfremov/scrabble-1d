/* Functions that check for game files. */


/* ============================================================================================= */

#ifndef FILE_CHECKS_H	// Include guard
#define FILE_CHECKS_H

/* --------------------------------------------------------------------------------------------- */


/* 
 * Check if the game settings config file exists.
 * If not -> create it with the default settings.
 * - return 0 if it exists
 * - return 2 for missing file
 */
int check_config_file(void);

/*
 * Check if the dictionary file exists.
 * If not -> create it with the default set of words.
 * - return 0 if it exists
 * - return 2 for missing file
 */
int check_dict_file(void);

/*
 * Check if the trie json file exists.
 * If not -> generate it from the dictionary.
 * - return 0 if it exists
 * - return 2 for missing file
 */
int check_trie_json_file(void);


/* --------------------------------------------------------------------------------------------- */

#endif		// End of include guard