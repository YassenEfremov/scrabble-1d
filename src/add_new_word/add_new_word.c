/* Definitions for functions declared in add_new_word.h */

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <ncurses.h>
#include <menu.h>
#include <form.h>

#include "libs/file_paths.h"
#include "libs/trie.h"
#include "libs/dict_handling/dict_handling.h"
#include "libs/ui_util/ui_util.h"

#include "add_new_word.h"


/* ============================================================================================= */
/* Private functions */


static void refresh_new_word_win(WINDOW *new_word_win, WINDOW *input_win) {

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, term_rows, term_cols);	// get the new dimentions of the main screen

	//werase(new_word_win);
	mvwin(new_word_win, term_rows/2 - 3, term_cols/2 - 21);
    wattron(new_word_win, A_UNDERLINE);
    wattron(new_word_win, A_BOLD);
    mvwprintw(new_word_win, 1, getmaxx(new_word_win)/2 - 7, "Enter new word:");
    wattroff(new_word_win, A_BOLD);
    wattroff(new_word_win, A_UNDERLINE);
	box(new_word_win, 0, 0);


    mvwin(input_win, new_word_win->_begy + 3, new_word_win->_begx + 6);

    mvwin(title_win, new_word_win->_begy - 6, term_cols/2 - 19);
    mvwin(msg_win, new_word_win->_begy + getmaxy(new_word_win) + 2, term_cols/2 - MSG_LEN/2);


    // WHEN THE SCREEN IS TOO SMALL SOME ELEMENTS DISAPPEAR										// TO DO

	// Refresh the necessary elements
	refresh();
	wrefresh(new_word_win);
    wrefresh(title_win);
}


/* ============================================================================================= */
/* Public functions */


int addNewWord(void) {

    refresh();
	getmaxyx(stdscr, term_rows, term_cols);	// get the dimentions of the main screeno


    // Variables

	char *input_str;
    int fld_count = 1;
	gboolean is_valid, dict_err;

    struct node_t *trie_root;
    gchar *dict_file_path;
    FILE *dict;

	int key;


	/* ----------------------------------------------------------------------------------------- */
	/* New word window */

    WINDOW *new_word_win = newwin(8, 42, term_rows/2 - 3, term_cols/2 - 21);
    box(new_word_win, 0, 0);


	/* ----------------------------------------------------------------------------------------- */
	/* Input form */

    WINDOW *input_win = newwin(1, INPUT_FLD_LEN, new_word_win->_begy + 3, new_word_win->_begx + 6);
    // Fields (only 1)
    FIELD **input_field = (FIELD **)calloc(fld_count + 1, sizeof(FIELD *));
	input_field[0] = new_field(1, INPUT_FLD_LEN, 0, 0, 0, 1);
	input_field[fld_count] = NULL;

    // Field settings
    set_field_type(input_field[0], TYPE_ALPHA, 2);		// SOMETIMES DOESN'T WORK
	field_opts_off(input_field[0], O_AUTOSKIP);
	set_field_just(input_field[0], JUSTIFY_CENTER);		// DOESN'T WORK

    // Form
	FORM *input_form = new_form(input_field);

    // Form settings
    set_form_win(input_form, input_win);
    set_form_sub(input_form, input_win);


	/* ----------------------------------------------------------------------------------------- */


    // Print title and hint
    wattron(new_word_win, A_UNDERLINE);
    wattron(new_word_win, A_BOLD);
    mvwprintw(new_word_win, 1, getmaxx(new_word_win)/2 - 7, "Enter new word:");
    wattroff(new_word_win, A_BOLD);
    wattroff(new_word_win, A_UNDERLINE);
    wattron(new_word_win, A_ITALIC);
    mvwprintw(new_word_win, 6, getmaxx(new_word_win)/2 - 11, "(Press SPACE to cancel)");
    wattroff(new_word_win, A_ITALIC);

    // Post the form
	post_form(input_form);

    werase(msg_win);
    wrefresh(msg_win);

    // Reposition message window
    mvwin(msg_win, new_word_win->_begy + getmaxy(new_word_win) + 2, term_cols/2 - MSG_LEN/2);


    /* ----------------------------------------------------------------------------------------- */
    /* Form input */


    form_driver(input_form, REQ_FIRST_FIELD);
    set_field_back(input_field[0], A_UNDERLINE);

    wrefresh(new_word_win);
    wrefresh(input_win);


    // Read input from the user
    do {
        key = getch();
        
        switch(key) {
            /* All possible actions in the field */

            case KEY_BACKSPACE: // Delete from the field (handle all backspace chars)
            case '\b':
            case 127:
                werase(msg_win);
                form_driver(input_form, REQ_VALIDATION);	// update field buffer
                input_str = field_buffer(input_field[0], 0);	// get the field string

                if(strchr(input_str, ' ') == NULL) {
                    form_driver(input_form, REQ_DEL_CHAR);
                    break;
                }
                form_driver(input_form, REQ_DEL_PREV);
                break;

            case 10:  // Enter key (validate input)
                form_driver(input_form, REQ_VALIDATION);  // update field buffer
                input_str = field_buffer(input_field[0], 0);	// get the field string
                strrmspaces(&input_str);	// remove spaces from the string (only back and front)


                // Check if the word is already in the dictionary
                is_valid = checkTrie(input_str);

                switch(is_valid) {
                    case -1:
                        // Word is shorter than 2 letters
                        is_valid = -1;
                        message_log("Word is too short!");
                        break;

                    case 1:
                        // Word is already in dictionary
                        is_valid = 0;
                        message_log("Word already in dictionary!");
                        break;

                    case 0:
                        // Word is valid -> add it to the json trie file
                        is_valid = 1;


                        // Open the dictionary for appending
                        dict_file_path =  g_build_filename(g_get_user_data_dir(), GAME_DATA_DIR, DICT_NAME, (char *)NULL);
                        dict = g_fopen(dict_file_path, "a+");

                        // Append the word to the dictionary + a newline
                        fprintf(dict, "%s\n", input_str);

                        fclose(dict);
                        g_free(dict_file_path);


                        trie_root = dictToTrie();   // generate trie structure from dictionary
                        if(trie_root == NULL) {
                            trie_delete(trie_root);
                            g_free(trie_root);
                            break;
                        }

                        dict_err = trieToJson(trie_root);   // write the generated trie to the json file
                        if(dict_err != 0) {
                            trie_delete(trie_root);
                            g_free(trie_root);
                            break;
                        }


                        // Free all allocated memory
                        trie_delete(trie_root);
                        g_free(trie_root);

                        message_log("Successfully added!");
                        break;
                    /*
                    case 2:
                        // Error processing dictionary files and structures
                        is_valid = 2;
                        message_log("Trie json file missing: Generated new.");
                        break;
                    */
                }
                break;

            case ' ':  // Space key: cancel
                werase(msg_win);
                break;

            case KEY_RESIZE:  // on window resize
                refresh_new_word_win(new_word_win, input_win);
                break;

            default:  // write to the field
                werase(msg_win);	
                // Move the input window
                //> resize the field
                //> move the field
                if(key >= 'a' && key <= 'z') form_driver(input_form, key);     // this check is sometimes needed
                break;
        }
        wrefresh(input_win);
        wrefresh(msg_win);

    }while(is_valid != 1 && key != ' ');	// Read input until ENTER is pressed and the word isn't valid


    set_field_back(input_field[0], A_NORMAL);

    exitForm(&input_form, &input_field, fld_count);
    delwin(input_win);
    delwin(new_word_win);
}