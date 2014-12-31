#include "OutputManager.h"
#include <fstream>
#include <iostream>

bool OutputManager::saveResults(DataCollector * dataCollector) {
    std::fstream output;

    // Saving results
    output.open (mResultsFilename, std::ios::out | std::ios::trunc);
    if (!output.is_open ()) {
        std::cout << " >> Saving results to file " + mResultsFilename + " failed!\n";
        return false;
    }

    std::vector <ProteinData*> vecProteinData = dataCollector->getLearningData (100);
    for (ProteinData * protein : vecProteinData) {
        output << protein->getReactionResult () << "\n";
    }

    return true;
}

void OutputManager::setResultsFilename (const std::string filename) {
    this->mResultsFilename = filename;
}

void OutputManager::setStatsFilename (const std::string filename) {
    this->mStatsFilename = filename;
}
