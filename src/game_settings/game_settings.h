#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <menu.h>

#include "../ui/exit_app.h"


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

// --------------------------------------------------------------------------------------------- // 

extern void write_settings(int line) {

    // This function copies the settings into a new file, changing only a specific line, and then deletes the old file
    
    int BUFFER_SIZE = 4;
    FILE * file_old;
    FILE * file_new;

    char buffer[BUFFER_SIZE];
    char newline[BUFFER_SIZE];
    int count;

    // Remove extra new line character from stdin

    //max letters must be 26
    //max rounds must be 99

    char c;
    int max_value, flag;

    do {
        // tell the program what you are going to enter
        if(line == 1) {
            printf("change the number of letters to:");
            max_value = 26;
        }else {
            printf("change the number of rounds to:");
            max_value = 99;
        }

        flag = 1;    // flag reset

        //getc(stdin);
        fgets(newline, BUFFER_SIZE, stdin);
        if(newline[1] != '\n') {
            while((c = getchar()) != '\n' && c != EOF);    // takes all the leftover chars
        }

        // check if each char of the string is an integer
        for(int i = 0; i < BUFFER_SIZE-1; i++) {
            if(!isdigit(newline[i])) {
                flag = 0;
            }
            if(newline[i+1] == '\0' || newline[i+1] == '\n') break;
        }

    }while(!flag || atoi(newline) > max_value || atoi(newline) < 1);

    
    ///////// To Add another string variable
    ///////// Amount of letter: newline


    //open our needed files
    file_old  = fopen("./game_settings/num_of_let_and_rounds.txt", "r");
    file_new = fopen("./game_settings/replace.tmp", "w"); 

    //check if files are unable to open
    if (file_old == NULL || file_new == NULL) {
        printf("\nUnable to open file.\n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        // exit(EXIT_SUCCESS);
        return;
    }


    /*
     * Read line from source file and write to destination 
     * file after replacing given line.
     */
    count = 0;
    while ((fgets(buffer, BUFFER_SIZE, file_old)) != NULL)
    {
        count++;


        if (count == line)
            fputs(newline, file_new);
        else
            fputs(buffer, file_new);
    }


    fclose(file_old);
    fclose(file_new);


    //Delete original source file
    remove("./game_settings/num_of_let_and_rounds.txt");

    // Rename temporary file as original file 
    rename("./game_settings/replace.tmp", "./game_settings/num_of_let_and_rounds.txt");

    printf("\nSuccessfully updated!");
    return;
}


// --------------------------------------------------------------------------------------------- //


extern void gameSettings() {
    refresh();

    // Predefined data used by ncurses

    int rows, cols;
	getmaxyx(stdscr, rows, cols);	// get the dimentions of the main screen

    char *options_list[] = {
		"Letters:",
		"Rounds:",
		"Back"
	};
    int num_of_options = sizeof(options_list)/sizeof(options_list[0]);

	ITEM *curr_item;
	int key;
	int curr_item_index;



    // Settings menu

    // Options
	ITEM **options = (ITEM **)calloc(num_of_options + 1, sizeof(ITEM *));
	for(int i = 0; i < num_of_options; i++) {
		options[i] = new_item(options_list[i], "");		// Name each option
	}
    options[num_of_options] = NULL;

    // Menu
	WINDOW *settings_menu_win = derwin(stdscr, num_of_options, 10, rows/3 + 3, cols/2 - 6);
    keypad(settings_menu_win, TRUE);
	MENU *settings_menu = new_menu((ITEM **)options);
	
    // Menu settings
	set_menu_sub(settings_menu, settings_menu_win);
	set_menu_mark(settings_menu, "> ");
	set_menu_fore(settings_menu, A_BOLD);
	
	post_menu(settings_menu);

    wrefresh(settings_menu_win);



	// Navigation
	do {
		key = getch();
		werase(msg_win);
		curr_item = current_item(settings_menu);

		switch(key) {
			case KEY_DOWN:
				menu_driver(settings_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
				menu_driver(settings_menu, REQ_UP_ITEM);
				break;

			case 10:	// Enter key
				curr_item_index = item_index(curr_item);

				switch(curr_item_index) {
					case 0:
						//mvwaddstr(msg_win, 0, msg_len/2 - strlen(soon_msg)/2, soon_msg);
                        // change number of letters
						break;

					case 1:
						//mvprintw(rows/2 + 6, cols/2 - 8, "Coming soon^(tm)!");
						// change number of rounds
						break;

					case 2:
                        exitMenu(&settings_menu, &options, num_of_options);
                        delwin(settings_menu_win);
                        return;
				}
				break;
		}
		wrefresh(msg_win);

	}while(1);





   /* 
    int option;

    do {
        printf("     (1)    Change number of letters\n");
        printf("     (2)    Change number of rounds\n");
        printf("     (3)    < Back\n");
        printf("> ");
        scanf("%d", &option);

        if(option < 1 || option > 3){
            system("clear");
            printf("Invalid, try again ! \n\n");
        }

    }while(option < 1 || option > 3);
    
    if (option == 3){

        return;
    }

    write_settings(option);
    */
}
