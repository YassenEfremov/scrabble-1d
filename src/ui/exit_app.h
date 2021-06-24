#include <stdio.h>
#include <ncurses.h>
#include <menu.h>


// ============================================================================================= //

#ifndef EXIT_APP
#define EXIT_APP

extern WINDOW *msg_win;


extern void exitMenu(MENU **menu, ITEM ***options, int num_of_options) {

	// Free the menu
	unpost_menu(*menu);
	free_menu(*menu);

	// Free the options
	for(int i = 0; i < num_of_options; i++) free_item((*options)[i]);
	free(*options);
}

#endif
