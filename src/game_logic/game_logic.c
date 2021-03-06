/*
 *	Game logic funcions
 *
 *  Copyright (C) 2021 Yassen Efremov
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "game_logic.h"

#include "libs/dict_handling/dict_handling.h"
#include "libs/ui_util/ui_util.h"

#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* ============================================================================================= */
/* Private functions */


static int check_word(char *word) {

	if(strcmp(word, "") == 0) return -1;	// nothing was typed (string is empty)
	if(checkTrie(word) != 1) return 0;
	return strlen(word);
}


/* --------------------------------------------------------------------------------------------- */


static void get_rand_letters(char letters_array[], int letters) {

    char random_letter, random_index;
    char vowels[] = "aiueoy";
	
    srand(time(0));	// set the seed for the random number generation
	
	// Fill the array with random letters
    for(int i = 0; i < letters; i++){
        // formula for generating a random letter -> (rand() % (upper - lower + 1)) + lower;
        random_letter = (rand() % (122 - 97 + 1)) + 97; 
        letters_array[i] = random_letter;
    }

	letters_array[letters] = '\0';

	// Change 1 of the random letters to a vowel
	for(int i = 0; i < 1; i++) {
		random_index = rand() % letters;	// choose a random index
		random_letter = vowels[rand() % 5];	// choose random vowel
		letters_array[random_index] = random_letter;
	}
}


/* --------------------------------------------------------------------------------------------- */


static void refresh_game_win(WINDOW *game_win, WINDOW *rand_letters_win, WINDOW *input_win,
							 int letters, int rounds, int r, int points, char *input_str) {

	// If the size of the terminal is smaller than (39, 5) everything glitches out!				// TO DO
    // WHEN THE SCREEN IS TOO SMALL SOME ELEMENTS DISAPPEAR										// TO DO

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, term_rows, term_cols);	// get the new dimentions of the main screen

	// This is done first because the title depends on it
	wresize(game_win, 0.8 * term_rows, 0.8 * term_cols);
	mvwin(game_win, 0.1 * term_rows, 0.1 * term_cols);
	werase(game_win);
	box(game_win, 0, 0);

	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	mvaddstr(game_win->_begy - 1, game_win->_begx + getmaxx(game_win)/2 - 4, "SCRABBLE");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));

	if(r != rounds) {
		// Game is in progress -> refresh current round

		mvwin(rand_letters_win,
			game_win->_begy + getmaxy(game_win)/3,								 // y position
			game_win->_begx + (getmaxx(game_win) - (letters + (letters - 1)))/2);
		mvwin(input_win, rand_letters_win->_begy + 5, game_win->_begx + (getmaxx(game_win) - strlen(input_str))/2);
		mvwin(msg_win, game_win->_begy + 0.8 * term_rows - 3, term_cols/2 - MSG_LEN/2);


		wattron(game_win, A_BOLD);
		mvwprintw(game_win, 1, 2, "Round %d / %d", r+1, rounds);	// Print current round
		if(r + 1 == rounds) {
			// Last round
			wattron(game_win, COLOR_PAIR(3));
			mvwprintw(game_win, 2, getmaxx(game_win)/2 - 6, "FINAL ROUND!");
			wattroff(game_win, COLOR_PAIR(3));
		}

		// Print current points
		mvwprintw(game_win, 1, getmaxx(game_win) - 11, "Points: %d", points);
		wattroff(game_win, A_BOLD);


		// Refresh the necessary elements
		refresh();
		wrefresh(game_win);
		wrefresh(rand_letters_win);

	}else {

		// Game has ended -> refresh ending screen

		wattron(game_win, A_BOLD);
		wattron(game_win, COLOR_PAIR(4));
		mvwprintw(game_win, 2, getmaxx(game_win)/2 - 5, "GAME OVER!");
		wattroff(game_win, COLOR_PAIR(4));

		mvwprintw(game_win, getmaxy(game_win)/2, getmaxx(game_win)/2 - 7, "Final score: %d", points);
		wattroff(game_win, A_BOLD);

		wattron(game_win, A_ITALIC);
		mvwprintw(game_win, getmaxy(game_win) - 2, getmaxx(game_win)/2 - 13, "Press ENTER to continue...");
		wattroff(game_win, A_ITALIC);


		refresh();
		wrefresh(game_win);
	}
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

	char rand_letters[letters + 1], save_letters[letters + 1];
    int fld_count = 1;
	char *input_str = "";

	int is_valid, take_input = 1;
	int key;


	/* ----------------------------------------------------------------------------------------- */
	/* Game windows */

	// Game window
	WINDOW *game_win = newwin(0.8 * term_rows, 0.8 * term_cols,		// size is 80% of the screen
							  0.1 * term_rows, 0.1 * term_cols);	// position is at 10% of the screen
	box(game_win, 0, 0);

	// Random letters window
 	WINDOW *rand_letters_win = newwin(1, letters + (letters - 1),	// size
									  game_win->_begy + getmaxy(game_win)/3,								 // y position
									  game_win->_begx + (getmaxx(game_win) - (letters + (letters - 1)))/2);	 // x position


	/* ----------------------------------------------------------------------------------------- */
	/* Input form */

	WINDOW *input_win = newwin(1, INPUT_FLD_LEN,
							   rand_letters_win->_begy + 5, game_win->_begx + (getmaxx(game_win) - INPUT_FLD_LEN)/2);
    // Fields (only 1)
    FIELD **input_field = (FIELD **)calloc(fld_count + 1, sizeof(FIELD *));
	input_field[0] = new_field(1, INPUT_FLD_LEN, 0, 0, 0, 1);
	input_field[fld_count] = NULL;

    // Field settings
    set_field_type(input_field[0], TYPE_ALPHA, 2);		// SOMETIMES DOESN'T WORK
	field_opts_off(input_field[0], O_AUTOSKIP);

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


	/* ----------------------------------------------------------------------------------------- */
	/* Game loop */


	// Round loop
	for(int r = 0; r < rounds; r++) {
		is_valid = 0;
		
		wattron(game_win, A_BOLD);

		mvwprintw(game_win, 1, 2, "Round %d / %d", r+1, rounds);	// Print current round
		if(r + 1 == rounds) {
			// On last round
			wattron(game_win, COLOR_PAIR(3));
			mvwprintw(game_win, 2, getmaxx(game_win)/2 - 6, "FINAL ROUND!");
			wattroff(game_win, COLOR_PAIR(3));
		}
		mvwprintw(game_win, 1, getmaxx(game_win) - 11, "Points: %d", points);	// Print current points

		wattroff(game_win, A_BOLD);


		// Generate random letters and print them
		get_rand_letters(rand_letters, letters);
		strcpy(save_letters, rand_letters);		// save the random letters
		for(int i = 0; i < letters; i++) mvwprintw(rand_letters_win, 0, i * 2, "%c ", rand_letters[i]);


		form_driver(input_form, REQ_FIRST_FIELD);
		form_driver(input_form, REQ_CLR_FIELD);


		wrefresh(game_win);
		wrefresh(rand_letters_win);
		wrefresh(input_win);
		wrefresh(msg_win);


		// Read input from the user
		do {
			if(take_input) key = getch();
			take_input = 1;
			
			switch(key) {
				/* All possible actions in the field */

                case KEY_BACKSPACE: // Delete from the field (handle all backspace chars)
                case '\b':
                case 127:
					werase(msg_win);
					form_driver(input_form, REQ_VALIDATION);	// update field buffer
					input_str = field_buffer(input_field[0], 0);	// get the field string
					strrmspaces(&input_str);	// remove spaces from the string (only back and front)

					// Delete the last letter
					if(strlen(input_str) == INPUT_FLD_LEN) {
						form_driver(input_form, REQ_DEL_CHAR);
					}else {
      	            	form_driver(input_form, REQ_DEL_PREV);
					}

					// Put the deleted letter in save_letters
					if(strlen(input_str) != 0) {
						for(int i = strlen(save_letters); i >= 0; i--) {
							if(save_letters[i] == ' ') {
								save_letters[i] = input_str[strlen(input_str) - 1];
								break;
							}
						}
					}

					// Print all the random letters again
					for(int i = 0; i < letters; i++) {
						mvwprintw(rand_letters_win, 0, i * 2, "%c ", save_letters[i]);
					}
					wrefresh(rand_letters_win);


					// Center the input window
					form_driver(input_form, REQ_VALIDATION);	// update field buffer
					input_str = field_buffer(input_field[0], 0);	// get the field string
					strrmspaces(&input_str);	// remove spaces from the string (only back and front)

					mvwin(input_win, rand_letters_win->_begy + 5,
									 game_win->_begx + (getmaxx(game_win) - strlen(input_str))/2);
					mvprintw(input_win->_begy, input_win->_begx - 1, " ");
						 
                    break;

				case 10:  // Enter key (validate input)
					form_driver(input_form, REQ_VALIDATION);  // update field buffer
					input_str = field_buffer(input_field[0], 0);	// get the field string
					strrmspaces(&input_str);	// remove spaces from the string (only back and front)


					/* Check if the word is in the dictionary */
					word_points = check_word(input_str);

					switch(word_points) {
						case -1:
							is_valid = 0;
							message_log("Type something using the letters above!");
							wrefresh(msg_win);
							break;

						case 0:
							is_valid = 0;
							message_log("This word isn't in the dictionary!");
							break;

						default:
							is_valid = 1;
							points += word_points;
							// pirnt different messages depending on the points
							if(word_points >= 1 && word_points <= 3) {
								wattron(msg_win, COLOR_PAIR(1));
								message_log("Great!");
								wattroff(msg_win, COLOR_PAIR(1));

							}else if(word_points >= 4 && word_points <= 6) {
								wattron(msg_win, COLOR_PAIR(6));
								message_log("Amazing!");
								wattroff(msg_win, COLOR_PAIR(6));

							}else {
								wattron(msg_win, COLOR_PAIR(4));
								message_log("* LEGENDARY! *");
								wattroff(msg_win, COLOR_PAIR(4));
							}
							wattron(game_win, COLOR_PAIR(1));
							mvwprintw(game_win, 2, getmaxx(game_win) - 4, "+%d", word_points);
							wattroff(game_win, COLOR_PAIR(1));
							break;
					}
					break;

				case KEY_STAB:  // Skip the current round (handle all tab chars)
				case '\t':
					werase(msg_win);
					wattron(msg_win, A_ITALIC);
					message_log("* Press TAB again to confirm *");
					wattroff(msg_win, A_ITALIC);
					wrefresh(msg_win);

					key = getch();
					if(key == KEY_STAB || key == '\t') {
						// press tab again
						take_input = 1;  // we skipped the round -> take input on next iteration
						werase(msg_win);
						message_log("Round skipped >>");
						mvwprintw(game_win, 2, getmaxx(game_win) - 4, "   ");	// this clears the +points

					}else {
						// we didn't skip the round -> input is already taken -> don't take input on next iteration
						take_input = 0;
					}
					break;

				case ' ':	// Space key: end game
					werase(msg_win);
					wattron(msg_win, A_ITALIC);
					message_log("* Press SPACE again to end the game *");
					wattroff(msg_win, A_ITALIC);
					wrefresh(msg_win);

					key = getch();
					if(key == ' ') {
						// press space again

						// Exit game window
						exitForm(&input_form, &input_field, fld_count);
						delwin(input_win);
						delwin(rand_letters_win);
						delwin(game_win);

						werase(msg_win);

						return;

					}else {
						// we didn't skip the round -> input is already taken -> don't take input on next iteration
						take_input = 0;
					}
					break;

                case KEY_RESIZE:  // on window resize
                    refresh_game_win(game_win, rand_letters_win, input_win, letters, rounds, r, points, input_str);
                    break;

				default:  // write to the field
					werase(msg_win);

					// Only type available letters
					for(int i = 0; i < strlen(save_letters); i++) {
						if(save_letters[i] == key) {
							form_driver(input_form, key);	// write the letter
							save_letters[i] = ' ';		// remove the typed letter from the save
							break;
						}
					}

					// Print all the random letters again
					for(int i = 0; i < letters; i++) mvwprintw(rand_letters_win, 0, i * 2, "%c ", save_letters[i]);
					wrefresh(rand_letters_win);


					// Center the input window
					form_driver(input_form, REQ_VALIDATION);	// update field buffer
					input_str = field_buffer(input_field[0], 0);	// get the field string
					strrmspaces(&input_str);	// remove spaces from the string (only back and front)

					mvwin(input_win, rand_letters_win->_begy + 5,
									 game_win->_begx + (getmaxx(game_win) - strlen(input_str))/2);
					break;
			}
			wrefresh(input_win);
			wrefresh(msg_win);

		}while(!is_valid && key != KEY_STAB && key != '\t');	// Read input until ENTER is pressed and the word isn't valid
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

	wattron(game_win, A_ITALIC);
	mvwprintw(game_win, getmaxy(game_win) - 2, getmaxx(game_win)/2 - 13, "Press ENTER to continue...");
	wattroff(game_win, A_ITALIC);

	wrefresh(game_win);


	werase(msg_win);
	while(getch() != 10) {
		// only refresh until enter key is pressed
		refresh_game_win(game_win, rand_letters_win, input_win, letters, rounds, rounds, points, input_str);
	}


	// Exit game window
	exitForm(&input_form, &input_field, fld_count);
	delwin(input_win);
	delwin(rand_letters_win);
	delwin(game_win);
}
