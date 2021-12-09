/* UI utility functions */

#ifndef UI_UTIL_H		// Include guard
#define UI_UTIL_H

/* ============================================================================================= */

#include <menu.h>
#include <form.h>

/* --------------------------------------------------------------------------------------------- */
/* UI element macros */


/* The maximum message length printable in the message window. */
#define MSG_LEN 40

/* The maximum length of the data in a field. */
#define INPUT_FLD_LEN 30


/* --------------------------------------------------------------------------------------------- */
/* Global variables */


/* Global teriminal rows and columns */
extern int term_rows, term_cols;


/* Global message window. */
extern WINDOW *msg_win;
/* Global title window. */
extern WINDOW *title_win;


/* --------------------------------------------------------------------------------------------- */
/* UI utility functions */


/* Scrabble: Log a message to the screen. */
void message_log(char *message);

/* Unpost a menu and free its items. */
void exitMenu(MENU **menu, ITEM ***items, int num_of_items);
/* Unpost a form and free its fields. */
void exitForm(FORM **form, FIELD ***fields, int fld_count);


/* --------------------------------------------------------------------------------------------- */

#endif		// End of include guard
