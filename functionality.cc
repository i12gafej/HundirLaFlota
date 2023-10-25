//
//

#include "functionality.h"
#include <sstream>
#include <string.h>

char** split(char* char_string, char delim_char){
    char** parts;
    int i = 0;
    char* delim = &delim_char;

    char *token = strtok(char_string, delim);

    while (token != NULL) {
        parts[i] = token;

        // Get the next token
        token = strtok(NULL, delim);
        i++;
    }

    return parts;
    
}