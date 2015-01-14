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

	//initializeLayers (3, 8, 1);
	
	std::vector<int> lay;

	lay.push_back (3);
	lay.push_back (4);
	lay.push_back (1);
	
	initializeLayers (lay);
	

	//for (int i = 1; i < 150; i++)
    //learn (mDataCollector.getLearningData (1, 100));

    for (unsigned int i = 0; i < CV_ITERATIONS_NUMBER; ++i) {
        learn (mDataCollector.getLearningData (mCrossValidator.getIterationStartRecordPosition (),
            mCrossValidator.getTestSetSize ()));
        CrossValidator::TestResults testResults;
        test (testResults, mDataCollector.getCheckData (mCrossValidator.getIterationStartRecordPosition (),
            mCrossValidator.getTestSetSize ()));
        mCrossValidator.addIterationInfo (testResults);        
    }
	//learn (mDataCollector.getLearningData (0, 100));

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

void Network::initializeLayers (std::vector<int> layers) {

	mLayersTable.push_back (new Layer (layers[0], 1));
	for (int i = 1; i < layers.size (); ++i )
		mLayersTable.push_back (new Layer (layers[i], layers[i-1]));

}


void Network::learn (const std::vector<ProteinData *> data) {
	int iter = 0;
	int TP = 0;
	int TN = 0;
	int FP = 0;
	int FN = 0;
	float RMS = 0;
	float ERMS = 1;

	for (auto &dataSet : data)  {
		++iter;
		//std::cout << " expected result: " << dataSet->getReactionResult() << "\t resutlt :";

		// PART 1 - CALCULATING OUTPUT
		 
		mLayersTable [0]->setOutput (dataSet);
		
		for (int i = 1; i < mLayersTable.size (); ++i)  {
			mLayersTable [i]->commandToCalculate (*mLayersTable [i - 1]);
		}

		//mLayersTable [2]->printOutput ()<< std::endl;


		if (iter >= 0)  {
			float a = mLayersTable [2]->getOutput ();

			if (a > 0)  {
				if (dataSet->getReactionResult () > 0)
					++TP;
				else
					++FN;
			}
			else {
				if (dataSet->getReactionResult () < 0)
					++TN;
				else
					++FP;
			}

				
		}

		// PART 2 - INSTRUCTOR AND LEARNING
		mLayersTable [mLayersTable.size () - 1]->outputLayerError (dataSet->getReactionResult());

		for (int i = mLayersTable.size() - 2; i >= 1 ; --i) {
			mLayersTable [i]->calculateErrors (*mLayersTable [i + 1]);
		}


		// PART 3 - WEIGHT CORRECTION

		for (int i = 1; i < mLayersTable.size (); ++i) {
			mLayersTable [i]->correctWeights (*mLayersTable [i - 1]);
		}
		

	//	RMS += (Wy [wu][j] - O [LW - 1][j])*(Wy [wu][j] - O [LW - 1][j]);
		RMS += (dataSet->getReactionResult() - mLayersTable [2]->getOutput ()) * (dataSet->getReactionResult() + mLayersTable [2]->getOutput ());
		

	}
	ERMS = sqrt (RMS / (double) (data.size()*2));

#ifdef __VERBOSE
	std::cout << "RESULTS" << std::endl;
    //std::cout << "TP: " << TP << std::endl;
	//std::cout << "FP: " << FP << std::endl;
	//std::cout << "TN: " << TN << std::endl;
	//std::cout << "FN: " << FN << std::endl;
	std::cout << "t: " << TP + TN << std::endl;
	std::cout << "f : " << FN + FP << std::endl;
    std::cout << " \t ERMS : " << ERMS << std::endl;
#endif
}

void Network::test (CrossValidator::TestResults & results, std::vector<ProteinData*> data) {
    // PART 1 - CALCULATING OUTPUT

    for (auto & dataSet : data) {


        mLayersTable[0]->setOutput (dataSet);

        for (int i = 1; i < mLayersTable.size (); ++i) {
            mLayersTable[i]->commandToCalculate (*mLayersTable[i - 1]);
        }

        //mLayersTable [2]->printOutput ()<< std::endl;


      
            float a = mLayersTable[mLayersTable.size() - 1]->getOutput ();

            if (a > 0) {
                if (dataSet->getReactionResult () > 0)
                    results.push_back (std::make_pair (true, true));
                else
                    results.push_back (std::make_pair (true, false));
            }
            else {
                if (dataSet->getReactionResult () < 0)
                    results.push_back (std::make_pair (false, false));
                else
                    results.push_back (std::make_pair (false, true));
            }


        

    }
}
