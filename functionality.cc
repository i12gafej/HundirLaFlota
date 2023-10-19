//
//

#include "functionality.h"
#include <sstream>

std::vector<std::string> split(char* char_string, char delim_char){
    std::string placeholder(char_string);

    std::istringstream stream(placeholder);
    std::vector<std::string> parts;
    std::string part;

    while (std::getline(stream, part, delim_char)) {
        parts.push_back(part);
    }

    return parts;
    
}