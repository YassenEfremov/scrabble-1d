/* Game logic functions */


/* ============================================================================================= */


/*
 * > Called by letter_generation
 * Enter a word and check if it's valid.
 * *will be changed*
 */
int enter_and_check(char rand_letters[], int letters, int* points);

/*
 * > Called by startGame
 * Function that generates random letters for one round and prints them.
 * *will be changed*
 */
void letter_generation(int letters, int *points);


/* Scrabble: Start a game. */
void startGame(int letters, int rounds);
