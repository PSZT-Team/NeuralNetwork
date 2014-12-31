/**
	Manages results, stats and output file generating.
*/

#ifndef OUTPUT_MANAGER
#define OUTPUT_MANAGER

#include <string>
#include "DataCollector.h"

class OutputManager {
public:
    OutputManager() {};
    ~OutputManager() {};

	// Collects all data and saves results
	bool saveResults (DataCollector * dataCollector);

    // Setters
    void setResultsFilename (const std::string);
    void setStatsFilename (const std::string);

private:
	// Filenames (with default values)
	std::string mResultsFilename = "_results.txt";
	std::string mStatsFilename = "_stats.txt";
};

#endif

