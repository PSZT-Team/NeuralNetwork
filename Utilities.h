/**
    Non-instantiable class. Provides various string operations.
*/

#ifndef UTILITIES
#define UTILITIES

#include <string>
#include <iostream>

class Utilities {
public:
    // Replace file keyword with proper data
    // \param string keyword : Keyword in file to be found and replaced.
    // \param string value : String to be inserted instead of the given keyword.
    // \param string & text : Reference to text, in which the replacement is done.
    static void replaceKeyword (const std::string keyword, const std::string value, std::string & text);
};

#endif

