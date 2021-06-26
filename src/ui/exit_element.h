#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>


// ============================================================================================= //

#ifndef EXIT_APP
#define EXIT_APP

extern WINDOW *msg_win;


extern void exitMenu(MENU **menu, ITEM ***items, int num_of_items) {

	// Free the menu
	unpost_menu(*menu);
	free_menu(*menu);

	// Free the options
	for(int i = 0; i < num_of_items + 1; i++) free_item((*items)[i]);
	free(*items);
}


extern void exitForm(FORM **form, FIELD ***fields, int num_of_fields) {

	// Free the form
	unpost_form(*form);
	free_form(*form);

	// Free the fields
	for(int i = 0; i < num_of_fields + 1; i++) free_field((*fields)[i]);
	free(*fields);
}

#endif
