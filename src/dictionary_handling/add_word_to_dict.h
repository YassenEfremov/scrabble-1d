#include <stdio.h>


// ============================================================================================= //


extern void addWordToDict() {

    // open the file in append mode
    FILE *dict = fopen("./dictionary_handling/dictionary.csv", "a");

    // take user input
    char word[46];
    scanf("%s", word);

    // append the word
    fprintf(dict, ",%s", word);

    // close the file
    fclose(dict);
}