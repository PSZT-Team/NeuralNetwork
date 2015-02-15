/**
    Manages cross validation procedure, holds all necessary validation
    indicators and results. Generates stats to be saved.
*/

#ifndef CROSS_VALIDATOR
#define CROSS_VALIDATOR

#define CV_ITERATIONS_NUMBER_DEFAULT 10
#define CV_GLOBAL_FILE "cv_global.txt"
#define CV_GLOBAL_FILE_DETAILED "cv_global_detailed.txt"
#define CV_GLOBAL_FORMATTED_FILE "cv_global_formatted.txt"
#define CV_GLOBAL_FORMATTED_FILE_DETAILED "cv_global_formatted_detailed.txt"


#include "IterationInfo.h"
#include <array>
#include <vector>
#include <iostream>
#include <iomanip>

class CrossValidator {
public:
    // Iterations for Network to learn and test results.
    static unsigned int CV_ITERATIONS_NUMBER;

    // Container for comparing results to expected values.
    // \param bool : Expected value.
    // \param bool : Actual result.
    typedef std::vector <std::pair <bool, bool> > TestResults;

    // Container for all iterations results.
    // \param IterationInfo : Results of one iteration.
    // \param int : Number of all iterations.
    typedef std::vector <IterationInfo<unsigned int> > DataContainer;

    CrossValidator () {
    };
    ~CrossValidator () {
    };

    // Constructor with additional parameters.
    // \param unsigned int : Number of all records.
    CrossValidator (unsigned int capacity);

    // Insert new set of data.
    // \param IterationInfo : New iteration to be inserted.
    void addIterationInfo (IterationInfo<unsigned int> & iterationInfo);

    // Insert new set of data based on provided results.
    // \param TestResult : Current iteration results.
    void addIterationInfo (TestResults & testResults);

    /// Getters

    // Returns pointer to DataContainer of this object.
    DataContainer * getDataContainer ();

    // Return number of records for next test set.
    unsigned int getTestSetSize ();

    // Return first unsused record number to be included in a next test set.
    unsigned int getIterationStartRecordPosition ();

    // Return average values from all IterationInfo's holding in this object's dataset.
    IterationInfo<float> getAverageIterationInfo ();

    /// Setters

    void setCapacity (const unsigned int capacity); // Set new capacity value.
    void setIterationsNumber (const unsigned int iterationsNumber); // Set number of learning and testing iterations.

    // Fills mData with random test values
    void fillWithRandomData () {
        std::srand (unsigned (time (NULL)));
        for (unsigned int iteration = 0; iteration < CV_ITERATIONS_NUMBER; ++iteration) {
            TestResults testResults;
            for (unsigned int iterSize = 0; iterSize < this->getTestSetSize (); ++iterSize) {
                testResults.push_back (generatePair ());
            }
            this->addIterationInfo (testResults);
        }
    }

private:
    // Holds all IterationInfo's
    DataContainer mData;

    // Current inserted iteration counter
    unsigned int mIterationCount = 0;

    // Number of all records
    unsigned int mCapacity;

    // Generates random pair with positive or negative values.
    std::pair <bool, bool> generatePair () {
        bool expected;
        bool result;
        expected = std::rand () % 2 == 0 ? false : true;
        result = std::rand () % 2 == 0 ? false : true;
        return std::make_pair (expected, result);
    }
};

#endif

