#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "../ui/exit_element.h"
#include "../ui/message_box.h"


// ============================================================================================= //

#include "../../libs/jRead.h"
#include "../../libs/jWrite.h"


// THESE WILL BE REPLACED WITH JSON! (possibly)
// v v v v v v v v v v v v v v v v v v v v v v

/*
void rounds_from_file(int* rounds){ //  function to read the amount of rounds from file

    FILE* fp;
    fp = fopen("./game_settings/num_of_let_and_rounds.txt", "r");

    char buffer[25];
    char ch;
    int i =0;

    while(1){ // READ EVERYTHING FROM FILE AND SAVE IT IN BUFFER
        ch = fgetc(fp);
        if(feof(fp)){
            break;
        }
        buffer[i] = ch;
        i++;
    }
    buffer[i] = '\0';

    char *pr = ",";
    char *token;
    token = strtok(buffer, pr);

    char* amount_of_rounds;
    int flag = 0;

    while(token != NULL){
        flag++;
        token = strtok(NULL, pr);
        if(flag == 1){
            amount_of_rounds = token;
        }
    }
    
    *rounds = atoi(amount_of_rounds);

    fclose(fp);
}
*/
/*
void letters_from_file(int* letters){ // function to read the amount of letters from file

    char buffer[25];
    char ch;
    int i = 0;
    
    FILE* fp;
    fp = fopen("./game_settings/num_of_let_and_rounds.txt", "r");

    while((ch = fgetc(fp)) != ','){
        buffer[i] = ch;
        i++;
    }

    *letters = atoi(buffer);
    
    fclose(fp);
}
*/

int change_letters(int new_letters, int rounds){
		
	FILE* settings_json = fopen("../config/settings.json", "w");
	
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


int change_rounds(int new_rounds, int letters){
		
	FILE* settings_json = fopen("../config/settings.json", "w");
	
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


extern void gameSettings() {
    
    cbreak();
    refresh();

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
	int curr_item_index;


    // --------------------------------------------------------------------------------------------- //
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
	

    // --------------------------------------------------------------------------------------------- //
    // Settings form

    // Fields
    FIELD **settings_fields = (FIELD **)calloc(num_of_fields + 1, sizeof(FIELD *));

	settings_fields[0] = new_field(1, 2, 0, 11, 0, 0);
    settings_fields[1] = new_field(1, 2, 1, 11, 0, 0);
	settings_fields[num_of_fields] = NULL;

    // Field settings
    set_field_type(settings_fields[0], TYPE_NUMERIC, 0, 3, 26);          // not sure about these
    set_field_type(settings_fields[1], TYPE_NUMERIC, 0, 1, 99);          // -.-

    for(int i = 0; i < num_of_fields; i++) {
	    set_field_back(settings_fields[i], A_UNDERLINE);
	    field_opts_off(settings_fields[i], O_AUTOSKIP);
    }

    // Form
	FORM *settings_form = new_form(settings_fields);
    set_form_sub(settings_form, settings_menu_win);


    // First post the form, then the manu
    post_form(settings_form);
    post_menu(settings_menu);

	wrefresh(settings_menu_win);


    // --------------------------------------------------------------------------------------------- //
	// Navigation

	do {
		key = getch();
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
                        // Enter letters field
                        curs_set(1);
                        form_driver(settings_form, REQ_FIRST_FIELD);    // The first field is the first one
                        form_driver(settings_form, REQ_END_FIELD);

                        do {
                            key = wgetch(settings_menu_win);

                            if(key == KEY_BACKSPACE) {
                                form_driver(settings_form, REQ_DEL_CHAR);
                                form_driver(settings_form, REQ_PREV_CHAR);
                            }else {
                                form_driver(settings_form, key);
                            }
                        }while(key != 10);

                        curs_set(0);
						break;

					case 1:
						// Enter rounds field
                        curs_set(1);
                        form_driver(settings_form, REQ_LAST_FIELD);     // The last field is the second one
                        form_driver(settings_form, REQ_END_FIELD);

                        do {
                            key = wgetch(settings_menu_win);
                            //form_driver(settings_form, REQ_LAST_FIELD);

                            if(key == KEY_BACKSPACE) {
                                form_driver(settings_form, REQ_DEL_CHAR);
                                form_driver(settings_form, REQ_PREV_CHAR);
                            }else {
                                form_driver(settings_form, key);
                            }
                        }while(key != 10);

                        curs_set(0);
						break;

					case 2:
                        // Exit settings menu
                        //> write the entered settings to the json file             // TO DO
                        exitForm(&settings_form, &settings_fields, num_of_fields);
                        exitMenu(&settings_menu, &items, num_of_items);
                        delwin(settings_menu_win);
                        raw();
                        return;
				}
				break;
		}
        set_menu_mark(settings_menu, "> ");
		wrefresh(msg_win);

	}while(1);
}

// --------------------------------------------------------------------------------------------- //
