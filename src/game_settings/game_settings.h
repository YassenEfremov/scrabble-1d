#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

extern void gameSettings(int *letters, int *rounds) {
    int choice;    
    int num_letters2;
    int num_rounds2;
    
    //system("clear");
    
    do{
    
        //system("clear");

        printf("     (1)    Change number of letters\n");
        printf("     (2)    Change number of rounds\n");
        printf("     (3)    < Back\n");
        scanf("%d", &choice);
        
        if(choice == 1){
        
            int flag = 0;
            
            do{
                printf("\nHow many letters do you want: ");
                scanf("%d", &num_letters2);
            
                if(num_letters2 < 3 || num_letters2 > 27){
                    printf("\nInvalid, try again !\n");
                    continue;
                }
                
                flag = 1;
                *letters = num_letters2;
                
            }while(flag != 1);

        }else if(choice == 2){
        
            int flag = 0;
            
            do{
                printf("How many rounds do you want: ");
                scanf("%d", &num_rounds2);    
                    
                if(num_rounds2 < 1 || num_rounds2 > 100){
                    printf("\nInvalid, try again ! \n");
                    continue;    
                }
                
                flag = 1;
                *rounds = num_rounds2;
                    
            }while(flag != 1);

        }else if(choice == 3) {
            break;

        }else {
            system("clear");
            printf("Invalid, try again ! \n\n");
        }
            
    }while (choice < 1 || choice > 3);
	
}
