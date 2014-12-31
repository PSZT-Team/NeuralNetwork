#include "OutputManager.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

bool OutputManager::saveResults (DataCollector * dataCollector, bool defaultPath) {
    std::fstream output;
    std::string resultsPath = mResultsFilename;
    std::string statsPath = mStatsFilename;

    if (defaultPath) {
        auto time = std::time (nullptr);
        auto localTime = *std::localtime (&time);
        std::stringstream ss;
        ss << std::put_time (&localTime, "_%Y-%m-%d_%H.%M.%S.txt");
        resultsPath = RESULTS_FOLDER + resultsPath + ss.str();
        statsPath = STATS_FOLDER + statsPath + ss.str ();
    }

    // Saving results
    output.open (resultsPath, std::ios::out | std::ios::trunc);
    if (!output.is_open ()) {
        std::cout << " >> Saving results to file failed!  (file: " + resultsPath + ")\n";
        return false;
    }

    std::vector <ProteinData*> vecProteinData = dataCollector->getLearningData (100);
    for (ProteinData * protein : vecProteinData) {
        output << protein->getReactionResult () << "\n";
    }

    output.close ();

    // Saving stats
    output.open (statsPath, std::ios::out | std::ios::trunc);
    if (!output.is_open ()) {
        std::cout << " >> Saving results to file failed!  (file: " + statsPath + ")\n";
        return false;
    }

    output << "69% ;>\n";

    output.close ();

    return true;
}

void OutputManager::setResultsFilename (const std::string filename) {
    this->mResultsFilename = filename;
}

void OutputManager::setStatsFilename (const std::string filename) {
    this->mStatsFilename = filename;
}
