/**
	Main project class. Aggregates all network layers, interface and statistics.
*/

#ifndef NETWORK
#define NETWORK

#include "Interface.h"
#include "DataCollector.h"
#include "OutputManager.h"
#include "CrossValidator.h"
#include "Layer.h"
#include "XMLParser.h"
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
	// Load data from file (if given)
	bool acquireData ();

	// Saving results to file (if needed)
	void saveResults ();

	// Saving stats to file (if needed)
	void saveStats ();

	void initializeLayers (const int& inputLayer, const int& hiddenLayer, const int& outputLayer);

	void initializeLayers (std::vector<int> layers);


	void learn (const std::vector<ProteinData *> data);
    void test (CrossValidator::TestResults & results, std::vector<ProteinData*> data);

	// Network modules
	Interface mInterface;
	DataCollector mDataCollector;
	OutputManager mOutputManager;
	CrossValidator mCrossValidator;
    XMLParser mXMLParser;
	
	std::vector <Layer*> mLayersTable;

};

#endif

