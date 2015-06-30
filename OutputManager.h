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
#include "Neuron.h"
#include "Layer.h"
#include "Utilities.h"

class OutputManager
{
public:
    // Formatted CV stats files widths.
    const static unsigned int CV_WIDTH = 107;
    const static unsigned int CV_GLOBAL_WIDTH = 150;

    OutputManager()
    {};
    ~OutputManager()
    {};

    // Collects all data and saves results
    bool saveResults(DataCollector * dataCollector, bool defaultPath = true);

    // Collects stats and saves them
    // \param bool : Whether to use predefined path for files.
    bool saveStats(Layer::Layers * layers, CrossValidator * crossValidator, bool defaultPath = true);

    /// Setters

    void setResultsFilename(const std::string);
    void setStatsFilename(const std::string);
    void setLayers(Layer::Layers * layers);

private:
    // Saves global stats into special file.
    template <typename Type>
    bool saveGlobalStats(IterationInfo<Type> * iterationInfo, const std::string currentDatetime);

    // Returns row of data provided from cross validator in a text form.
    // \param DataContainer * : Pointer to dataset.
    // \param int iteration : Subset number for parsing.
    // \param bool isGlobal : Specifies whether row is for global stats or not.
    // \param (opt) bool isFormatted : Specifies whether row has to be formatted in columns (only matters if isGlobal == true).
    // \param (opt) bool isAverage : Specifies whether row is the latest row in the file and holds average values (only matters if isGlobal == false).
    // \param (opt) bool isDetailed : True if needed extra indicators saved in file (only if isGlobal == true).
    std::string prepareRow(CrossValidator::DataContainer * dataContainer, const unsigned int iteration,
                           bool isGlobal, bool isFormatted = false, bool isAverage = false,
                           bool isDetailed = false);

    // Returns row of data provided from cross validator in a text form.
    // \param IterationInfo * : Pointer to particular data structure.
    // \param bool isGlobal : Specifies whether row is for global stats or not.
    // \param (opt) bool isFormatted : Specifies whether row has to be formatted in columns (only if isGlobal == true).
    // \param (opt) bool isAverage : Specifies whether row is the latest row in the file and holds sum up values (only matters if isGlobal == false).
    // \param (opt) bool isDetailed : True if needed extra indicators saved in file (only if isGlobal == true).
    template <typename Type>
    std::string prepareRow(IterationInfo<Type> * iterationInfo, bool isGlobal,
                           bool isFormatted = false, bool isAverage = false,
                           bool isDetailed = false);

    // Return string formatted to fit certain width.
    // \param Type value : Value to be formatted.
    // \param int width : Size of a field.
    // \param (opt) char fill : Character used to fill missing space.
    // \param (opt) string separator : String to be put at the end to separate different columns.
    template <typename Type>
    std::string formatWidth(const Type value, const unsigned int width,
                            const char fill = '0', const std::string separator = " | ",
                            bool showPos = false);

    // Filenames (with default values)
    std::string mResultsFilename = "results";
    std::string mStatsFilename = "cv";
    Layer::Layers * mLayers;
};

#endif

