/**
    Organizes input parameters, filenames and displays useful information.
*/
#ifndef INTERFACE
#define INTERFACE

#include <iostream>
#include <string>
#include <map>

class Interface {
public:    
    Interface () {};
    ~Interface () {};

    // Constructor with input parameters
    Interface (int argc, char* argv[]);

    // Configures maps with proper parameters
    void configureParameters ();

    // Prints all input parameters
    void printParams ();

    // Prints some helpful information to the user before terminating.
    void printUsage ();
    void printHelp ();

	// Getters
	std::string getStringParam (const std::string key);

private:
    int mArgCount = 0;

    std::string mUsageMessage = "Usage msg.";    // TEMP
    std::string mHelpMessage = "Help msg.";

    // Param name => (if exists; associated param)
    std::map <std::string, std::pair<bool, std::string>> mapParamBools;

    // Param name => (filename; associated param)
    std::map <std::string, std::pair<std::string, std::string>> mapParamStrings;
};

#endif

