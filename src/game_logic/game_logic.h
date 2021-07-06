/* Game logic functions */


/* ============================================================================================= */

/* Check if the given word is in the json trie.
- return 1 if it is
- return 0 if it isn't */
int check_trie(char *word);     // This function may be moved to trie.h (or dict_handling.h ?)

/* > Called by letter_generation
   Enter a word and check if it's valid.
 * *will be changed* */
int enter_and_check(char rand_letters[], int letters, int* points);

/* > Called by startGame
   Function that generates random letters for one round and prints them.
 * *will be changed* */
void letter_generation(int letters, int *points);


/* Scrabble: Start a game. */
void startGame(int letters, int rounds);
