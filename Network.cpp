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

void Network::initializeLayers (const int & inputNeuron, const int & hiddenNeuron, const int & outputNeuron) {
	Layer newLayer;
	
	newLayer.initializeLayer (inputNeuron);
	mLayers.push_back (newLayer);

	newLayer.initializeLayer (hiddenNeuron);
	mLayers.push_back (newLayer);

	newLayer.initializeLayer (outputNeuron);
	mLayers.push_back (newLayer);

	learn (mDataCollector.getLearningData(10,15));

}

void Network::learn (const std::vector<const ProteinData* &> & data) {

	for (auto &dataSet : data) {
		
		mLayers [0].setOutput (dataSet);
	
		for (int i = 1; i < mLayers.size (); ++i)  {
			mLayers [i].commandToLearn (mLayers [i - 1]);
		}
	}

}

