#include <stdio.h>
#include <stdlib.h>


// ============================================================================================= //

#ifndef FILE_CONTENTS_TO_STRING
#define FILE_CONTENTS_TO_STRING

char *copyFileContentsToString(FILE **file) {

    // Get the number of letters in the file
    fseek(*file, 0L, SEEK_END);   // set file pos. indicator at the end
    long file_size = ftell(*file);   // get the value of the pos. indicator = bytes in the file
    rewind(*file);  // reset pos. indicator

    // Save them in a buffer
    char *buffer = (char *)malloc((file_size + 1) * sizeof(char));
    fread(buffer, file_size, 1, *file);

    buffer[file_size] = '\0';   // last char is terminating zero

    return buffer;
}

#endif
