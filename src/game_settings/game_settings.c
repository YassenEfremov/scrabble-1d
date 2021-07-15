/* Definitions for functions declared in game_settings.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>
#include <ctype.h>      // atoi
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "libs/ui_util/ui_util.h"

#include "game_settings.h"


/* ============================================================================================= */
/* Private functions */


static int change_settings(int new_letters, int new_rounds) {

    config_t game_settings;

    config_init(&game_settings);

    /*
     * The options are represented by a 5 bit number. The default is 10110 (22 dec).
     * To disable semicolons in the configuration file we reset the 4th bit using XOR.
     */
    config_set_options(&game_settings, 22 ^ CONFIG_OPTION_SEMICOLON_SEPARATORS);
 
	if(config_read_file(&game_settings, "../config/game_settings.cfg") != CONFIG_TRUE) {    // TEMPORARY LOCATION
		printf("\nError: game_settings.json missing!");
		return 2;
	}

    config_setting_t *root = config_root_setting(&game_settings);
    config_setting_t *letters = config_setting_lookup(root, "letters");
    config_setting_t *rounds = config_setting_lookup(root, "rounds");

    // If 0 is passed we don't change the setting
    if(new_letters != 0) config_setting_set_int(letters, new_letters);
    if(new_rounds != 0) config_setting_set_int(rounds, new_rounds);

    config_write_file(&game_settings, "../config/game_settings.cfg");   // TEMPORARY LOCATION

    config_destroy(&game_settings);

    return 0;
}


/* --------------------------------------------------------------------------------------------- */


static void refresh_settings_menu(int rows, int cols, WINDOW *settings_menu_win, int num_of_items) {

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


/* --------------------------------------------------------------------------------------------- */


static char *take_field_input(int fld_index, char *err_msg,
                              WINDOW *settings_menu_win,
                              FORM *settings_form, FIELD **settings_fields, 
                              int rows, int cols, int num_of_items) {

    int key;
    int is_valid;

    char *field_str = field_buffer(settings_fields[fld_index], 0);   // read previous setting
    char field_save[3];     // save it to a string
    strcpy(field_save, field_str);
    
    set_field_back(settings_fields[fld_index], A_UNDERLINE);

    // Input and validation
    do {
        // Move to the correct field and posiotion
        form_driver(settings_form, REQ_FIRST_FIELD);
        for(int i = 0; i < fld_index; i++) form_driver(settings_form, REQ_NEXT_FIELD);    // move to specified field
        for(int i = 0; field_str[i+1] != ' '; i++) form_driver(settings_form, REQ_NEXT_CHAR); // move to end of word
        wrefresh(settings_menu_win);

        // Read input from the user
        do {
            key = getch();

            switch(key) {
                /* All possible actions in the field */

                case KEY_BACKSPACE:  // delete from the field
                    form_driver(settings_form, REQ_DEL_CHAR);
                    form_driver(settings_form, REQ_PREV_CHAR);
                    break;

                case 'q':  // exit the field
                    if(is_valid != E_OK) strcpy(field_str, field_save);
                    set_field_buffer(settings_fields[fld_index], 0, field_str);
                    werase(msg_win);
                    set_field_back(settings_fields[fld_index], A_NORMAL);
                    wrefresh(settings_menu_win);
                    return field_str;

                case 10:  // Enter key (validate input)
                    form_driver(settings_form, REQ_VALIDATION);  // update field buffer

                    for(int i = 0; field_str[i] != '\0'; i++) {
                        if(field_str[i] == ' ') {
                            // if every char in the field is a space -> field isn't valid
                            is_valid = E_BAD_ARGUMENT;
                        }else {
                            // if the field isn't entirely spaces -> validate the actual data in it
                            is_valid = form_driver(settings_form, REQ_VALIDATION);
                            break;
                        }
                    }
                    

                    if(is_valid != E_OK) {
                        werase(msg_win);
                        message_log(err_msg);
                        wrefresh(msg_win);
                    }else {
                        field_str = field_buffer(settings_fields[fld_index], 0);
                    }
                    break;

                case KEY_RESIZE:  // on window resize
                    refresh_settings_menu(rows, cols, settings_menu_win, num_of_items);
                    break;

                default:  // write to the field
                    form_driver(settings_form, REQ_VALIDATION);     // update field buffer
                    // in an empty field the first char is always a space
                    if(field_str[0] != ' ') form_driver(settings_form, REQ_NEXT_CHAR);
                    form_driver(settings_form, key);
                    break;
            }
            wrefresh(settings_menu_win);

        }while(key != 10);

    }while(is_valid != E_OK);

    werase(msg_win);
    set_field_back(settings_fields[fld_index], A_NORMAL);

    return field_str;
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

			case 10:  // Enter key
                werase(msg_win);
				curr_item_index = item_index(curr_item);
                
                set_menu_mark(settings_menu, "  ");
                wrefresh(settings_menu_win);

				switch(curr_item_index) {
					case 0:
                        /* Letters field */

                        str_keys = take_field_input(curr_item_index, "Invalid! Letters are from 2 to 26.",
                                                    settings_menu_win,
                                                    settings_form, settings_fields,
                                                    rows, cols, num_of_items);

                        change_settings(atoi(str_keys), 0); // Write the entered data to the settings file

						break;

					case 1:
						/* Rounds field */

                        str_keys = take_field_input(curr_item_index, "Invalid! Rounds are from 1 to 99.",
                                                    settings_menu_win,
                                                    settings_form, settings_fields,
                                                    rows, cols, num_of_items);

                        change_settings(0, atoi(str_keys)); // Write the entered data to the settings file

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
