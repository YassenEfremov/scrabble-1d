#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libconfig.h>
#include <ctype.h>      // atoi
#include "game_settings.h"


/* ============================================================================================= */
/* Private functions */


static int change_settings(int new_letters, int new_rounds) {

    config_t game_settings;

    config_init(&game_settings);

    /*
     * The options are represented by a 5 bit number. The default is 10110 (22 dec).
     * To disable semicolons in the configuration file we reset the 4th bit using XOR.
     */
    config_set_options(&game_settings, 22 ^ CONFIG_OPTION_SEMICOLON_SEPARATORS);
 
	if(config_read_file(&game_settings, "../config/game_settings.cfg") != CONFIG_TRUE) {    // TEMPORARY LOCATION
		printf("\nError: game_settings.json missing!");
		return 2;
	}

    config_setting_t *root = config_root_setting(&game_settings);
    config_setting_t *letters = config_setting_lookup(root, "letters");
    config_setting_t *rounds = config_setting_lookup(root, "rounds");

    // If 0 is passed we don't change the setting
    if(new_letters != 0) config_setting_set_int(letters, new_letters);
    if(new_rounds != 0) config_setting_set_int(rounds, new_rounds);

    config_write_file(&game_settings, "../config/game_settings.cfg");   // TEMPORARY LOCATION

    config_destroy(&game_settings);

    return 0;
}


/* ============================================================================================= */
/* Public functions */


void gameSettings(int *letters, int *rounds) {

    char choice[30];    // the size of this array causes problems
    int value;
    int flag;
    int err_code;

    int new_letters;
    int new_rounds;
    
    do{
		printf(
			"\n"
			"\n"
			"			SCRABBLE\n" 
			"		  --------------------\n"
			"\n"
			"       (1)  Change number of letters (current: %d)\n"
			"       (2)  Change number of rounds (current: %d)\n"
			"       (3)  Back\n"
			"\n"
			"\n"
			"______________________________________________________\n",
            *letters, *rounds
		);
		printf("> ");
        scanf("%s", choice);

        // turn char value to int
        value = atoi(choice);

        switch(value) {

            case 1:
                flag = 0;

                do{
                    printf("How many letters do you want?: ");
                    scanf("%d", &new_letters);
                    
                    if(new_letters < 3 || new_letters > 26){
                        printf("\nInvalid, try again !\n");
                        continue;
                    }
                    
                    flag = 1;
                    err_code = change_settings(new_letters, 0);

                    if(err_code != 0) {
                        printf("Error: change_letters failed!");
                        return;
                    }
                    printf("Successfully updated!");
                }while(flag != 1);
                return;
                
            case 2:
            
                flag = 0;
                
                do{
                    printf("How many rounds do you want?: ");
                    scanf("%d", &new_rounds);    
                        
                    if(new_rounds < 1 || new_rounds > 99){
                        printf("\nInvalid, try again ! \n");
                        continue;    
                    }
                    
                    flag = 1;
                    err_code = change_settings(0, new_rounds);

                    if(err_code != 0) {
                        printf("Error: change_rounds failed!");
                        return;
                    }
                    printf("Successfully updated!");
                }while(flag != 1);
                return;
                
            case 3:
                return;

            default:
                // Invalid option
                system("clear");
                printf("Invalid, try again! \n\n");
                break;
            }
        
    }while(1);
}
