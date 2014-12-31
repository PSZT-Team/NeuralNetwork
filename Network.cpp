#include "Network.h"

Network::Network (int argc, char* argv[]) : mInterface (argc, argv) {
	
}

void Network::run () {
    // TEMP
	mInterface.printParams();
	mInterface.printHelp ();
	mInterface.printUsage ();

    // Acquire data
	std::string filename = mInterface.getStringParam("--file");
    if (filename != "") {
        mDataCollector.setFilename (filename);
        mDataCollector.loadData ();
    }
    else {
        std::cout << " >> No input file to read from!\n";
        return;
    }

    // Saving results
    filename = mInterface.getStringParam ("--output");
    if (filename != "") {
        mOutputManager.setResultsFilename (filename);
    }
    mOutputManager.saveResults (&mDataCollector);

    // Saving stats
}