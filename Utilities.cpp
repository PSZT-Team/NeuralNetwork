#include "Utilities.h"

void Utilities::replaceKeyword (const std::string keyword, const std::string value, std::string & text) {
    // If keyword is not found, print an error.
    unsigned int keywordPos = text.find (keyword);
    if (keywordPos == std::string::npos) {
        std::cout << " >> ERROR: Keyword '" + keyword + "' cannot be found!\n";
        return;
    }

    // Otherwise...
    while (keywordPos != std::string::npos) {
        std::string before = "", after = "";
        before = text.substr (0, keywordPos);
        after = text.substr (keywordPos + keyword.size (), text.size () - (keywordPos + keyword.size ()));

        text = before + value + after;
        keywordPos = text.find (keyword);
    }
}