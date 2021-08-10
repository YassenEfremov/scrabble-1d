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
/* Private functions */


static int check_word(char *word, char rand_letters[]) {

	int word_points = 0;

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


static void refresh_game_win(int rows, int cols,
							 WINDOW *game_win, WINDOW *rand_letters_win, WINDOW *input_win,
							 int letters, int rounds, int r, int points) {

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, rows, cols);	// get the new dimentions of the main screen

	// If the size of the terminal is smaller than (39, 5) everything glitches out!				// TO DO
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	mvaddstr(game_win->_begy - 1, game_win->_begx + getmaxx(game_win)/2 - 4, "SCRABBLE");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));

	werase(game_win);
	wresize(game_win, 0.8 * rows, 0.8 * cols);
	mvwin(game_win, 0.8 * rows, 0.8 * cols);
	box(game_win, 0, 0);

	

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

	mvwin(rand_letters_win,
		  game_win->_begy + getmaxy(game_win)/3,								 // y position
		  game_win->_begx + (getmaxx(game_win) - (letters + (letters - 1)))/2);
	mvwin(msg_win, game_win->_begy + 0.8 * rows - 3, cols/2 - MSG_LEN/2);

    // WHEN THE SCREEN IS TOO SMALL SOME ELEMENTS DISAPPEAR										// TO DO

	// Refresh the necessay elements
	refresh();
	wrefresh(game_win);
	wrefresh(rand_letters_win);
}


/* ============================================================================================= */
/* Public functions */


void startGame(int letters, int rounds) {

	erase();
	refresh();

	// Game variables

	int points = 0;			// total game points
	int word_points = 0;	// points per word
	char rand_letters[letters];
	char *raw_input_str;
	//char *input_str;
	int is_valid;


	// Predefined data

	int rows, cols;
	getmaxyx(stdscr, rows, cols);	// get the dimentions of the main screen

    int num_of_fields = 1;

	int key;


	/* ----------------------------------------------------------------------------------------- */
	/* Game windows */

	// Game window
	WINDOW *game_win = newwin(0.8 * rows, 0.8 * cols,	// size is 80% of the screen
							  0.1 * rows, 0.1 * cols);	// start is at 10% of the screen
	// Random letters window
 	WINDOW *rand_letters_win = newwin(1, letters + (letters - 1),	// size
									  game_win->_begy + getmaxy(game_win)/3,								 // y position
									  game_win->_begx + (getmaxx(game_win) - (letters + (letters - 1)))/2);	 // x position
	// Input window
	WINDOW *input_win = newwin(1, getmaxx(game_win) - 2, rand_letters_win->_begy + 5, game_win->_begx + 1);

	box(game_win, 0, 0);


	/* ----------------------------------------------------------------------------------------- */
	/* Input form */

    // Fields (only 1)
    FIELD **input_field = (FIELD **)calloc(num_of_fields + 1, sizeof(FIELD *));
	input_field[0] = new_field(1, getmaxx(game_win) - 2, 0, 0, 0, 1);
	input_field[num_of_fields] = NULL;

    // Field settings
    set_field_type(input_field[0], TYPE_ALPHA, 2);
	field_opts_off(input_field[0], O_AUTOSKIP);
	set_field_just(input_field[0], JUSTIFY_CENTER);		// DOESN'T WORK

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
	mvwin(msg_win, game_win->_begy + 0.8 * rows - 3, cols/2 - MSG_LEN/2);
	wrefresh(msg_win);


	/* ----------------------------------------------------------------------------------------- */
	/* Game loop */

	char input_str[getmaxx(game_win) - 1];
	raw_input_str = field_buffer(input_field[0], 0);   // set input string buffer

	// Round loop
	for(int r = 0; r < rounds; r++) {
		is_valid = 0;
		
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
			key = getch();
			werase(msg_win);

			switch(key) {
				/* All possible actions in the field */

                case KEY_BACKSPACE: // Delete from the field (handle all backspace chars)
                case '\b':
                case 127:
					form_driver(input_form, REQ_VALIDATION);	// update field buffer
					raw_input_str = field_buffer(input_field[0], 0);	// get the field string

					if(strchr(raw_input_str, ' ') == NULL) {
						form_driver(input_form, REQ_DEL_CHAR);
						break;
					}
                    form_driver(input_form, REQ_DEL_PREV);
                    break;

				case 10:  // Enter key (validate input)
					form_driver(input_form, REQ_VALIDATION);  // update field buffer
					raw_input_str = field_buffer(input_field[0], 0);	// get the field string

					int i;	// this is the size of the actual word (whitout the whitespaces)
					for(i = 0; raw_input_str[i] != ' '; i++);
					if(i == 0) {  // field is empty
						is_valid = 0;
						message_log("Type something...");
						wrefresh(msg_win);
						break;
					}

					// copy only the entered word to a new buffer (without whitespaces)
					strncpy(input_str, raw_input_str, i);
					input_str[i] = '\0';
					//mvprintw(0, 0, "!%s!", input_str);

					// Check if the word is valid and give points accordingly
					word_points = check_word(input_str, rand_letters);
					switch(word_points) {
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
					message_log("Round skipped >>");
					break;

                case KEY_RESIZE:  // on window resize
                    refresh_game_win(rows, cols, game_win, rand_letters_win, input_win, letters, rounds, r, points);
                    break;

				default:  // write to the field
					// Move the input window
					//> resize the field
					//> move the field
					form_driver(input_form, key);
					break;
			}
			wrefresh(input_win);
			wrefresh(msg_win);

		}while(!is_valid && key != ' ');	// Enter or space key


		if(r + 1 == rounds) {
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
			while(getch() != 10); // Enter key
		}
	}

	exitForm(&input_form, &input_field, num_of_fields);
	delwin(input_win);
	delwin(rand_letters_win);
	delwin(game_win);
}
