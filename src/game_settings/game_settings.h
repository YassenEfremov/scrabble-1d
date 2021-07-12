/* Game settings functions */


/* ============================================================================================= */


/*
 * Change the number of random letters generated per round.
 * *will be changed*
 */
int change_letters(int new_letters, int rounds);

/*
 * Change the number of rounds per game.
 * *will be changed*
 */
int change_rounds(int new_rounds, int letters);


/* Scrabble: Open the game settings. */
void gameSettings(int *letters, int *rounds);