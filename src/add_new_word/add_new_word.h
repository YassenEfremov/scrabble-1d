/* Function for adding a new word */


/* ============================================================================================= */
/* Private functions */

static void refresh_new_word_win(WINDOW *new_word_win, WINDOW *input_win);


/* ============================================================================================= */
/* Public functions */


/*
 * Scrabble: Add a new word to the dictionary file.
 * - returns 0 on success
 */
int addNewWord(void);
