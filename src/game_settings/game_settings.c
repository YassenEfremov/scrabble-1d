/*
 *	Game settings menu
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


#include "game_settings.h"

#include "libs/file_paths.h"
#include "libs/dict_handling/dict_handling.h"
#include "libs/ui_util/ui_util.h"

#include <glib.h>
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ============================================================================================= */
/* Private functions */


/* Update the game settings in the configuration file. */
static int change_settings(int new_letters, int new_rounds) {

	GKeyFile *game_settings = g_key_file_new();
	GKeyFileFlags conf_flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;   // set those 2 flags 
	GError *conf_error = NULL;

	gchar *config_path = g_build_filename(g_get_user_config_dir(), GAME_DIR, GAME_CONFIG_NAME, (char *)NULL);

    g_key_file_load_from_file(game_settings, config_path, conf_flags, &conf_error);

    // Write the new letters and rounds
    if(new_letters != 0) g_key_file_set_integer(game_settings, "Settings", "letters", new_letters);
    if(new_rounds != 0) g_key_file_set_integer(game_settings, "Settings", "rounds", new_rounds);

    g_key_file_save_to_file(game_settings, config_path, &conf_error);

	g_key_file_free(game_settings);
    g_free(config_path);

    return 0;
}


/* --------------------------------------------------------------------------------------------- */


static void refresh_settings_menu(WINDOW *settings_menu_win, int num_of_items) {

	// If the size of the terminal is smaller than (39, 5) everything glitches out!				// TO DO
    // WHEN THE SCREEN IS TOO SMALL SOME ELEMENTS DISAPPEAR										// TO DO

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, term_rows, term_cols);	// get the new dimentions of the main screen

	mvwin(settings_menu_win, term_rows/2 - num_of_items/2, term_cols/2 - 6);
    attron(A_UNDERLINE);
    attron(A_BOLD);
    mvprintw(settings_menu_win->_begy - 2, term_cols/2 - 4, "Settings");
    attroff(A_BOLD);
    attroff(A_UNDERLINE);

	mvwin(title_win, settings_menu_win->_begy - 4 - 4, term_cols/2 - 19);
	mvwin(msg_win, settings_menu_win->_begy + num_of_items + 3, term_cols/2 - MSG_LEN/2);


	// Refresh the necessary elements
	refresh();
	wrefresh(title_win);
    wrefresh(msg_win);
}


/* --------------------------------------------------------------------------------------------- */


static char *take_field_input(int fld_index, char *err_msg,
                              WINDOW *settings_menu_win, FORM *settings_form, FIELD **settings_fields, int num_of_items) {

    int key;
    char first_num = '0';   // usefull so you can't type 0 in as the first number
    gboolean is_valid = 0, to_refresh = 0;

    char *field_str = field_buffer(settings_fields[fld_index], 0);   // get the field string
                                                                     // field_str MAY RANDOMLY CHANGE ITS VALUE!!
                                                                     // > USE FIELD_SAVE INSTEAD!!!
    strrmspaces(&field_str);    // remove spaces (also adds '\0')
    set_field_buffer(settings_fields[fld_index], 1, field_str);     // save the old string in the additional field

    char field_save[FLD_LEN + 1];
    strcpy(field_save, field_str);  // save the field string for later


    set_field_back(settings_fields[fld_index], A_UNDERLINE);

    // Move to the correct field and posiotion
    form_driver(settings_form, REQ_FIRST_FIELD);
    for(int i = 0; i < fld_index; i++) form_driver(settings_form, REQ_NEXT_FIELD);    // move to specified field
    //for(int i = 0; field_str[i+1] != '\0'; i++) form_driver(settings_form, REQ_NEXT_CHAR); // move to end of word
    form_driver(settings_form, REQ_END_FIELD); // move to end of word
    wrefresh(settings_menu_win);


    // Read input from the user
    do {
        key = getch();
        if(to_refresh && key != KEY_RESIZE) {
            wattron(settings_menu_win, A_UNDERLINE);
            mvwprintw(settings_menu_win, fld_index, 11, "%s", field_save);
            wattroff(settings_menu_win, A_UNDERLINE);
            to_refresh = 0;
        }

        switch(key) {
            /* All possible actions in the field */

            case KEY_BACKSPACE: // Delete from the field (handle all backspace chars)
            case '\b':
            case 127:
                werase(msg_win);
                form_driver(settings_form, REQ_VALIDATION);  // update field buffer
                field_str = field_buffer(settings_fields[fld_index], 0);    // get field string
                strrmspaces(&field_str);	// remove spaces from the string (only back and front)
                /* For some reason the REQ_VALIDATION line causes the cursor to be moved 1 position back.
                   That is a problem only when the field is full e.g. it contains a 2 digit number. Therefore we move
                   the cursor 1 position forward. */
                if(atoi(field_str) > 10) form_driver(settings_form, REQ_NEXT_CHAR);

                // Delete the last letter
                if(strlen(field_str) == FLD_LEN || strcmp(field_str, "") == 0) {
                    form_driver(settings_form, REQ_NEXT_CHAR);  // without this line sometimes we delete the previous letter
                    form_driver(settings_form, REQ_DEL_CHAR);
                }else {
                    // these 2 separate lines are needed because otherwise we move to a different field
                    form_driver(settings_form, REQ_PREV_CHAR);
                    form_driver(settings_form, REQ_DEL_CHAR);
                }
                break;

            case 10:  // Enter key (validate input)
                is_valid = form_driver(settings_form, REQ_VALIDATION);  // update field buffer
                field_str = field_buffer(settings_fields[fld_index], 0);    // get field string
                strrmspaces(&field_str);	// remove spaces (also adds '\0')
                if(strcmp(field_str, "") == 0) is_valid = E_BAD_ARGUMENT;   // nothing was typed (string is empty)

                // Handle invalid input
                if(is_valid != E_OK) {
                    werase(msg_win);
                    message_log(err_msg);
                    wrefresh(msg_win);

                    field_str = field_buffer(settings_fields[fld_index], 0);    // get because of spaces when printig
                    if(atoi(field_str) < 10) field_str[1] = ' ';    // NEEDED ON SOME SYSTEMS!
                    strcpy(field_save, field_str);

                    wattron(settings_menu_win, COLOR_PAIR(5));
                    wattron(settings_menu_win, A_UNDERLINE);
                    mvwprintw(settings_menu_win, fld_index, 11, "%s", field_save);
                    wattroff(settings_menu_win, A_UNDERLINE);
                    wattroff(settings_menu_win, COLOR_PAIR(5));
                    to_refresh = 1;
                }
                break;

            case 'q':  // exit the field
                werase(msg_win);
                field_str = field_buffer(settings_fields[fld_index], 1);    // get the old field string
                set_field_buffer(settings_fields[fld_index], 0, field_str);
                set_field_back(settings_fields[fld_index], A_NORMAL);
                return field_str;

            case KEY_RESIZE:  // on window resize
                refresh_settings_menu(settings_menu_win, num_of_items);
                break;

            default:  // write to the field
                werase(msg_win);
                form_driver(settings_form, REQ_VALIDATION);     // update field buffer
                field_str = field_buffer(settings_fields[fld_index], 0);    // update field string
                // in an empty field the first char is always a space
                if(field_str[0] != ' ') {
                    form_driver(settings_form, REQ_NEXT_CHAR);
                    first_num = '0';
                }else {
                    first_num = '1';
                }
                if(key >= first_num && key <= '9') form_driver(settings_form, key);  // check if the key is valid
                break;
        }

        // Refresh everything
        wrefresh(settings_menu_win);

    }while(key != 10 || is_valid != E_OK);  // Enter key

    werase(msg_win);
    set_field_back(settings_fields[fld_index], A_NORMAL);


    return field_str;
}


/* ============================================================================================= */
/* Public functions */


void gameSettings(int *letters, int *rounds) {

    refresh();
	getmaxyx(stdscr, term_rows, term_cols);	// get the dimentions of the terminal


    // Variables

    char *field_str;

    char *items_list[] = {
		"Letters:",
		"Rounds:",
		"Back"
	};
    int num_of_items = sizeof(items_list)/sizeof(items_list[0]);
    int fld_count = 2;

	int key;
	ITEM *curr_item;
	int curr_item_index;

    // Used for writing to the field buffer
    char str_letters[3];
    char str_rounds[3];


    /* ----------------------------------------------------------------------------------------- */
    /* Settings menu */

    // Options
	ITEM **items = (ITEM **)calloc(num_of_items + 1, sizeof(ITEM *));
	for(int i = 0; i < num_of_items; i++) {
		items[i] = new_item(items_list[i], "");		// Name each option
	}
    items[num_of_items] = NULL;

    // Menu
	WINDOW *settings_menu_win = newwin(num_of_items, 14, term_rows/2 - num_of_items/2, term_cols/2 - 7);
	MENU *settings_menu = new_menu((ITEM **)items);
	set_menu_win(settings_menu, settings_menu_win);
    set_menu_sub(settings_menu, settings_menu_win);

    // Menu settings
	set_menu_mark(settings_menu, "> ");
	set_menu_fore(settings_menu, A_BOLD);
	

    /* ----------------------------------------------------------------------------------------- */
    /* Settings form */

    // Fields
    FIELD **settings_fields = (FIELD **)calloc(fld_count + 1, sizeof(FIELD *));
	settings_fields[0] = new_field(1, FLD_LEN, 0, 11, 0, 1);
    settings_fields[1] = new_field(1, FLD_LEN, 1, 11, 0, 1);
	settings_fields[fld_count] = NULL;

    // Field settings
    set_field_type(settings_fields[0], TYPE_INTEGER, 0, 2, 26);     // WORKS PARTIALLY
    set_field_type(settings_fields[1], TYPE_INTEGER, 0, 1, 99);     // WORKS PARTIALLY

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
    mvprintw(settings_menu_win->_begy - 2, term_cols/2 - 4, "Settings");
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
        werase(msg_win);

		switch(key) {
            // Arrow keys

			case KEY_DOWN:
				menu_driver(settings_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				menu_driver(settings_menu, REQ_UP_ITEM);
				break;

			case 10:  // Enter key
				curr_item_index = item_index(curr_item);
                
                set_menu_mark(settings_menu, "  ");
                wrefresh(settings_menu_win);

				switch(curr_item_index) {

					case 0:
                        /* Letters field */

                        field_str = take_field_input(curr_item_index, "Invalid! Letters are from 2 to 26.",
                                                    settings_menu_win, settings_form, settings_fields, num_of_items);

                        // Change the settings in the file (only if they have changed!)
                        if(atoi(field_str) != *letters) change_settings(atoi(field_str), 0);
						break;

					case 1:
						/* Rounds field */

                        field_str = take_field_input(curr_item_index, "Invalid! Rounds are from 1 to 99.",
                                                    settings_menu_win, settings_form, settings_fields, num_of_items);

                        // Change the settings in the file (only if they have changed!)
                        if(atoi(field_str) != *letters) change_settings(0, atoi(field_str));
						break;

					case 2:
                        // Exit settings menu
                        exitForm(&settings_form, &settings_fields, fld_count);
                        exitMenu(&settings_menu, &items, num_of_items);
                        delwin(settings_menu_win);
                        return;
				}
				break;

            case KEY_RESIZE:
				// On window resize
                refresh_settings_menu(settings_menu_win, num_of_items);
                break;
		}
        set_menu_mark(settings_menu, "> ");

        // Refresh everything
        wrefresh(settings_menu_win);
		wrefresh(msg_win);

	}while(1);
}
