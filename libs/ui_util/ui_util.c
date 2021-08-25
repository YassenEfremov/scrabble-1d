/* Definitions for functions declared in ui_util.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "ui_util.h"


/* ============================================================================================= */


int term_rows, term_cols;

WINDOW *msg_win = NULL;
WINDOW *title_win = NULL;


/* --------------------------------------------------------------------------------------------- */


void message_log(char *message) {

	mvwaddstr(msg_win, 0, MSG_LEN/2 - strlen(message)/2, message);
}


/* --------------------------------------------------------------------------------------------- */

/*
int get_field_str(FORM *form, FIELD **fields, char *str) {
	int is_valid = form_driver(form, REQ_VALIDATION);  // update field buffer
	str = field_buffer(fields[fld_index], 0);    // get field string
	strrmspaces(&str);	// remove spaces (also adds '\0')
	return is_valid;	
}
*/

/* --------------------------------------------------------------------------------------------- */


void exitMenu(MENU **menu, ITEM ***items, int num_of_items) {

	// Free the menu
	unpost_menu(*menu);
	free_menu(*menu);

	// Free the options
	for(int i = 0; i < num_of_items + 1; i++) free_item((*items)[i]);
	g_free(*items);
}


/* --------------------------------------------------------------------------------------------- */


void exitForm(FORM **form, FIELD ***fields, int fld_count) {

	// Free the form
	unpost_form(*form);
	free_form(*form);

	// Free the fields
	for(int i = 0; i < fld_count + 1; i++) free_field((*fields)[i]);
	g_free(*fields);
}
