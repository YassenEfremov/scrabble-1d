#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void trieGenerate(char *word) {
    // generate trie from string
}


extern void extractSringFromDict() {
    
    FILE *dict  = fopen("./dictionary_handling/dictionary.csv", "r");
    if(!dict){
        printf("\nThere is no dictionary.");
        return;
    }

    fseek(dict, 0, SEEK_END);   // set file position indicator at the end
    long dict_size = ftell(dict);   // get the value of the pos indicator = bytes in the file
    fseek(dict, 0, SEEK_SET);   // reset pos indicator

    char *buffer = (char *)malloc(dict_size + 1);
    fread(buffer, 1, dict_size, dict);
    fclose(dict);

    buffer[dict_size] = '\0';


    char *token = strtok(buffer, ",");
    while(token != NULL) {
        // add to trie
        printf("%s\n", token);
        token = strtok(NULL, ",");
        
    }


    /*
    char buffer[45];
    fgets(buffer, sizeof(buffer)/sizeof(char), dict);
    printf("%ld  ", sizeof(buffer)/sizeof(char));
    */

    free(buffer);
}
