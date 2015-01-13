/**
    XML file parser. Stores content of xml file in tree-like form.
    All names and values are of type std::string.
*/

#ifndef XMLPARSER
#define XMLPARSER

#define STRING_NOT_FOUND std::string::npos
#define DEFAULT_CONFIG_FILE "./Data/network.config"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class XMLParser {public:
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
        bool isEmpty = true;                       // True if tag has no nested tags. False otherwise.
        std::string name = "";                   // Name of the tag.
        std::vector<Attribute> attributes;  // Vector of possible attributes the tag contains.
        std::vector<Tag> tags;              // Vector of possible nested tags.
    };

    // Parse given file. Returns true if operation was successful, false otherwise.
    bool parseFile ();

    /// Getters

    std::vector<Tag> * getRootTags ();  // Get pointer to vector containing root tags.

private:
    /// Operations on tags and attributes.

    void parseHeader (std::string content);        // Parse special <? ... ?> tag (no need to have though).
    bool parseTag (std::string content);           // Parse tag. Recursively called for nested tags.
    bool parseAttributes (std::string content, Tag & tag);    // Parse tag attributes.
    void removePreceedingSpaces (std::string & content);    // Remove spaces that preceed tag or attribute (content in general).

    // Contains root tags.
    std::vector <Tag> vecRootTags;

    // Name of the file to be parsed.
    std::string mFilename = "";

    // True if filename was given (default constructor was not used). False otherwise.
    bool isFilenameGiven = false;

};

#endif

