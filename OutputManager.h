/**
	Manages results and stats output file generating.
*/

#ifndef OUTPUT_MANAGER
#define OUTPUT_MANAGER

#define RESULTS_FOLDER "./_results/"
#define STATS_FOLDER "./_stats/"

#include <string>
#include "DataCollector.h"
#include "CrossValidator.h"

class OutputManager {
public:
    OutputManager() {};
    ~OutputManager() {};

	// Collects all data and saves results
	bool saveResults (DataCollector * dataCollector, bool defaultPath = true);

    // Collects stats and saves them
    // \param bool : Whether to use predefined path for files.
    bool saveStats (bool defaultPath = true);

    /// Setters

    void setResultsFilename (const std::string);
    void setStatsFilename (const std::string);

private:
    // Saves global stats into special file.
    void saveGlobalStats ();

	// Filenames (with default values)
	std::string mResultsFilename = "results";
	std::string mStatsFilename = "stats";
};

#endif

