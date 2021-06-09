#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void trie_generate(){
    
    FILE *dict  = fopen("./dictionary_to_trie/dictionary.csv", "r");
    if(!dict){
        printf("sori");
        return;
    }
    char buffer[45];
    fgets(buffer, sizeof(buffer)/sizeof(char), dict);
    printf("%ld   ", sizeof(buffer)/sizeof(char));
    printf("%s", buffer);


    fclose(dict);
}
