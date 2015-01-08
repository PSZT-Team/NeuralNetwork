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

	mCrossValidator.setCapacity (mDataCollector.getDataCount ());
	
	// TEMP
	mCrossValidator.fillWithRandomData ();

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
		std::cout << " >> ERROR: No input file to read from!\n";
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
		mOutputManager.saveStats (&mCrossValidator,false);
	}
	else
		mOutputManager.saveStats (&mCrossValidator);
}

void Network::initializeLayers (const int& inputLayer, const int& hiddenLayer, const int& outputLayer) {
	mLayersTable.push_back (new Layer (inputLayer, 1));
	mLayersTable.push_back (new Layer (hiddenLayer, inputLayer));
	mLayersTable.push_back (new Layer (outputLayer, hiddenLayer));
}

void Network::learn (const std::vector<ProteinData *> data) {

	for (auto &dataSet : data)  {
		

		mLayersTable [0]->setOutput (dataSet);
		
		for (int i = 1; i < mLayersTable.size (); ++i)  {
			mLayersTable [i]->commandToCalculate (*mLayersTable [i - 1]);
		}

	
		// PART 2 - INSTRUCTOR AND LEARNING
		// i don't know where to put this error yet
		//float error = dataSet->getReactionResult() - mLayersTable [mLayersTable.size () - 1]->getNeurons () [0]->getOutput ();

		
	}
	
}