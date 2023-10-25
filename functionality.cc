//
//

#include "functionality.h"
#include <sstream>
#include <string.h>


//May need to test it independently
char** split(char* char_string, char delim_char){
    char** parts;
    int i = 0;
    char delim[2];
    delim[0] = delim_char;
    delim[1] = '\0';

    char *token = strtok(char_string, delim);

    while (token != NULL) {
        parts[i] = token;

        // Get the next token
        token = strtok(NULL, delim);
        i++;
    }

    return parts;
    
}