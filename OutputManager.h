/**
	Manages results and stats output file generating.
*/

#ifndef OUTPUT_MANAGER
#define OUTPUT_MANAGER

#define RESULTS_FOLDER "./_results/"
#define STATS_FOLDER "./_stats/"
#define DATA_FOLDER "./Data/"
#define HEADERS_FOLDER "./Data/headers/"
#define TEMPLATE_FILE "cv_template.txt."

#include <string>
#include "DataCollector.h"
#include "CrossValidator.h"

class OutputManager {
public:
    // Formatted CV stats files widths.
    const static unsigned int CV_WIDTH = 107;
    const static unsigned int CV_GLOBAL_WIDTH = 150;

    OutputManager() {};
    ~OutputManager() {};

	// Collects all data and saves results
	bool saveResults (DataCollector * dataCollector, bool defaultPath = true);

    // Collects stats and saves them
    // \param bool : Whether to use predefined path for files.
    bool saveStats (CrossValidator * crossValidator, bool defaultPath = true);

    /// Setters

    void setResultsFilename (const std::string);
    void setStatsFilename (const std::string);

private:
    // Saves global stats into special file.
    template <typename Type>
    bool saveGlobalStats (IterationInfo<Type> * iterationInfo, const std::string currentDatetime);

    // Replace file keyword with proper data
    // \param string keyword : Keyword in file to be found and replaced.
    // \param string value : String to be inserted instead of the given keyword.
    // \param string & text : Reference to text, in which the replacement is done.
    void replaceKeyword (const std::string keyword, const std::string value, std::string & text);

    // Returns row of data provided from cross validator in a text form.
    // \param DataContainer * : Pointer to dataset.
    // \param int iteration : Subset number for parsing.
    // \param bool isGlobal : Specifies whether row is for global stats or not.
    // \param (opt) bool isFormatted : Specifies whether row has to be formatted in columns (only matters if isGlobal == true).
    // \param (opt) bool isAverage : Specifies whether row is the latest row in the file and holds average values (only matters if isGlobal == false).
    std::string prepareRow (CrossValidator::DataContainer * dataContainer, const unsigned int iteration,
                            bool isGlobal, bool isFormatted = false, bool isAverage = false);

    // Returns row of data provided from cross validator in a text form.
    // \param IterationInfo * : Pointer to particular data structure.
    // \param bool isGlobal : Specifies whether row is for global stats or not.
    // \param (opt) bool isFormatted : Specifies whether row has to be formatted in columns (only if isGlobal == true).
    // \param (opt) bool isAverage : Specifies whether row is the latest row in the file and holds sum up values (only matters if isGlobal == false).
    template <typename Type>
    std::string prepareRow (IterationInfo<Type> * iterationInfo, bool isGlobal, 
                            bool isFormatted = false, bool isAverage = false);

    // Return string formatted to fit certain width.
    // \param Type value : Value to be formatted.
    // \param int width : Size of a field.
    // \param (opt) char fill : Character used to fill missing space.
    // \param (opt) string separator : String to be put at the end to separate different columns.
    template <typename Type>
    std::string formatWidth (const Type value, const unsigned int width, 
                             const char fill = '0', const std::string separator = " | ", 
                             bool showPos = false);

	// Filenames (with default values)
	std::string mResultsFilename = "results";
	std::string mStatsFilename = "cv";
};

#endif

