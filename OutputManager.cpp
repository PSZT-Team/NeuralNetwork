#include "OutputManager.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

bool OutputManager::saveResults(DataCollector * dataCollector, bool defaultPath)
{
    std::fstream output;
    std::string resultsPath = mResultsFilename;

    // Acquire desired final path.
    if (defaultPath)
    {
        auto time = std::time(nullptr);
        auto localTime = *std::localtime(&time);
        std::stringstream ss;
        ss << std::put_time(&localTime, "_%Y-%m-%d_%H.%M.%S.txt");
        resultsPath = RESULTS_FOLDER + resultsPath + ss.str();
    }

    // Open file.
    output.open(resultsPath, std::ios::out | std::ios::trunc);
    if (!output.is_open())
    {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Saving results to file failed!(file: " + resultsPath + ")\n";
        return false;
    }

    // Save results.
    std::vector <ProteinData*> vecProteinData = dataCollector->getLearningData(0, 100);
    for (ProteinData * protein : vecProteinData)
    {
        output << protein->getReactionResult() << "\n";
    }

    output.close();

    return true;
}

bool OutputManager::saveStats(Layer::Layers * layers, CrossValidator * crossValidator, bool defaultPath)
{
    setLayers(layers);
    std::fstream output;
    std::ifstream templateFile;
    std::string statsPath = mStatsFilename, templatePath = HEADERS_FOLDER,
        data = "", templateContent = "", currentDateTime = "";

    // Open file template with prepared header
    templatePath += TEMPLATE_FILE;
    templateFile.open(templatePath, std::ios::in);
    if (!templateFile.is_open())
    {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Cannot open cross validation stats template file! (file: " + templatePath + ")\n";
        return false;
    }

    // Acquire desired final path.
    std::stringstream ss;
    if (defaultPath)
    {
        auto time = std::time(nullptr);
        auto localTime = *std::localtime(&time);
        ss.str("");
        ss << std::put_time(&localTime, "_%Y-%m-%d_%H.%M.%S.txt");
        statsPath = STATS_FOLDER + statsPath + ss.str();
        ss.str("");
        ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        currentDateTime = ss.str();
    }

    // Open file.
    output.open(statsPath, std::ios::out | std::ios::trunc);
    if (!output.is_open())
    {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Saving results to file failed!  (file: " + statsPath + ")\n";
        return false;
    }

    // Copy template data into stringstream and then into string
    ss.str("");
    ss << templateFile.rdbuf();
    templateContent = ss.str();

    // Prepare headers
    Utilities::replaceKeyword("$date", currentDateTime, templateContent);
    Utilities::replaceKeyword("$iterations", std::to_string(CrossValidator::CV_ITERATIONS_NUMBER), templateContent);
    Utilities::replaceKeyword("$records", std::to_string(crossValidator->getTestSetSize() * CrossValidator::CV_ITERATIONS_NUMBER), templateContent);
    Utilities::replaceKeyword("$alpha", std::to_string(Neuron::ALPHA), templateContent);
    Utilities::replaceKeyword("$beta", std::to_string(Neuron::BETA), templateContent);
    Utilities::replaceKeyword("$eta", std::to_string(Neuron::ETA), templateContent);
    Utilities::replaceKeyword("$input", std::to_string(layers->operator[](0)->getLayerSize()), templateContent);
    Utilities::replaceKeyword("$output", std::to_string(layers->operator[](layers->size() - 1)->getLayerSize()), templateContent);

    // Hidden layers info.
    std::string hiddenLayers = "";
    for (unsigned int i = 1; i < layers->size() - 1; ++i)
    {
        hiddenLayers += std::to_string(layers->operator[](i)->getLayerSize()) + " ";
    }
    Utilities::replaceKeyword("$hidden", hiddenLayers, templateContent);

    // Prepare data for non-global stats.
    for (unsigned int iteration = 0; iteration < CrossValidator::CV_ITERATIONS_NUMBER; ++iteration)
    {
        std::string row = prepareRow(crossValidator->getDataContainer(), iteration, false, true, false, true);
        data += row;
    }

    // Insert data and average.
    IterationInfo<float> averageII = crossValidator->getAverageIterationInfo();
    Utilities::replaceKeyword("$rows", data, templateContent);
    Utilities::replaceKeyword("$average", prepareRow(&averageII, false, true, true, true), templateContent);

    // Save stats.
    output << templateContent;
    output.close();

    // Save global stats as well.
    if (!saveGlobalStats(&averageII, currentDateTime))
        return false;

    return true;
}

void OutputManager::setResultsFilename(const std::string filename)
{
    this->mResultsFilename = filename;
}

void OutputManager::setStatsFilename(const std::string filename)
{
    this->mStatsFilename = filename;
}

void OutputManager::setLayers(Layer::Layers * layers)
{
    this->mLayers = layers;
}

template <typename Type>
bool OutputManager::saveGlobalStats(IterationInfo<Type> * iterationInfo, const std::string currentDatetime)
{
    std::fstream output;
    //std::string row = "", rowF = "";
    std::string globalStatsPath = DATA_FOLDER, globalStatsPathF = DATA_FOLDER,
        globalStatsPathDetailed = DATA_FOLDER, globalStatsPathDetailedF = DATA_FOLDER;
    globalStatsPath += CV_GLOBAL_FILE;
    globalStatsPathF += CV_GLOBAL_FORMATTED_FILE;
    globalStatsPathDetailed += CV_GLOBAL_FILE_DETAILED;
    globalStatsPathDetailedF += CV_GLOBAL_FORMATTED_FILE_DETAILED;

    // Open global NON-formatted file.
    output.open(globalStatsPath, std::ios::out | std::ios::app);
    if (!output.is_open())
    {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Saving global results to file failed!  (file: " + globalStatsPath + ")\n";
        return false;
    }

    // Save non-formatted file data.
    output << currentDatetime << ";" << prepareRow(iterationInfo, true, false, false, false);
    output.close();

    // Open global NON-formatted, detailed file.
    output.open(globalStatsPathDetailed, std::ios::out | std::ios::app);
    if (!output.is_open())
    {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Saving global detailed results to file failed!  (file: " + globalStatsPathDetailed + ")\n";
        return false;
    }

    // Save non-formatted detailed file data.
    output << currentDatetime << ";" << prepareRow(iterationInfo, true, false, false, true);
    output.close();

    // Open global formatted file.
    output.open(globalStatsPathF, std::ios::out | std::ios::app);
    if (!output.is_open())
    {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Saving global formatted results to file failed!  (file: " + globalStatsPathF + ")\n";
        return false;
    }

    // Save formatted file data.
    output << currentDatetime << " | " << prepareRow(iterationInfo, true, true, false, false);
    output.close();

    // Open global formatted detailed file.
    output.open(globalStatsPathDetailedF, std::ios::out | std::ios::app);
    if (!output.is_open())
    {
        std::cout << "\n <!> ERROR <!>\n";
        std::cout << "  Saving global formatted detailed results to file failed!  (file: " + globalStatsPathDetailedF + ")\n";
        return false;
    }

    // Save formatted detailed file data.
    output << currentDatetime << " | " << prepareRow(iterationInfo, true, true, false, true);
    output.close();

    return true;
}

std::string OutputManager::prepareRow(CrossValidator::DataContainer * dataContainer, const unsigned int iteration,
                                      bool isGlobal, bool isFormatted, bool isAverage, bool isDetailed)
{
    return prepareRow<unsigned int>(&dataContainer->operator[](iteration), isGlobal, isFormatted, isAverage, isDetailed);
}

template <typename Type>
std::string OutputManager::prepareRow(IterationInfo<Type> * iterationInfo, bool isGlobal,
                                      bool isFormatted, bool isAverage, bool isDetailed)
{
    std::string row = "";
    std::string separator = " | ";

    // Global stats.
    if (isGlobal)
    {

        // Formatted.
        if (isFormatted)
        {
            // Add formatted iterations and records.
            row += formatWidth<unsigned int>(CrossValidator::CV_ITERATIONS_NUMBER, 10, ' ', separator);
            row += formatWidth<unsigned int>((unsigned int)(iterationInfo->mTP + iterationInfo->mTN +
                iterationInfo->mFP + iterationInfo->mFN) * CrossValidator::CV_ITERATIONS_NUMBER,
                7, ' ', separator);
        }
        // Non-formatted.
        else
        {
            // Add non-formatted iterations and records.
            separator = ";";
            row += formatWidth<unsigned int>(CrossValidator::CV_ITERATIONS_NUMBER, 0, ' ', separator);
            row += formatWidth<unsigned int>((unsigned int)(iterationInfo->mTP + iterationInfo->mTN +
                iterationInfo->mFP + iterationInfo->mFN) * CrossValidator::CV_ITERATIONS_NUMBER,
                0, ' ', separator);
        }
    }
    // Non-global stats.
    else
    {

        // Average.
        if (isAverage)
        {
            // DO NOTHING
        }
        // Non-average/
        else
        {
            // Add iteration
            row += formatWidth<unsigned int>(iterationInfo->id, 9, ' ', separator);
        }
    }

    unsigned int size = 0;

    if (isGlobal)
    {

        // Add network parameters info.
        size = isFormatted ? 5 : 0;
        row += formatWidth<float>(Neuron::ALPHA, size, ' ', separator);
        row += formatWidth<float>(Neuron::BETA, size, ' ', separator);
        row += formatWidth<float>(Neuron::ETA, size, ' ', separator);

        // Add layers information.
        std::string hiddenLayers = "";
        for (unsigned int i = 1; i < mLayers->size() - 1; ++i)
        {
            hiddenLayers += std::to_string(mLayers->operator[](i)->getLayerSize());
            if (i != mLayers->size() - 2)
                hiddenLayers += " ";
        }
        row += formatWidth<int>(mLayers->operator[](0)->getLayerSize(), size, ' ', separator);
        size = isFormatted ? 6 : 0;
        row += formatWidth<std::string>(hiddenLayers, size, ' ', separator);
        row += formatWidth<int>(mLayers->operator[](mLayers->size() - 1)->getLayerSize(), size, ' ', separator);
    }

    // Add classifications.
    size = isFormatted ? 7 : 0;
    row += formatWidth<Type>(iterationInfo->mTP, size, ' ', separator);
    row += formatWidth<Type>(iterationInfo->mTN, size, ' ', separator);
    row += formatWidth<Type>(iterationInfo->mFP, size, ' ', separator);
    row += formatWidth<Type>(iterationInfo->mFN, size, ' ', separator);

    // Add indicators.
    size = isFormatted ? 5 : 0;
    row += formatWidth<float>(iterationInfo->mAccuracy, size, '0', separator);

    // Detailed info.
    if (isDetailed)
    {
        row += formatWidth<float>(iterationInfo->mSpecificity, size, '0', separator);
        row += formatWidth<float>(iterationInfo->mSensitivity, size, '0', separator);
        row += formatWidth<float>(iterationInfo->mPrecision, size, '0', separator);
        row += formatWidth<float>(iterationInfo->mGMean, size, '0', separator);
        row += formatWidth<float>(iterationInfo->mAUC, size, '0', separator);
        row += formatWidth<float>(iterationInfo->mF1, size, '0', separator);
        row += formatWidth<float>(iterationInfo->mMCC, size, '0', separator, true);
    }

    row += "\n";

    return row;
}

template <typename Type>
std::string OutputManager::formatWidth(const Type value, const unsigned int width,
                                       const char fill, const std::string separator,
                                       bool showPos)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3) << std::left << std::setw(width)
        << std::setfill(fill) << (showPos ? std::showpos : std::noshowpos)
        << value << separator;

    return ss.str();
}