#include "Network.h"

Network::Network (int argc, char* argv[]) : mInterface (argc, argv) {
	
}

void Network::run () {
	mInterface.printParams();
	mInterface.printHelp ();
	mInterface.printUsage ();

	std::string filename = mInterface.getStringParam("--file");
	if (filename != "") {
		mDataCollector.setFilename(filename);
		mDataCollector.loadData();
	}	

	std::cout << "Press Enter to continue...";
	std::cin.get ();
}