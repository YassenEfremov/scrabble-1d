#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>      // atoi
#include "game_settings.h"

#include "libs/jRead.h"
#include "libs/jWrite.h"


/* ============================================================================================= */


int change_letters(int new_letters, int rounds) {
		
	FILE* settings_json = fopen("../../config/game_settings.json", "w");
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


/* --------------------------------------------------------------------------------------------- */


int change_rounds(int new_rounds, int letters) {
		
	FILE* settings_json = fopen("../../config/game_settings.json", "w");
	
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


/* ============================================================================================= */


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
                    err_code = change_letters(new_letters, *rounds);

                    if(err_code != JWRITE_OK) {
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
                    err_code = change_rounds(new_rounds, *letters);

                    if(err_code != JWRITE_OK) {
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
