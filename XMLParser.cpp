#include "XMLParser.h"

XMLParser::XMLParser () : mFilename (DEFAULT_CONFIG_FILE) {
}

XMLParser::XMLParser (const std::string filename) : mFilename (filename) {
    isFilenameGiven = true;
}

bool XMLParser::parseFile () {
    // Check filename.
    if (isFilenameGiven && mFilename == "") {
        std::cout << " >> ERROR: Requested file parsing on empty filename!\n";
        return false;
    }

    std::ifstream xmlFile;

    // Open file
    xmlFile.open (mFilename, std::ios::in);
    if (!xmlFile.is_open ()) {
        std::cout << " >> ERROR: Cannot open XML config file! (file: " + mFilename + ")\n";
        return false;
    }

    std::string xmlContent = "";
    std::stringstream ss;

    // Copy file content into std::string.
    ss << xmlFile.rdbuf ();
    xmlContent = ss.str ();

    parseHeader (xmlContent);

    return true;
}

void XMLParser::parseHeader (std::string content) {
    // Find header
    std::string openString = "<?xml", closeString = "?>";
    unsigned int openPos = content.find (openString);
    unsigned int closePos = content.find (closeString);
    if (openPos != STRING_NOT_FOUND && closePos != STRING_NOT_FOUND) {

        // Parse header attributes.
        Tag headerTag;
        headerTag.name = "xml";
        if (!parseAttributes (content.substr (openPos + openString.size (),
            closePos - openPos - openString.size ()), headerTag)) {
#ifdef __VERBOSE
            std::cout << " >> WARNING: XML header's attributes are incorrect!\n";
#endif
        }
#ifdef __VERBOSE
        else {
            std::cout << "XML config file header:\n  ";
            for (Attribute & attr : headerTag.attributes) {
                std::cout << attr.name << "=\"" << attr.value << "\"  ";
            }
            std::cout << "\n\n";
        }
#endif
    }
    else {
#ifdef __VERBOSE
        std::cout << " >> WARNING: XML config file has incorrect header!\n";
#endif
    }
}

bool XMLParser::parseTag (std::string content) {

    return true;
}

bool XMLParser::parseAttributes (std::string content, Tag & tag) {
    // Return if no attributes.
    if (content.size () == 0 || content.find ("=") == STRING_NOT_FOUND)
        return false;

    std::string name = "", value = "", currentString = "";
    removePreceedingSpaces (content);
    unsigned int equalsPos = content.find ("=");

    // Acquire attributes.
    while (equalsPos != STRING_NOT_FOUND) {
        // Acquire name.
        try {
            name = content.substr (0, equalsPos);
            content = content.substr (equalsPos + 1, content.size () - equalsPos - 1);
        }
        catch (...) {
            std::cout << " >> ERROR: Parsing attribute name for tag '" << tag.name << "' failed!\n";
            return false;
        }

        // Acquire value.
        try {
            unsigned int quotePos = content.find ('"');
            // Find opening quote.
            if (quotePos != STRING_NOT_FOUND && quotePos == 0) {
                content = content.substr (1, content.size () - 1);  // Delete it.

                // Find closing quote.
                quotePos = content.find ('"');
                if (quotePos != STRING_NOT_FOUND) {
                    value = content.substr (0, quotePos);   // Set value.

                    // Erase value and closing quote from content.
                    content = content.substr (quotePos + 1, content.size () - quotePos - 1);
                }
                else {
                    std::cout << " >> ERROR: Invalid value for attribute '" << name << "'.\n";
                    return false;
                }
            }
            else {
                std::cout << " >> ERROR: Invalid value for attribute '" << name << "'.\n";
                return false;
            }
        }
        catch (...) {
            std::cout << " >> ERROR: Parsing attribute value for tag '" << tag.name << "' failed!\n";
            return false;
        }

        // Add new Attribute to Tag.
        Attribute attribute;
        attribute.name = name;
        attribute.value = value;
        tag.attributes.push_back (attribute);

        // Find new potential attribute.
        removePreceedingSpaces (content);
        equalsPos = content.find ("=");
    }

    return true;
}

void XMLParser::removePreceedingSpaces (std::string & content) {
    // Remove all spaces from position 0 to first non-space character.
    for (unsigned int pos = 0; pos < content.size (); ++pos) {
        if (content[pos] == ' ')
            continue;
        else {
            content = content.substr (pos, content.size () - pos);
            break;
        }
    }
}