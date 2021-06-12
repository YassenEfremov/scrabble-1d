#include <stdio.h>
#include <stdlib.h>


// ============================================================================================= //


#ifndef FILE_CONTENTS_TO_STRING
#define FILE_CONTENTS_TO_STRING

extern char *copyFileContentsToString(FILE **file) {

    // Get the number of letters in the file
    fseek(*file, 0, SEEK_END);   // set file pos. indicator at the end
    long dict_size = ftell(*file);   // get the value of the pos. indicator = bytes in the file
    fseek(*file, 0, SEEK_SET);   // reset pos. indicator

    // Save them in a buffer
    char *buffer = (char *)malloc(dict_size + 1);
    fread(buffer, dict_size, 1, *file);

    buffer[dict_size] = '\0';   // last char is terminating zero

    return buffer;
}

#endif