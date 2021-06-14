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

//------------------------------------------------------------------//

extern void write_settings(int line)
{
    
    int BUFFER_SIZE = 10;
    FILE * fPtr;
    FILE * fTemp;

    char buffer[BUFFER_SIZE];
    char newline[BUFFER_SIZE];
    int count;

    // Remove extra new line character from stdin
    fflush(stdin);

    //max letters must be 26
    //max rounds must be 99
    if(line == 1){
        printf("change the number of letters to:");
    }
    else{
        printf("change the number of rounds to:");
    }

    getchar();
    fgets(newline, BUFFER_SIZE, stdin);

    /////////To Add another string variable
    ///////// Amount of letter: newline


    //open our needed fines
    fPtr  = fopen("./game_settings/num_of_let_and_rounds.txt", "r");
    fTemp = fopen("./game_settings/replace.tmp", "w"); 

    //check if giles are unavle to open
    if (fPtr == NULL || fTemp == NULL)
    {
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
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL)
    {
        count++;


        if (count == line)
            fputs(newline, fTemp);
        else
            fputs(buffer, fTemp);
    }


    fclose(fPtr);
    fclose(fTemp);


    //Delete original source file
    remove("./game_settings/num_of_let_and_rounds.txt");

    // Rename temporary file as original file 
    rename("./game_settings/replace.tmp", "./game_settings/num_of_let_and_rounds.txt");

    printf("\nSuccessfully updated");

    return;
}

//-----------------------------------------------------------------//

extern void choose_option(){
    
    int option;
    do{

        printf("     (1)    Change number of letters\n");
        printf("     (2)    Change number of rounds\n");
        printf("     (3)    < Back\n");
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
    
    return;
}
//--------------------------------------------------//
