#include "Network.h"

Network::Network (int argc, char* argv[]) : mInterface (argc, argv) {
	
}

void Network::run () {
    // TEMP
	mInterface.printParams();
	mInterface.printHelp ();
	mInterface.printUsage ();

    if (!this->acquireData ())
        return;

    this->saveResults ();
    this->saveStats ();
}

bool Network::acquireData () {
    std::string filename = mInterface.getStringParam ("--file");
    if (filename != "") {
        mDataCollector.setFilename (filename);
        mDataCollector.loadData ();
    }
    else {
        std::cout << " >> No input file to read from!\n";
        return false;
    }

    return true;
}

void Network::saveResults () {
    // Saving results
    std::string filename = mInterface.getStringParam ("--output");
    if (filename != "") {
        mOutputManager.setResultsFilename (filename);
        mOutputManager.saveResults (&mDataCollector, false);
    }
    else
        mOutputManager.saveResults (&mDataCollector);
}

void Network::saveStats () {
    // Saving stats
    std::string filename = mInterface.getStringParam ("--stats");
    if (filename != "") {
        mOutputManager.setStatsFilename (filename);
        mOutputManager.saveStats (false);
    }
    else
        mOutputManager.saveStats ();
}