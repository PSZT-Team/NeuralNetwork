/**
    XML type file parser. Stores content of xml file in tree-like form.
    All names and values are of type std::string.
*/

#ifndef XMLPARSER
#define XMLPARSER

#include <string>
#include <vector>

class XMLParser {public:
    XMLParser () {
    };
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
        bool isEmpty;                       // True if tag has no nested tags. False otherwise.
        std::string name;                   // Name of the tag.
        std::vector<Attribute> attributes;  // Vector of possible attributes the tag contains.
        std::vector<Tag> tags;              // Vector of possible nested tags.
    };

    // Parse given file. Returns true if operation was successful, false otherwise.
    bool parseFile ();

private:
    // Name of the file to be parsed.
    std::string mFilename = "";

};

#endif

