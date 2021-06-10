#include <stdio.h>

// Here are the sittings for letters and rounds

extern void gameSettings(int *letters, int *rounds) {
    int choice;    
    int num_letters2;
    int num_rounds2;
    
    system("clear");
    
    do{
    
    printf("     (1)   Change number of letters: \n");
    printf("     (2)      Change number of rounds: \n");
    scanf("%d", &choice);
    
    if(choice == 1){
    
        int flag = 0;
        
        do{
            printf("How many letters do you want: \n");
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
            printf("How many rounds do you want:  \n");
            scanf("%d", &num_rounds2);    
                
            if(num_rounds2 < 1 || num_rounds2 > 100){
                printf("\nInvalid, try again ! \n");
                continue;    
            }
            
            flag = 1;
            *rounds = num_rounds2;
                
        }while(flag != 1);
    
    }
            
    }while (choice < 0 || choice > 3);
	
}
