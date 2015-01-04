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
        resultsPath = RESULTS_FOLDER + resultsPath + ss.str();
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

bool OutputManager::saveStats (bool defaultPath) {
    std::fstream output;
    std::string statsPath = mStatsFilename;

    // Acquire desired final path.
    if (defaultPath) {
        auto time = std::time (nullptr);
        auto localTime = *std::localtime (&time);
        std::stringstream ss;
        ss << std::put_time (&localTime, "_%Y-%m-%d_%H.%M.%S.txt");
        statsPath = STATS_FOLDER + statsPath + ss.str ();
    }

    // Open file.
    output.open (statsPath, std::ios::out | std::ios::trunc);
    if (!output.is_open ()) {
        std::cout << " >> ERROR: Saving results to file failed!  (file: " + statsPath + ")\n";
        return false;
    }

    // Save stats.


    // TEMP
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

void OutputManager::saveGlobalStats () {
    std::fstream output;
    std::string row = "";
    std::string globalStatsPath = STATS_FOLDER;
    globalStatsPath += CV_RESULTS_FILE;


    
    //    auto time = std::time (nullptr);
    //    auto localTime = *std::localtime (&time);
    //    std::stringstream ss;
    //    ss << std::put_time (&localTime, "_%Y-%m-%d_%H.%M.%S.txt");
    //    statsPath = STATS_FOLDER + statsPath + ss.str ();

    //// Open file.
    //output.open (statsPath, std::ios::out | std::ios::trunc);
    //if (!output.is_open ()) {
    //    std::cout << " >> ERROR: Saving results to file failed!  (file: " + statsPath + ")\n";
    //    return false;
    //}

    // Save stats.


    // TEMP
    //utput << "69% ;>\n";



    output.close ();
}