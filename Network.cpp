#include "Network.h"

Network::Network(int argc, char* argv[])
    : mInterface(argc, argv)
{}

void Network::run()
{
    // Handle parameters.
    std::cout << ">>> Acquiring parameters <<<\n";
    mInterface.printParams();

    if (mInterface.getBoolParam("-h"))
    {
        mInterface.printHelp();
        return;
    }

    if (mInterface.getBoolParam("-u"))
    {
        mInterface.printUsage();
        return;
    }


    // Obtain data.
    std::cout << "\n>>> Obtaining data from file <<<\n";

    if (!this->acquireData())
        return;
    mCrossValidator.setCapacity(mDataCollector.getDataCount());


    // Obtain learning parameters and layers' details.
    std::cout << "\n>>> Parsing XML config file <<<\n";

    mXMLParser.parseFile();
    loadParameters();

    // Optional normalization.
    std::string normalizeValue = "";
    if (mXMLParser.searchValue<std::string>(*mXMLParser.getRootTag(), "Network", "Normalize", normalizeValue))
    {
        if (normalizeValue == "true")
            mDataCollector.normalize();
    }

    // Learning and testing iterations.
    std::cout << "\n>>> Learning and testing <<<\n";
    for (unsigned int i = 0; i < CrossValidator::CV_ITERATIONS_NUMBER; ++i)
    {
        std::cout << "  > Iteration " << i << "...";
        learn(mDataCollector.getLearningData(mCrossValidator.getIterationStartRecordPosition(),
            mCrossValidator.getTestSetSize()));
        CrossValidator::TestResults testResults;
        test(testResults, mDataCollector.getCheckData(mCrossValidator.getIterationStartRecordPosition(),
            mCrossValidator.getTestSetSize()));
        mCrossValidator.addIterationInfo(testResults);
        std::cout << "done.\n";
        resetNetwork();
    }

    // Save work results
    std::cout << "\n>>> Saving results and stats <<< \n";
    this->saveResults();
    this->saveStats();

    std::cout << "\n >> SUCCESS! All operations done.\n";
}

void Network::loadParameters()
{
    int iterations = 0, inputNeurons = 0, outputNeurons = 0, hiddenLayers = 0;
    float alpha = 0.f, beta = 0.f, eta = 0.f;
    std::vector<int> hiddenLayerNeurons;

    // Obtain network parmeters and input/output layers neurons.
    mXMLParser.searchValue<int>(*mXMLParser.getRootTag(), "Network", "Iterations", iterations);
    mXMLParser.searchValue<float>(*mXMLParser.getRootTag(), "Network", "Alpha", alpha);
    mXMLParser.searchValue<float>(*mXMLParser.getRootTag(), "Network", "Beta", beta);
    mXMLParser.searchValue<float>(*mXMLParser.getRootTag(), "Network", "Eta", eta);
    mXMLParser.searchValue<int>(*mXMLParser.getRootTag(), "InputLayer", "Neurons", inputNeurons);
    mXMLParser.searchValue<int>(*mXMLParser.getRootTag(), "OutputLayer", "Neurons", outputNeurons);

    // Obtain hideen layer layout.
    int occurence = 0;
    int occurenceStart = 0;
    XMLParser::Tag * hiddenTag = nullptr;
    hiddenLayerNeurons.push_back(inputNeurons);
    hiddenTag = mXMLParser.searchTagByName(*mXMLParser.getRootTag(), "HiddenLayer", occurence);
    while (hiddenTag != nullptr)
    {
        int neurons;
        mXMLParser.searchValue<int>(*hiddenTag, "HiddenLayer", "Neurons", neurons);
        hiddenLayerNeurons.push_back(neurons);

        ++occurenceStart;
        occurence = occurenceStart;
        hiddenTag = mXMLParser.searchTagByName(*mXMLParser.getRootTag(), "HiddenLayer", occurence);
    }
    hiddenLayerNeurons.push_back(outputNeurons);

    // Set iterations and network parameters.
    mCrossValidator.setIterationsNumber(iterations);
    Neuron::setParameters(alpha, beta, eta);

    // Initialize layers.
    initializeLayers(hiddenLayerNeurons);


#ifdef __VERBOSE
    std::cout << "\nIterations: " << iterations << "\n";
    std::cout << "Alpha: " << alpha << ", Beta: " << beta << ", Eta: " << eta << "\n\n";
    std::cout << "Input neurons: " << inputNeurons << ", Output neurons: " << outputNeurons << "\n";
    std::cout << "Hidden layers: " << hiddenLayers << ". Hidden neurons: ";
    std::cout << "\n";
#endif

}

bool Network::acquireData()
{
    std::string filename = mInterface.getStringParam("--file");
    if (filename != "")
    {
        mDataCollector.setFilename(filename);
        mDataCollector.loadData();
    }
    else
    {
        std::cout << " <!> ERROR <!>\n";
        std::cout << "  No input file to read from!\n";
        return false;
    }

    return true;
}

void Network::saveResults()
{
    // Saving results
    std::string filename = mInterface.getStringParam("--output");
    if (filename != "")
    {
        mOutputManager.setResultsFilename(filename);
        mOutputManager.saveResults(&mDataCollector, false);
    }
    else
        mOutputManager.saveResults(&mDataCollector);
}

void Network::saveStats()
{
    // Saving stats
    std::string filename = mInterface.getStringParam("--stats");
    if (filename != "")
    {
        mOutputManager.setStatsFilename(filename);
        mOutputManager.saveStats(&mLayersTable, &mCrossValidator, false);
    }
    else
        mOutputManager.saveStats(&mLayersTable, &mCrossValidator);
}

void Network::initializeLayers(const int& inputLayer, const int& hiddenLayer, const int& outputLayer)
{

    mLayersTable.push_back(new Layer(inputLayer, 1));
    mLayersTable.push_back(new Layer(hiddenLayer, inputLayer));
    mLayersTable.push_back(new Layer(outputLayer, hiddenLayer));
}

void Network::initializeLayers(std::vector<int> layers)
{

    mLayersTable.push_back(new Layer(layers[0], 1));
    for (unsigned int i = 1; i < layers.size(); ++i)
        mLayersTable.push_back(new Layer(layers[i], layers[i - 1]));

}

void Network::learn(const std::vector<ProteinData *> data)
{

    for (auto &dataSet : data)
    {

        // PART 1 - CALCULATING OUTPUT

        //loading data
        mLayersTable[0]->setOutput(dataSet);

        for (unsigned int i = 1; i < mLayersTable.size() - 1; ++i)
        {
            mLayersTable[i]->commandToCalculate(*mLayersTable[i - 1]);
        }
        mLayersTable[mLayersTable.size() - 1]->calculateOutput(*mLayersTable[mLayersTable.size() - 2]);



        // PART 2 - INSTRUCTOR AND LEARNING
        mLayersTable[mLayersTable.size() - 1]->outputLayerError(dataSet->getReactionResult());

        for (unsigned int i = mLayersTable.size() - 2; i >= 1; --i)
        {
            mLayersTable[i]->calculateErrors(*mLayersTable[i + 1]);
        }


        // PART 3 - WEIGHT CORRECTION

        for (unsigned int i = 1; i < mLayersTable.size(); ++i)
        {
            mLayersTable[i]->correctWeights(*mLayersTable[i - 1]);
        }

    }

}

void Network::test(CrossValidator::TestResults & results, std::vector<ProteinData*> data)
{
    // PART 1 - CALCULATING OUTPUT

    for (auto & dataSet : data)
    {


        mLayersTable[0]->setOutput(dataSet);

        for (unsigned int i = 1; i < mLayersTable.size() - 1; ++i)
        {
            mLayersTable[i]->commandToCalculate(*mLayersTable[i - 1]);
        }

        mLayersTable[mLayersTable.size() - 1]->calculateOutput(*mLayersTable[mLayersTable.size() - 2]);

        float a = mLayersTable[mLayersTable.size() - 1]->getOutput();

        if (a > 0)
        {
            if (dataSet->getReactionResult() > 0)
                results.push_back(std::make_pair(true, true));
            else
                results.push_back(std::make_pair(false, true));
        }
        else
        {
            if (dataSet->getReactionResult() < 0)
                results.push_back(std::make_pair(false, false));
            else
                results.push_back(std::make_pair(true, false));
        }

    }
}

void Network::resetNetwork()
{
    for (unsigned int i = 1; i < mLayersTable.size(); ++i)
    {
        mLayersTable[i]->resetLayer();
    }
}