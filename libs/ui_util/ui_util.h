/* UI utility functions */

#include <ncurses.h>
#include <menu.h>
#include <form.h>


/* ============================================================================================= */

#ifndef UI_UTIL_H		// Include guard
#define UI_UTIL_H

/* --------------------------------------------------------------------------------------------- */
/* UI element macros */


/*
 * The maximum message length printable in the message window.
 * Used for width of message window.
 */
#define MSG_LEN 36


/* --------------------------------------------------------------------------------------------- */
/* Global variables */


/* 
 * Global message window.
 */
extern WINDOW *msg_win;


/* --------------------------------------------------------------------------------------------- */
/* UI utility functions */


/*
 * Scrabble: Log a message to the screen.
 */
void message_log(char *message);


/*
 * Unpost a menu and free its items.
 */
void exitMenu(MENU **menu, ITEM ***items, int num_of_items);

/*
 * Unpost a form and free its fields.
 */
void exitForm(FORM **form, FIELD ***fields, int num_of_fields);


/* --------------------------------------------------------------------------------------------- */

#endif		// End of include guard
