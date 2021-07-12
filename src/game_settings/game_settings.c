/* Definitions for functions declared in game_settings.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>      // atoi
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "libs/jWrite.h"
#include "libs/ui_util/ui_util.h"

#include "game_settings.h"


/* ============================================================================================= */
/* Private functions */


int change_letters(int new_letters, int rounds){
		
	FILE* settings_json = fopen("../config/game_settings.json", "w");
	
	if(!settings_json){
		message_log("Error: settings.json missing!");
		return 2;
	}
	
    int json_size = 39;  // default size (when bot options are 2 digit numbers)
    if(new_letters < 10) json_size -= 1;    // decrease the size by one
    if(rounds < 10) json_size -= 1;    // decrease the size by one

	char buffer[json_size];
	jwOpen(buffer, json_size, JW_OBJECT, JW_PRETTY);
	jwObj_int("letters", new_letters);
	jwObj_int("rounds", rounds);
	int err_code = jwClose();

	fwrite(buffer, json_size, 1, settings_json);
	
	fclose(settings_json);

    return err_code;
}


/* --------------------------------------------------------------------------------------------- */


int change_rounds(int new_rounds, int letters){
		
	FILE* settings_json = fopen("../config/game_settings.json", "w");
	
	if(!settings_json){
		message_log("Error: settings.json missing!");
		return 2;
	}
	
    int json_size = 39;  // default size (when bot options are 2 digit numbers)
    if(new_rounds < 10) json_size -= 1;    // decrease the size by one
    if(letters < 10) json_size -= 1;    // decrease the size by one

	char buffer[json_size];
	jwOpen(buffer, json_size, JW_OBJECT, JW_PRETTY);
	jwObj_int("letters", letters);
	jwObj_int("rounds", new_rounds);
	int err_code = jwClose();

	fwrite(buffer, json_size, 1, settings_json);
	
	fclose(settings_json);

    return err_code;
}


/* --------------------------------------------------------------------------------------------- */


/*
 * Refresh the settings menu screen. (use on resize of terminal)
 */
void refresh_settings_menu(int rows, int cols, WINDOW *settings_menu_win, int num_of_items) {

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, rows, cols);	// get the new dimentions of the main screen

	// If the size of the terminal is smaller than (39, 5) everything glitches out!				// TO DO
	mvwin(settings_menu_win, rows/2 - num_of_items/2, cols/2 - 6);
    attron(A_UNDERLINE);
    attron(A_BOLD);
    mvprintw(settings_menu_win->_begy - 2, cols/2 - 4, "Settings");
    attroff(A_BOLD);
    attroff(A_UNDERLINE);

	mvwin(title_win, settings_menu_win->_begy - 4 - 4, cols/2 - 19);
	mvwin(msg_win, settings_menu_win->_begy + num_of_items + 3, cols/2 - MSG_LEN/2);

	// Refresh the necessay elements
	refresh();
	wrefresh(title_win);
}


/* ============================================================================================= */
/* Public functions */


void gameSettings(int *letters, int *rounds) {

    refresh();

    // Used for writing to the field buffer
    char str_letters[3];
    char str_rounds[3];

    // Predefined data used by ncurses

    int rows, cols;
	getmaxyx(stdscr, rows, cols);	// get the dimentions of the main screen

    char *items_list[] = {
		"Letters:",
		"Rounds:",
		"Back"
	};
    int num_of_items = sizeof(items_list)/sizeof(items_list[0]);
    int num_of_fields = 2;

	ITEM *curr_item;
	int key;
    //char str_value[3];
    char *str_keys;
	int curr_item_index;
    int is_valid;


    /* ----------------------------------------------------------------------------------------- */
    /* Settings menu */

    // Options
	ITEM **items = (ITEM **)calloc(num_of_items + 1, sizeof(ITEM *));
	for(int i = 0; i < num_of_items; i++) {
		items[i] = new_item(items_list[i], "");		// Name each option
	}
    items[num_of_items] = NULL;

    // Menu
	WINDOW *settings_menu_win = newwin(num_of_items, 14, rows/2 - num_of_items/2, cols/2 - 7);
	MENU *settings_menu = new_menu((ITEM **)items);
	
	set_menu_win(settings_menu, settings_menu_win);
    set_menu_sub(settings_menu, settings_menu_win);

    // Menu settings
	set_menu_mark(settings_menu, "> ");
	set_menu_fore(settings_menu, A_BOLD);
	

    /* ----------------------------------------------------------------------------------------- */
    /* Settings form */

    // Fields
    FIELD **settings_fields = (FIELD **)calloc(num_of_fields + 1, sizeof(FIELD *));

	settings_fields[0] = new_field(1, 2, 0, 11, 0, 0);
    settings_fields[1] = new_field(1, 2, 1, 11, 0, 0);
	settings_fields[num_of_fields] = NULL;

    // Field settings
    set_field_type(settings_fields[0], TYPE_INTEGER, 0, 2, 26);
    set_field_type(settings_fields[1], TYPE_INTEGER, 0, 1, 99);

    field_opts_off(settings_fields[0], O_AUTOSKIP);
    field_opts_off(settings_fields[1], O_AUTOSKIP);

    // Form
	FORM *settings_form = new_form(settings_fields);

    // Form settings
    set_form_win(settings_form, settings_menu_win);
    set_form_sub(settings_form, settings_menu_win);
    

    /* ----------------------------------------------------------------------------------------- */


    // Print menu title
    attron(A_UNDERLINE);
    attron(A_BOLD);
    mvprintw(settings_menu_win->_begy - 2, cols/2 - 4, "Settings");
    attroff(A_BOLD);
    attroff(A_UNDERLINE);

    // First post the form, then the manu
    post_form(settings_form);
    post_menu(settings_menu);

    // Set the settings values (read form file)
    sprintf(str_letters, "%d", *letters);
    sprintf(str_rounds, "%d", *rounds);

    set_field_buffer(settings_fields[0], 0, str_letters);
    set_field_buffer(settings_fields[1], 0, str_rounds);

    // Refresh everything
    wrefresh(settings_menu_win);
    wrefresh(msg_win);

	
    /* ----------------------------------------------------------------------------------------- */
	/* Navigation */

	do {
        key = getch();
		curr_item = current_item(settings_menu);

		switch(key) {
            // Arrow keys

			case KEY_DOWN:
                werase(msg_win);
				menu_driver(settings_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
                werase(msg_win);
				menu_driver(settings_menu, REQ_UP_ITEM);
				break;

			case 10: 
                // Enter key
                werase(msg_win);
				curr_item_index = item_index(curr_item);
                
                set_menu_mark(settings_menu, "  ");
                wrefresh(settings_menu_win);

				switch(curr_item_index) {
					case 0:
                        /* --------------------------------------------------------------------- */
                        /* Letters field */

                        str_keys = field_buffer(settings_fields[0], 0);
                        set_field_back(settings_fields[0], A_UNDERLINE);

                        // Input and validation
                        do {
                            // Move to the end of word in field
                            form_driver(settings_form, REQ_FIRST_FIELD);
                            if(atoi(str_keys) < 10) {
                                form_driver(settings_form, REQ_BEG_LINE);
                            }else {
                                form_driver(settings_form, REQ_END_LINE);
                            }
                            wrefresh(settings_menu_win);

                            // Read input from the user
                            do {
                                key = getch();

                                if(key == 'q') {    // exit the field
                                    set_field_buffer(settings_fields[0], 0, str_keys);
                                    break;
                                }else if(key == KEY_BACKSPACE) {    // delete from the field
                                    form_driver(settings_form, REQ_DEL_CHAR);
                                    form_driver(settings_form, REQ_PREV_CHAR);
                                }else if(key == KEY_RESIZE) {     // write to the field
                                    refresh_settings_menu(rows, cols, settings_menu_win, num_of_items);
                                }else {    
                                    form_driver(settings_form, key);
                                }
                                wrefresh(settings_menu_win);
                            }while(key != 10);

                            // Check if it's valid
                            is_valid = form_driver(settings_form, REQ_VALIDATION);
                            if(is_valid != E_OK) {
                                message_log("Invalid! Letters are from 2 to 26.");
                                wrefresh(msg_win);
                            }else {
                                str_keys = field_buffer(settings_fields[0], 0);
                            }

                        }while(is_valid != E_OK);

                        werase(msg_win);
                        
                        // Move the entered data to the field buffer
                        str_keys = field_buffer(settings_fields[0], 0);
                        change_letters(atoi(str_keys), *rounds);     // Write it to the settings

                        set_field_back(settings_fields[0], A_NORMAL);
						break;

					case 1:
                        /* --------------------------------------------------------------------- */
						/* Rounds field */

                        str_keys = field_buffer(settings_fields[1], 0);
                        set_field_back(settings_fields[1], A_UNDERLINE); 

                        // Input validation
                        do {
                            // Move to the end of word in field
                            form_driver(settings_form, REQ_LAST_FIELD);
                            if(atoi(str_keys) < 10) {
                                form_driver(settings_form, REQ_BEG_LINE);
                            }else {
                                form_driver(settings_form, REQ_END_LINE);
                            }
                            wrefresh(settings_menu_win);

                            // Read input from the user
                            do {
                                key = getch();

                                if(key == 'q') {    // exit the field
                                    set_field_buffer(settings_fields[1], 0, str_keys);
                                    break;
                                }else if(key == KEY_BACKSPACE) {    // delete from the field
                                    form_driver(settings_form, REQ_DEL_CHAR);
                                    form_driver(settings_form, REQ_PREV_CHAR);
                                }else if(key == KEY_RESIZE) {     // write to the field
                                    refresh_settings_menu(rows, cols, settings_menu_win, num_of_items);
                                }else {
                                    form_driver(settings_form, key);
                                }
                                wrefresh(settings_menu_win);
                            }while(key != 10);

                            // Check if it's valid
                            is_valid = form_driver(settings_form, REQ_VALIDATION);
                            if(is_valid != E_OK) {
                                message_log("Invalid! Rounds are from 1 to 99.");
                                wrefresh(msg_win);
                            }else {
                                str_keys = field_buffer(settings_fields[1], 0);
                            }

                        }while(is_valid != E_OK);                
                        
                        wrefresh(msg_win);

                        str_keys = field_buffer(settings_fields[1], 0);
                        change_rounds(atoi(str_keys), *letters);     // Write it to the settings

                        set_field_back(settings_fields[1], A_NORMAL);
						break;

					case 2:
                        // Exit settings menu
                        exitForm(&settings_form, &settings_fields, num_of_fields);
                        exitMenu(&settings_menu, &items, num_of_items);
                        wrefresh(settings_menu_win);
                        delwin(settings_menu_win);
                        return;
				}
				break;

            case KEY_RESIZE:
				// On window resize
                refresh_settings_menu(rows, cols, settings_menu_win, num_of_items);
                break;
		}
        set_menu_mark(settings_menu, "> ");

        // Refresh everything
        wrefresh(settings_menu_win);
		wrefresh(msg_win);

	}while(1);
}
