/**
    Main project class. Aggregates all network layers, interface and statistics.
*/

#ifndef NETWORK
#define NETWORK

#include "Interface.h"
#include "DataCollector.h"
#include "OutputManager.h"
#include <iostream>

class Network {
public:
    Network () {};
    ~Network () {};

    // Constructor with input parameters to be passed to Interface
    Network (int argc, char* argv[]);

    // Main class method. Includes all operations and interfaces.
    void run ();

private:
    Interface mInterface;
	DataCollector mDataCollector;
    OutputManager mOutputManager;
};

#endif
