#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "../../libs/jRead.h"
#include "../../libs/jWrite.h"

#include "../ui/exit_element.h"
#include "../ui/message_box.h"


// ============================================================================================= //


int change_letters(int new_letters, int rounds){
		
	FILE* settings_json = fopen("../config/game_settings.json", "w");
	
	if(!settings_json){
		printf("\nError: settings.json missing! ");
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


// --------------------------------------------------------------------------------------------- //


int change_rounds(int new_rounds, int letters){
		
	FILE* settings_json = fopen("../config/game_settings.json", "w");
	
	if(!settings_json){
		printf("\nError: settings.json missing! ");
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


// ============================================================================================= //


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


    // ----------------------------------------------------------------------------------------- //
    // Settings menu

    // Options
	ITEM **items = (ITEM **)calloc(num_of_items + 1, sizeof(ITEM *));
	for(int i = 0; i < num_of_items; i++) {
		items[i] = new_item(items_list[i], "");		// Name each option
	}
    items[num_of_items] = NULL;

    // Menu
	WINDOW *settings_menu_win = derwin(stdscr, num_of_items, 14, rows/3 + 3, cols/2 - 7);
    keypad(settings_menu_win, TRUE);
    //wbkgd(settings_menu_win, COLOR_PAIR(99));
	MENU *settings_menu = new_menu((ITEM **)items);
	
    // Menu settings
	set_menu_sub(settings_menu, settings_menu_win);
	set_menu_mark(settings_menu, "> ");
	set_menu_fore(settings_menu, A_BOLD);
	

    // ----------------------------------------------------------------------------------------- //
    // Settings form

    // Fields
    FIELD **settings_fields = (FIELD **)calloc(num_of_fields + 1, sizeof(FIELD *));

	settings_fields[0] = new_field(1, 2, 0, 11, 0, 0);
    settings_fields[1] = new_field(1, 2, 1, 11, 0, 0);
	settings_fields[num_of_fields] = NULL;

    // Field settings
    set_field_type(settings_fields[0], TYPE_INTEGER, 0, 2, 26);          // not sure about these
    set_field_type(settings_fields[1], TYPE_INTEGER, 0, 1, 99);

    field_opts_off(settings_fields[0], O_AUTOSKIP);
    field_opts_off(settings_fields[1], O_AUTOSKIP);

    // Form
	FORM *settings_form = new_form(settings_fields);
    set_form_sub(settings_form, settings_menu_win);
    


    // First post the form, then the manu
    post_form(settings_form);
    post_menu(settings_menu);

    wrefresh(settings_menu_win);


    // Set the settings values (read form file)
    sprintf(str_letters, "%d", *letters);
    sprintf(str_rounds, "%d", *rounds);

    set_field_buffer(settings_fields[0], 0, str_letters);
    set_field_buffer(settings_fields[1], 0, str_rounds);

    wrefresh(settings_menu_win);


	
    // ----------------------------------------------------------------------------------------- //
	// Navigation

	do {
        key = wgetch(settings_menu_win);
		werase(msg_win);
		curr_item = current_item(settings_menu);

		switch(key) {
            // Navigation with arrow keys

			case KEY_DOWN:
				menu_driver(settings_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				menu_driver(settings_menu, REQ_UP_ITEM);
				break;

			case 10: 
                // Enter key

				curr_item_index = item_index(curr_item);
                
                set_menu_mark(settings_menu, "  ");

				switch(curr_item_index) {
					case 0:
                        // --------------------------------------------------------------------- //
                        // Letters field

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

                            // Read input from the user
                            do {
                                key = wgetch(settings_menu_win);

                                if(key == 'q') {    // exit the field
                                    set_field_buffer(settings_fields[0], 0, str_keys);
                                    break;
                                }else if(key == KEY_BACKSPACE) {    // delete from the field
                                    form_driver(settings_form, REQ_DEL_CHAR);
                                    form_driver(settings_form, REQ_PREV_CHAR);
                                }else {     // write to the field
                                    form_driver(settings_form, key);
                                }
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
                        // --------------------------------------------------------------------- //
						// Rounds field

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

                            // Read input from the user
                            do {
                                key = wgetch(settings_menu_win);

                                if(key == 'q') {    // exit the field
                                    set_field_buffer(settings_fields[1], 0, str_keys);
                                    break;
                                }else if(key == KEY_BACKSPACE) {    // delete from the field
                                    form_driver(settings_form, REQ_DEL_CHAR);
                                    form_driver(settings_form, REQ_PREV_CHAR);
                                }else {     // write to the field
                                    form_driver(settings_form, key);
                                }
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
                        delwin(settings_menu_win);
                        return;
				}
				break;
		}

        set_menu_mark(settings_menu, "> ");
		wrefresh(msg_win);

	}while(1);
}

// --------------------------------------------------------------------------------------------- //
