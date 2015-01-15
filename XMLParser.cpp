#include "XMLParser.h"

XMLParser::XMLParser () : mFilename (DEFAULT_CONFIG_FILE) {
}

XMLParser::XMLParser (const std::string filename) : mFilename (filename) {
    isFilenameGiven = true;
}

bool XMLParser::parseFile () {
    // Set root tag.
    rootTag.name = ROOT_TAG_NAME;

    // Check filename.
    if (isFilenameGiven && mFilename == "") {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Requested file parsing on empty filename!\n";
        return false;
    }

    std::ifstream xmlFile;

    // Open file
    xmlFile.open (mFilename, std::ios::in);
    if (!xmlFile.is_open ()) {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Cannot open XML config file! (file: " + mFilename + ")\n";
        return false;
    }

    std::string xmlContent = "";
    std::stringstream ss;

    // Copy file content into std::string. Remove new lines.
    ss << xmlFile.rdbuf ();
    xmlContent = ss.str ();
    Utilities::replaceKeyword ("\n", "", xmlContent);

    // Parse content
    if (parseHeader (rootTag, xmlContent)) {
        parseTag (xmlContent, rootTag);
    }
    else
        return false;

#ifdef __VERBOSE
    // Print XML Network config.
    std::cout << "XML config file structure\n--------------------------\n";
    this->printRecursive (rootTag, 0);
#endif

    return true;
}

XMLParser::Tag * XMLParser::searchTagByName (Tag & tag, const std::string name, int & occur) {
    // Check current tag
    if (tag.name == name) {
        --occur;
        return &tag;
    }
    else {
        if (tag.isEmpty || tag.tags.size () == 0) {
            return nullptr;
        }
        else {
            Tag * searchedTag = nullptr;
            for (Tag & childTag : tag.tags) {
                searchedTag = searchTagByName (childTag, name, occur);
                if (searchedTag != nullptr) {
                    if (occur < 0)
                        return searchedTag;
                }
            }
        }
    }

    return nullptr;
}

unsigned int XMLParser::getOccurences (Tag & parentTag, Tag & searchedTag) {
    return 0;
}

XMLParser::Tag * XMLParser::getRootTag () {
    return &rootTag;
}

bool XMLParser::parseHeader (Tag & headerTag, std::string & content) {
    // Find header
    removePreceedingSpaces (content);
    std::string openString = "<?xml", closeString = "?>";
    unsigned int openPos = content.find (openString);
    unsigned int closePos = content.find (closeString);
    if (openPos != STRING_NOT_FOUND && closePos != STRING_NOT_FOUND) {

        // Parse header attributes.
        headerTag.name = "xml";
        headerTag.isEmpty = false;
        if (!parseAttributes (content.substr (openPos + openString.size (),
            closePos - openPos - openString.size ()), headerTag)) {
#ifdef __VERBOSE
            std::cout << "\n <!> WARNING <!>\n";
            std::cout << "  XML header's attributes are incorrect!\n";
#endif
            return false;
        }

        // Erase header from content
        content = content.substr (closePos + closeString.size (),
                                  content.size () - closePos - closeString.size ());
    }
    else {
#ifdef __VERBOSE
        std::cout << "\n <!> WARNING <!>\n";
        std::cout << "  XML config file has incorrect header!\n";
#endif
        return false;
    }

    return true;
}

bool XMLParser::parseTag (std::string content, Tag & tag) {
    // Prepare content and variables.
    removePreceedingSpaces (content);
    unsigned int openTagBegin = content.find ("<");
    unsigned int openTagEnd = content.find (">");
    unsigned int closeTagPos = 0;
    std::string name = "", closeTag = "";
    bool isTagEmpty = false;

    // Repeat while opening tag found.
    while (openTagBegin != STRING_NOT_FOUND && openTagEnd != STRING_NOT_FOUND) {

        // New objects and variables.
        Tag newTag;
        std::string openTagContent = content.substr (openTagBegin + 1, openTagEnd - 1);
        removePreceedingSpaces (openTagContent);
        unsigned int afterNameSpacePos = openTagContent.find (" ");
        unsigned int slashPos = openTagContent.find ("/");

        // Check for empty tag.
        if (slashPos != STRING_NOT_FOUND) {
            isTagEmpty = true;
        }

        // No space found => tag with no attributes.
        if (afterNameSpacePos == STRING_NOT_FOUND) {
            if (!isTagEmpty)
                name = openTagContent;
            else
                name = openTagContent.substr (0, slashPos);
        }
        // Space found => possible attributes.
        else {
            name = openTagContent.substr (0, afterNameSpacePos);
            parseAttributes (openTagContent.substr (afterNameSpacePos, openTagContent.size () - afterNameSpacePos), newTag);
        }

        // Set tag parameters.
        newTag.name = name;
        newTag.isEmpty = isTagEmpty;

        // Search for close tag.
        closeTag = "</" + name + ">";
        closeTagPos = content.find (closeTag);
        if (!isTagEmpty) {
            // Closing tag found.
            if (closeTagPos != STRING_NOT_FOUND) {

                // RECURSIVELY CALL THIS METHOD FOR CONTENT BETWEEN TAGS.                
                parseTag (content.substr (openTagEnd + 1, closeTagPos - openTagEnd - 1), newTag);

                // Erase already parsed content.
                content = content.substr (closeTagPos + closeTag.size (),
                                          content.size () - closeTag.size () - closeTagPos);
            }
            else {
                // ERROR close tag should be found
                return false;
            }
        }
        else {
            // Erase already parsed content.
            closeTag = "/>";
            closeTagPos = content.find (closeTag);
            content = content.substr (closeTagPos + closeTag.size (),
                                      content.size () - closeTag.size () - closeTagPos);
        }

        // Insert new tag.
        tag.tags.push_back (newTag);

        //Get new positions. Reset parameters.
        removePreceedingSpaces (content);

        if (content.size () == 0)
            break;

        openTagBegin = content.find ("<");
        openTagEnd = content.find (">");
        closeTagPos = 0;
        name = "";
        closeTag = "";
        isTagEmpty = false;
    }

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
            std::cout << "\n <!> ERROR <!>\n";
            std::cout << "  Parsing attribute name for tag '" << tag.name << "' failed!\n";
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
                    removePreceedingSpaces (content);
                }
                else {
                    std::cout << "\n <!> ERROR <!>\n";
                    std::cout << "  Invalid value for attribute '" << name << "'.\n";
                    return false;
                }
            }
            else {
                std::cout << "\n <!> ERROR <!>\n";
                std::cout << "  Invalid value for attribute '" << name << "'.\n";
                return false;
            }
        }
        catch (...) {
            std::cout << "\n <!> ERROR <!>\n";
            std::cout << "  Parsing attribute value for tag '" << tag.name << "' failed!\n";
            return false;
            // Find new potential attribute.
            removePreceedingSpaces (content);
        }

        // Add new Attribute to Tag.
        Attribute attribute;
        attribute.name = name;
        attribute.value = value;
        tag.attributes.push_back (attribute);

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

void XMLParser::printRecursive (Tag & tag, const unsigned int indent) {
    // Print open tag.
    std::cout << std::setw (indent) << "" << "<" << tag.name;

    // Print attributes.
    for (Attribute & attr : tag.attributes) {
        std::cout << " " << attr.name << "=\"" << attr.value << "\"";
    }

    // Close open tag.
    if (tag.isEmpty)
        std::cout << " />\n";
    else
        std::cout << ">\n";

    // Print nested tags if exist.
    for (Tag & tag : tag.tags) {
        printRecursive (tag, indent + 2);
    }

    // Print closing tag.
    if (!tag.isEmpty)
        std::cout << std::setw (indent) << "" << "</" << tag.name << ">\n";
}