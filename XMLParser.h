#ifndef XML_PARSER
#define XML_PARSER

#define STRING_NOT_FOUND std::string::npos
#define DEFAULT_CONFIG_FILE "./Data/network.config"
#define ROOT_TAG_NAME "Root";

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Utilities.h"

/**
    XML file parser. Stores content of XML file in tree-like form.
    All names and values are of type std::string.
    =======================
    Notice:
    - '<', '/' and '>' characters can be only used within tag declaration.
    =======================
    Not supported:
    - XML comments.
    - Nested tags with the same name. Only supported when listed on after another.
    - Spaces in closing tags.
*/
class XMLParser {
public:
    // Default constructor with DEFAULT_CONFIG_FILE path.
    XMLParser ();

    // Default destructor.
    ~XMLParser () {
    };

    // Constructor with filename argument.
    // \param string filename : Name of the file to be parsed.
    XMLParser (const std::string filename);

    // Tag attribute: Name="Value"
    struct Attribute {
        std::string name = "";   // Name of the attribute;
        std::string value = "";  // Attribute value;
    };

    // XML Tag. Two variants:
    // <tag>...</tag>
    // <emptyTag />
    struct Tag {
        //Tag (const std::string name, bool isEmpty) : name (name), isEmpty (isEmpty) {
        //}
        bool isEmpty = false;                       // True if tag has no nested tags. False otherwise.
        std::string name = "";                   // Name of the tag.
        std::vector<Attribute> attributes;  // Vector of possible attributes the tag contains.
        std::vector<Tag> tags;              // Vector of possible nested tags.
    };

    // Parse given file. Returns true if operation was successful, false otherwise.
    bool parseFile ();

    // Search recursively for value in XML structure.
    // \param Tag tag : Currently searched tag.
    // \param Type : Expecting type of return value to be obtained from string.
    // \param string tagName : First matched tag name to obtain attribute from.
    template <typename Type>
    bool searchValue (Tag & tag, const std::string & tagName,
                      const std::string & attributeName, Type & value);

    // Search for tag by name. Return given occurence or nullptr (for ex. if exceeded).
    // \param Tag tag : Currently searched tag.
    // \param string name : Name of the tag to be found.
    // \param int occur : Number of tag occurence to be returned (starting from 0).
    // WARNING: Check XMLParser class comment for important disclaimer.
    Tag * searchTagByName (Tag & tag, const std::string name, int & occur);

    // Count occurences of specified tag inside another given tag.
    // \ param Tag parentTag : Tag inside which the search is performed.
    // \ param Tag searchedTah : Tag to be found and which occurences are counted.
    unsigned int getOccurences (Tag & parentTag, Tag & searchedTag);

    /// Getters

    Tag * getRootTag ();  // Get pointer to vector containing root tags.


private:
    /// Operations on tags and attributes.

    bool parseHeader (Tag & headerTag, std::string & content);        // Parse special <? ... ?> tag (no need to have though).
    bool parseTag (std::string content, Tag & tag);           // Parse tag. Recursively called for nested tags.
    bool parseAttributes (std::string content, Tag & tag);    // Parse tag attributes.
    void removePreceedingSpaces (std::string & content);    // Remove spaces that preceed tag or attribute (content in general).

    // Print the whole xml structure.
    // \param Tag tag : Tag to begin printing from.
    // \indent int indent : Number of spaces to be inserted before each level of tag structure.
    void printRecursive (Tag & tag, const unsigned int indent);

    // Root "tag".
    Tag rootTag;

    // Name of the file to be parsed.
    std::string mFilename = "";

    // True if filename was given (default constructor was not used). False otherwise.
    bool isFilenameGiven = false;

};

#endif

template <typename Type>
inline bool XMLParser::searchValue (Tag & tag, const std::string & tagName,
                                    const std::string & attributeName, Type & value) {
    // Check name of the tag.
    if (tag.name == tagName) {

        // Check attributes of current tag.
        for (Attribute & attr : tag.attributes) {
            if (attr.name == attributeName) {

                // Convert string value to Type value.
                std::stringstream ss;
                ss.str (attr.value);
                ss >> value;
                return true;
            }
        }
    }

    // No need to go deeper.
    if (tag.isEmpty || tag.tags.size () == 0) {
        return false;
    }
    else {
        for (Tag & childTag : tag.tags) {
            if (searchValue (childTag, tagName, attributeName, value))
                return true;
        }
    }

    return false;
}
