/* Definitions for functions declared in game_logic.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "libs/dict_handling/dict_handling.h"
#include "libs/ui_util/ui_util.h"

#include "game_logic.h"


/* ============================================================================================= */
/* Private functions */


static int check_word(char *word, char rand_letters[]) {

	int word_points = 0;

	if(strcmp(word, "") == 0) return -2;	// nothing was typed (string is empty)

	// Copy the random letters to a temporary array so the original one won't change 
	char temp_word[strlen(word) + 1];
	temp_word[strlen(word) + 1] = '\0';
	strcpy(temp_word, rand_letters);


	// Check if the word is composed up of the available letters
	for(int i = 0; word[i] != '\0'; i++) {

		int match = 0;
	
		for(int j = 0; temp_word[j] != '\0'; j++) {
			if(word[i] == temp_word[j]) {
				word_points++;
				temp_word[j] = '-';	// set the checked letter to '-' so double letters won't cause problems
				match = 1;
				break;
			}
		}
		
		// No letter matched
		if(match == 0) return -1;
	}

	// Check if the entered word is in the dictionary
	if(checkTrie(word) == 0) {
		word_points = 0;	// if it's not -> 0 points
		return 0;
	}

	return word_points;
}


/* --------------------------------------------------------------------------------------------- */


static void get_rand_letters(char letters_array[], int letters) {

    char random_letter;
    char vowels[] = {'a','e','i','o','u','y'};
	
    srand(time(0));	// set the seed for the random number generation
    
    random_letter = vowels[rand() % 6];
    letters_array[0] = random_letter;	// the first letter is a random vowel
	
    for(int i = 0; i < letters - 1; i++){
        // formula for generating a random letter -> (rand() % (upper - lower + 1)) + lower;
        random_letter = (rand() % (122 - 97 + 1)) + 97; 
        letters_array[i+1] = random_letter;
    }
}


/* --------------------------------------------------------------------------------------------- */


static void refresh_game_win(WINDOW *game_win, WINDOW *rand_letters_win, WINDOW *input_win,
							 int letters, int rounds, int r, int points) {

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, term_rows, term_cols);	// get the new dimentions of the main screen

	// This comes first because the title depends on it
	werase(game_win);
	wresize(game_win, 0.8 * term_rows, 0.8 * term_cols);
	mvwin(game_win, 0.1 * term_rows, 0.1 * term_cols);
	box(game_win, 0, 0);

	// If the size of the terminal is smaller than (39, 5) everything glitches out!				// TO DO
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	mvaddstr(game_win->_begy - 1, game_win->_begx + getmaxx(game_win)/2 - 4, "SCRABBLE");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));

	if(r == rounds) {
		// Game has ended -> display ending screen

		wattron(game_win, A_BOLD);

		wattron(game_win, COLOR_PAIR(4));
		mvwprintw(game_win, 2, getmaxx(game_win)/2 - 5, "GAME OVER!");
		wattroff(game_win, COLOR_PAIR(4));

		mvwprintw(game_win, getmaxy(game_win)/2, getmaxx(game_win)/2 - 7, "Final score: %d", points);
		wattroff(game_win, A_BOLD);


		mvwprintw(game_win, getmaxy(game_win) - 2, getmaxx(game_win)/2 - 13, "Press ENTER to continue...");

		refresh();
		wrefresh(game_win);
		werase(msg_win);
		return;
	}
	

	wattron(game_win, A_BOLD);

	// Print current round
	mvwprintw(game_win, 1, 2, "Round %d / %d", r+1, rounds);
	if(r + 1 == rounds) {
		// Last round
		wattron(game_win, COLOR_PAIR(3));
		mvwprintw(game_win, 2, getmaxx(game_win)/2 - 6, "FINAL ROUND!");
		wattroff(game_win, COLOR_PAIR(3));
	}

	// Print current points
	mvwprintw(game_win, 1, getmaxx(game_win) - 11, "Points: %d", points);

	wattroff(game_win, A_BOLD);


	mvwin(input_win, rand_letters_win->_begy + 5, game_win->_begx + (getmaxx(game_win) - INPUT_FLD_LEN)/2);	

	mvwin(rand_letters_win,
		  game_win->_begy + getmaxy(game_win)/3,								 // y position
		  game_win->_begx + (getmaxx(game_win) - (letters + (letters - 1)))/2);
	mvwin(msg_win, game_win->_begy + 0.8 * term_rows - 3, term_cols/2 - MSG_LEN/2);

    // WHEN THE SCREEN IS TOO SMALL SOME ELEMENTS DISAPPEAR										// TO DO

	// Refresh the necessay elements
	refresh();
	wrefresh(game_win);
	wrefresh(rand_letters_win);
}


/* ============================================================================================= */
/* Public functions */


void startGame(int letters, int rounds) {

	clear();
	refresh();
	getmaxyx(stdscr, term_rows, term_cols);	// get the dimentions of the terminal


	// Variables

	int points = 0;			// total game points
	int word_points = 0;	// points per word

	char rand_letters[letters];
	char *input_str = NULL;
    int fld_count = 1;

	int is_valid, is_skipped = 1;
	int key;


	/* ----------------------------------------------------------------------------------------- */
	/* Game windows */

	// Game window
	WINDOW *game_win = newwin(0.8 * term_rows, 0.8 * term_cols,	// size is 80% of the screen
							  0.1 * term_rows, 0.1 * term_cols);	// start is at 10% of the screen
	// Random letters window
 	WINDOW *rand_letters_win = newwin(1, letters + (letters - 1),	// size
									  game_win->_begy + getmaxy(game_win)/3,								 // y position
									  game_win->_begx + (getmaxx(game_win) - (letters + (letters - 1)))/2);	 // x position
	// Input window
	WINDOW *input_win = newwin(1, INPUT_FLD_LEN,
							   rand_letters_win->_begy + 5, game_win->_begx + (getmaxx(game_win) - INPUT_FLD_LEN)/2);

	box(game_win, 0, 0);


	/* ----------------------------------------------------------------------------------------- */
	/* Input form */

    // Fields (only 1)
    FIELD **input_field = (FIELD **)calloc(fld_count + 1, sizeof(FIELD *));
	input_field[0] = new_field(1, INPUT_FLD_LEN, 0, 0, 0, 1);
	input_field[fld_count] = NULL;

    // Field settings
    set_field_type(input_field[0], TYPE_ALPHA, 2);		// SOMETIMES DOESN'T WORK
	field_opts_off(input_field[0], O_AUTOSKIP);
	set_field_just(input_field[0], JUSTIFY_CENTER);		// SOMETIMES DOESN'T WORK

    // Form
	FORM *input_form = new_form(input_field);

    // Form settings
    set_form_win(input_form, input_win);
    set_form_sub(input_form, input_win);


	/* ----------------------------------------------------------------------------------------- */


	// Small title
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	mvaddstr(game_win->_begy - 1, game_win->_begx + getmaxx(game_win)/2 - 4, "SCRABBLE");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));

	// Post the form
	post_form(input_form);

	// Reposition message window
	mvwin(msg_win, game_win->_begy + 0.8 * term_rows - 3, term_cols/2 - MSG_LEN/2);
	wrefresh(msg_win);


	/* ----------------------------------------------------------------------------------------- */
	/* Game loop */

	// Round loop
	for(int r = 0; r < rounds; r++) {
		is_valid = 0;
		
		wattron(game_win, A_BOLD);

		// Print current round
		mvwprintw(game_win, 1, 2, "Round %d / %d", r+1, rounds);
		if(r + 1 == rounds) {
			// Last round
			wattron(game_win, COLOR_PAIR(3));
			mvwprintw(game_win, 2, getmaxx(game_win)/2 - 6, "FINAL ROUND!");
			wattroff(game_win, COLOR_PAIR(3));
		}

		// Print current points
		mvwprintw(game_win, 1, getmaxx(game_win) - 11, "Points: %d", points);

		wattroff(game_win, A_BOLD);


		// Generate random letters and print them
		get_rand_letters(rand_letters, letters);
		for(int i = 0; i < letters; i++) {
			mvwprintw(rand_letters_win, 0, i * 2, "%c ", rand_letters[i]);
		}


		form_driver(input_form, REQ_FIRST_FIELD);
		form_driver(input_form, REQ_CLR_FIELD);


		wrefresh(game_win);
		wrefresh(rand_letters_win);
		wrefresh(input_win);


		// Read input from the user
		do {
			if(is_skipped) key = getch();
			is_skipped = 1;
			
			switch(key) {
				/* All possible actions in the field */

                case KEY_BACKSPACE: // Delete from the field (handle all backspace chars)
                case '\b':
                case 127:
					werase(msg_win);
					form_driver(input_form, REQ_VALIDATION);	// update field buffer
					input_str = field_buffer(input_field[0], 0);	// get the field string

					if(strchr(input_str, ' ') == NULL) {
						form_driver(input_form, REQ_DEL_CHAR);
						break;
					}
                    form_driver(input_form, REQ_DEL_PREV);
                    break;

				case 10:  // Enter key (validate input)
					form_driver(input_form, REQ_VALIDATION);  // update field buffer
					input_str = field_buffer(input_field[0], 0);	// get the field string
					strrmspaces(&input_str);	// remove spaces from the string (only back and front)


					// Check if the word is valid and give points accordingly
					word_points = check_word(input_str, rand_letters);

					switch(word_points) {
						case -2:
							is_valid = 0;
							message_log("Type something...");
							wrefresh(msg_win);
							break;

						case -1:
							is_valid = 0;
							message_log("Word isn't valid!");
							break;

						case 0:
							is_valid = 0;
							message_log("Word isn't in dictionary!");
							break;

						default:
							is_valid = 1;
							points += word_points;
							// pirnt different messages depending on the points
							if(word_points >= 1 && word_points <= 4) {
								message_log("Great!");
							}else if(word_points >= 5 && word_points <= 8) {
								message_log("Amazing!");
							}else {
								message_log("* LEGENDARY! *");
							}
							break;
					}
					break;

				case ' ':  // Space key: skip the round
					werase(msg_win);
					message_log("* Press SPACE again to confirm *");
					wrefresh(msg_win);
					key = getch();
					if(key == ' ') {
						// press space again
						is_skipped = 1;  // we skipped the round -> take input on next iteration
						werase(msg_win);
						message_log("Round skipped >>");
					}else {
						// we didn't skip the round -> input is already taken -> don't take input on next iteration
						is_skipped = 0;
					}
					break;

                case KEY_RESIZE:  // on window resize
                    refresh_game_win(game_win, rand_letters_win, input_win, letters, rounds, r, points);
                    break;

				default:  // write to the field
					werase(msg_win);	
					// Move the input window
					//> resize the field			uhh no
					//> move the field
					if(key >= 'a' && key <= 'z') form_driver(input_form, key);     // this check is sometimes needed
					break;
			}
			wrefresh(input_win);
			wrefresh(msg_win);

		}while(!is_valid && key != ' ');	// Read input until ENTER is pressed and the word isn't valid
	}


	// Game has ended -> display ending screen

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
	werase(msg_win);
	while(getch() != 10) {
		// only refresh until enter key is pressed
		refresh_game_win(game_win, rand_letters_win, input_win, letters, rounds, rounds, points);
	}


	exitForm(&input_form, &input_field, fld_count);
	delwin(input_win);
	delwin(rand_letters_win);
	delwin(game_win);
}
