/* Definitions for functions declared in game_logic.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>		// atoi
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "libs/dict_handling/dict_handling.h"
#include "libs/ui_util/ui_util.h"

#include "game_logic.h"


/* ============================================================================================= */


int enter_and_check(char rand_letters[], int letters, int* points) {

	char word[letters];
	printf("\nEnter word (Enter 9 to skip round):  ");
	scanf("%s", word);
	
	for(int i = 0; i < strlen(word); i++){
		word[i] = tolower(word[i]);
	}
	
	// If we enter 9 => end round
	if(strcmp(word, "9") == 0){
		printf("> Skipped\n");
		return -1;
	}

	// Copy the random letters to a temporary array so the original one won't change 
	char temp[letters + 1];
	strcpy(temp, rand_letters);



	// Check if the word is composed of the available letters
	int count = 0;
	for(int i = 0; word[i] != '\0'; i++){

		int flag = 0;
	
		for(int j = 0; temp[j] != '\0'; j++){
			if(word[i] == temp[j]){
				count++;
				temp[j] = '-';	// set the checked letter to '-' so double letters won't cause problems
				flag = 1;
				break;
			}
		}
		
		if(flag == 0){
			count = 0;
			printf("Try again(or skip) \n");	
			return 0;
		}
	}

/*
	CHCCK TRIE CAUSES COMPILATION PROBLEMS!

	// Check if the entered word is in the dict_trie
	if(checkTrie(word) == 0) {
		// If it isn't => round points are 0
		count = 0;
		printf("This word is not in the dictionary!\n");
		printf("Try again(or skip)\n");
		return 0;
	}
*/

	*points += count;
	printf("\n\nTotal points: %d\n", *points);

	return 1;
}


/* --------------------------------------------------------------------------------------------- */


void letter_generation(int letters, int *points) {
    char random_letter;
    char array[letters + 1];
    char vowels[] = {'a','e','i','o','u','y'};
	
    srand(time(0));	// set the seed for the random number generation
    
    random_letter = vowels[rand() % 6];
    array[0] = random_letter;	// the first letter is a random vowel
	printf("\n %c ", array[0]);
	
    for(int i = 0; i < letters - 1; i++){
        // formula for generating a random letter -> (rand() % (upper - lower + 1)) + lower;
        random_letter = (rand() % (122 - 97 + 1)) + 97; 
        array[i+1] = random_letter;
        printf(" %c ", array[i+1]);
    }
	printf("\n");
    
	array[letters + 1] = '\0';
	
	int flag;
	do{
	
		flag = enter_and_check(array, letters, points);
		
	}while(flag == 0);
}


/* --------------------------------------------------------------------------------------------- */


void startGame(int letters, int rounds) {

	erase();
	refresh();

	// Game variables

	int points = 0;


	// Predefined data

	int rows, cols;
	getmaxyx(stdscr, rows, cols);	// get the dimentions of the main screen

	int key;


	/* ----------------------------------------------------------------------------------------- */
	/* Setup */

	// Game window
	WINDOW *game_win = newwin(0.8 * rows, 0.8 * cols,	// size is 80% of the screen
							  0.1 * rows, 0.1 * cols);	// start is at 10% of the screen
	box(game_win, 0, 0);


	/* ----------------------------------------------------------------------------------------- */


	// Small title
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	mvaddstr(game_win->_begy - 1, game_win->_begx + getmaxx(game_win)/2 - 4, "SCRABBLE");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));

	// Reposition message window
	mvwin(msg_win, game_win->_begy + 0.8 * rows + 1, cols/2 - MSG_LEN/2);

	// Refresh everything
	wrefresh(game_win);
	wrefresh(msg_win);


	/* ----------------------------------------------------------------------------------------- */
	/* Game loop */

	// Round loop
	for(int r = 0; r < rounds; r++, points += 5) {
		
		wattron(game_win, A_BOLD);

		// Print current round
		mvwprintw(game_win, 1, 1, "Round #%d", r+1);
		if(r + 1 == rounds) {
			// Last round
			wattron(game_win, COLOR_PAIR(3));
			mvwprintw(game_win, 2, getmaxx(game_win)/2 - 6, "FINAL ROUND!");
			wattroff(game_win, COLOR_PAIR(3));
		}

		// Print current points
		mvwprintw(game_win, 1, getmaxx(game_win) - 11, "Points: %d", points);

		wattroff(game_win, A_BOLD);


		//> generate random letters

		wrefresh(game_win);

		// Read input from the user
		do {
			key = getch();

			switch(key) {
				/* All possible actions in-game */

				default:
					//mvwprintw(game_win, 3, 3, "%c", key);
					break;
			}
			wrefresh(game_win);

		}while(key != 10);	// Enter key

        //> check the entered word

		if(r + 1 == rounds) {
			// This is the last round -> display ending screen

			wattron(game_win, A_BOLD);

			werase(game_win);
			box(game_win, 0, 0);
			wattron(game_win, COLOR_PAIR(4));
			mvwprintw(game_win, 2, getmaxx(game_win)/2 - 5, "GAME OVER!");
			wattroff(game_win, COLOR_PAIR(4));

			mvwprintw(game_win, getmaxy(game_win)/2, getmaxx(game_win)/2 - 7, "Final score: %d", points);

			wattroff(game_win, A_BOLD);

			mvwprintw(game_win, getmaxy(game_win) - 2, getmaxx(game_win)/2 - 13, "Press ENTER to continue...");

			wrefresh(game_win);
			getch();
		}
	}

	delwin(game_win);
}
