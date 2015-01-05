#include "OutputManager.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

bool OutputManager::saveResults (DataCollector * dataCollector, bool defaultPath) {
    std::fstream output;
    std::string resultsPath = mResultsFilename;

    // Acquire desired final path.
    if (defaultPath) {
        auto time = std::time (nullptr);
        auto localTime = *std::localtime (&time);
        std::stringstream ss;
        ss << std::put_time (&localTime, "_%Y-%m-%d_%H.%M.%S.txt");
        resultsPath = RESULTS_FOLDER + resultsPath + ss.str ();
    }

    // Open file.
    output.open (resultsPath, std::ios::out | std::ios::trunc);
    if (!output.is_open ()) {
        std::cout << " >> ERROR: Saving results to file failed!  (file: " + resultsPath + ")\n";
        return false;
    }

    // Save results.
    std::vector <ProteinData*> vecProteinData = dataCollector->getLearningData (0, 100);
    for (ProteinData * protein : vecProteinData) {
        output << protein->getReactionResult () << "\n";
    }

    output.close ();

    return true;
}

bool OutputManager::saveStats (CrossValidator * crossValidator, bool defaultPath) {
    std::fstream output;
    std::ifstream templateFile;
    std::string statsPath = mStatsFilename, templatePath = HEADERS_FOLDER,
        data = "", templateContent = "", currentDateTime = "";

    // Open file template with prepared header
    templatePath += TEMPLATE_FILE;
    templateFile.open (templatePath, std::ios::in);
    if (!templateFile.is_open ()) {
        std::cout << " >> ERROR: Cannot open cross validation stats template file! (file: " + templatePath + ")\n";
        return false;
    }

    // Acquire desired final path.
    std::stringstream ss;
    if (defaultPath) {
        auto time = std::time (nullptr);
        auto localTime = *std::localtime (&time);
        ss.str ("");
        ss << std::put_time (&localTime, "_%Y-%m-%d_%H.%M.%S.txt");
        statsPath = STATS_FOLDER + statsPath + ss.str ();
        ss.str ("");
        ss << std::put_time (&localTime, "%Y-%m-%d %H:%M:%S");
        currentDateTime = ss.str ();
    }

    // Open file.
    output.open (statsPath, std::ios::out | std::ios::trunc);
    if (!output.is_open ()) {
        std::cout << " >> ERROR: Saving results to file failed!  (file: " + statsPath + ")\n";
        return false;
    }

    // Copy template data into stringstream and then into string
    ss.str ("");
    ss << templateFile.rdbuf ();
    templateContent = ss.str ();

    // Prepare headers
    replaceKeyword ("$date", currentDateTime, templateContent);
    replaceKeyword ("$iterations", std::to_string (CV_ITERATIONS_NUMBER), templateContent);
    replaceKeyword ("$records", std::to_string (crossValidator->getTestSetSize () * CV_ITERATIONS_NUMBER), templateContent);

    // Prepare data for non-global stats.
    for (unsigned int iteration = 0; iteration < CV_ITERATIONS_NUMBER; ++iteration) {
        std::string row = prepareRow (crossValidator->getDataContainer (), iteration, false, true);
        data += row;
    }

    // Insert data and average.
    IterationInfo<float> averageII = crossValidator->getAverageIterationInfo ();
    replaceKeyword ("$rows", data, templateContent);
    replaceKeyword ("$average", prepareRow (&averageII, false, true, true), templateContent);

    // Save stats.
    output << templateContent;
    output.close ();

    // Save global stats as well.
    if (!saveGlobalStats (&averageII, currentDateTime))
        return false;

    return true;
}

void OutputManager::setResultsFilename (const std::string filename) {
    this->mResultsFilename = filename;
}

void OutputManager::setStatsFilename (const std::string filename) {
    this->mStatsFilename = filename;
}

template <typename Type>
bool OutputManager::saveGlobalStats (IterationInfo<Type> * iterationInfo, const std::string currentDatetime) {
    std::fstream output;
    //std::string row = "", rowF = "";
    std::string globalStatsPath = DATA_FOLDER, globalStatsPathF = DATA_FOLDER;
    globalStatsPath += CV_GLOBAL_FILE;
    globalStatsPathF += CV_GLOBAL_FORMATTED_FILE;

    // Open global NON-formatted file.
    output.open (globalStatsPath, std::ios::out | std::ios::app);
    if (!output.is_open ()) {
        std::cout << " >> ERROR: Saving global results to file failed!  (file: " + globalStatsPath + ")\n";
        return false;
    }

    // Save non-formatted file data.
    output << currentDatetime << ";" << prepareRow (iterationInfo, true, false);
    output.close ();

    // Open global NON-formatted file.
    output.open (globalStatsPathF, std::ios::out | std::ios::app);
    if (!output.is_open ()) {
        std::cout << " >> ERROR: Saving global formatted results to file failed!  (file: " + globalStatsPathF + ")\n";
        return false;
    }

    // Save non-formatted file data.
    output << currentDatetime << " | " << prepareRow (iterationInfo, true, true);
    output.close ();

    return true;
}

void OutputManager::replaceKeyword (const std::string keyword, const std::string value, std::string & text) {
    // If keyword is not found, print an error.
    unsigned int keywordPos = text.find (keyword);
    if (keywordPos == std::string::npos) {
        std::cout << " >> ERROR: Keyword '" + keyword + "' cannot be found!\n" +
            "           Check  " + HEADERS_FOLDER + "  for  " + TEMPLATE_FILE + ".\n";
        return;
    }

    // Otherwise...
    std::string before = "", after = "";
    before = text.substr (0, keywordPos);
    after = text.substr (keywordPos + keyword.size (), text.size () - (keywordPos + keyword.size ()));

    text = before + value + after;
}

std::string OutputManager::prepareRow (CrossValidator::DataContainer * dataContainer, const unsigned int iteration,
                                       bool isGlobal, bool isFormatted, bool isAverage) {
    return prepareRow<unsigned int> (&dataContainer->operator[](iteration), isGlobal, isFormatted, isAverage);
}

template <typename Type>
std::string OutputManager::prepareRow (IterationInfo<Type> * iterationInfo, bool isGlobal,
                                       bool isFormatted, bool isAverage) {
    std::string row = "";
    std::string separator = " | ";

    // Global stats.
    if (isGlobal) {

        // Formatted.
        if (isFormatted) {
            // Add formatted iterations and records.
            row += formatWidth<unsigned int> (CV_ITERATIONS_NUMBER, 10, ' ', separator);
            row += formatWidth<unsigned int> ((iterationInfo->mTP + iterationInfo->mTN +
                iterationInfo->mFP + iterationInfo->mFN) * CV_ITERATIONS_NUMBER,
                7, ' ', separator);
        }
        // Non-formatted.
        else {
            // Add non-formatted date, iterations and records.
            separator = ";";
            row += formatWidth<unsigned int> (CV_ITERATIONS_NUMBER, 0, ' ', separator);
            row += formatWidth<unsigned int> ((iterationInfo->mTP + iterationInfo->mTN +
                iterationInfo->mFP + iterationInfo->mFN) * CV_ITERATIONS_NUMBER,
                0, ' ', separator);
        }
    }
    // Non-global stats.
    else {

        // Average.
        if (isAverage) {
            // DO NOTHING
        }
        // Non-average/
        else {
            // Add iteration
            row += formatWidth<unsigned int> (iterationInfo->id, 9, ' ', separator);
        }
    }

    // Add classifications.
    unsigned int size = isFormatted ? 5 : 0;
    row += formatWidth<Type> (iterationInfo->mTP, size, ' ', separator);
    row += formatWidth<Type> (iterationInfo->mTN, size, ' ', separator);
    row += formatWidth<Type> (iterationInfo->mFP, size, ' ', separator);
    row += formatWidth<Type> (iterationInfo->mFN, size, ' ', separator);

    // Add indicators.
    row += formatWidth<float> (iterationInfo->mAccuracy, size, '0', separator);
    row += formatWidth<float> (iterationInfo->mSpecificity, size, '0', separator);
    row += formatWidth<float> (iterationInfo->mSensitivity, size, '0', separator);
    row += formatWidth<float> (iterationInfo->mPrecision, size, '0', separator);
    row += formatWidth<float> (iterationInfo->mGMean, size, '0', separator);
    row += formatWidth<float> (iterationInfo->mAUC, size, '0', separator);
    row += formatWidth<float> (iterationInfo->mF1, size, '0', separator);
    row += formatWidth<float> (iterationInfo->mMCC, size, '0', separator, true);

    row += "\n";

    return row;
}

template <typename Type>
std::string OutputManager::formatWidth (const Type value, const unsigned int width,
                                        const char fill, const std::string separator,
                                        bool showPos) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision (3) << std::left << std::setw (width)
        << std::setfill (fill) << (showPos ? std::showpos : std::noshowpos)
        << value << separator;

    return ss.str ();
}