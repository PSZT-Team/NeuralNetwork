#include "Network.h"

Network::Network (int argc, char* argv[]) : mInterface (argc, argv), a ("testData.txt"){
	mDataCollector = new DataCollector (a);
}

void Network::run () {
	mInterface.printParams();
	mInterface.printHelp ();
	mInterface.printUsage ();
	mDataCollector->loadData ();
	std::cin.get ();
}