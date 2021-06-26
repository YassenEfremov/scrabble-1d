#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../../libs/jRead.h"
#include "../../libs/jWrite.h"


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

extern void change_letters(int changes, int rounds){
		
	FILE* settings_json = fopen("../json/settings.json", "w");
	
	if(!settings_json){
		printf("\nFile doesn't exist! ");
		return;
	}
	
	char buffer[70];
	jwOpen(buffer, sizeof(buffer), JW_OBJECT, JW_PRETTY);
	jwObj_int("letters", changes);
	jwObj_int("rounds", rounds);
	jwClose();
	fwrite(buffer, sizeof(buffer), 1, settings_json);
	
	fclose(settings_json);
}

extern void change_rounds(int letters, int changes){
		
	FILE* settings_json = fopen("../json/settings.json", "w");
	
	if(!settings_json){
		printf("\nFile doesn't exist! ");
		return;
	}
	
	char buffer[40];
	jwOpen(buffer, sizeof(buffer), JW_OBJECT, JW_PRETTY);
	jwObj_int("letters", letters);
	jwObj_int("rounds", changes);
	jwClose();
	fwrite(buffer, sizeof(buffer), 1, settings_json);
	
	fclose(settings_json);
}

// --------------------------------------------------------------------------------------------- // 
extern void gameSettings(int *letters, int *rounds) {
    int choice;    
    int new_letters;
    int new_rounds;
    
    do{

        printf("     (1)    Change number of letters: \n");
        printf("     (2)    Change number of rounds: \n");
        printf("     (3)    < Back \n");
        printf("> ");
        scanf("%d", &choice);

        if(choice == 1){

            int flag = 0;

            do{
                printf("How many letters do you want: ");
                scanf("%d", &new_letters);
                
                if(new_letters < 3 || new_letters > 26){
                    printf("\nInvalid, try again !\n");
                    continue;
                }
                
                flag = 1;
                change_letters(new_letters, *rounds);
                
                printf("Successfully updated!");
            }while(flag != 1);
            
        }else if(choice == 2){
        
            int flag = 0;
            
            do{
                printf("How many rounds do you want: ");
                scanf("%d", &new_rounds);    
                    
                if(new_rounds < 1 || new_rounds > 100){
                    printf("\nInvalid, try again ! \n");
                    continue;    
                }
                
                flag = 1;
                change_rounds(*letters, new_rounds);
                printf("Successfully updated!");
            }while(flag != 1);
            
        }else if(choice == 3){
            break;   
        }else {
            system("clear");
            printf("Invalid, try again! \n\n");
        }
        
    }while(choice < 1 || choice > 3);
    
}

// --------------------------------------------------------------------------------------------- //
