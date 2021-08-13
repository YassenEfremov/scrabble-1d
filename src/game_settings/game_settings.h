/* Game settings functions */


/* ============================================================================================= */


/* The length of a setting field. */
#define FLD_LEN 2


/*
 * Change the number of letters and rounds.
 * If 0 is passed to either option it isn't changed.
 */
static int change_settings(int new_letters, int new_rounds);

/* Refresh the settings menu screen. (use on resize of terminal) */
static void refresh_settings_menu(int rows, int cols, WINDOW *settings_menu_win, int num_of_items);

/* Take and vaidate input using the settings form fields. */
static char *take_field_input(int opt_index, char *err_msg,
                              WINDOW *settings_menu_win, FORM *settings_form, FIELD **settings_fields, 
                              int num_of_items);


/* Scrabble: Open the game settings. */
void gameSettings(int *letters, int *rounds);