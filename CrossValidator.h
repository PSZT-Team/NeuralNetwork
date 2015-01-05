/**
    Manages cross validation procedure, holds all necessary validation 
    indicators and results. Generates stats to be saved.
*/

#ifndef CROSS_VALIDATION
#define CROSS_VALIDATION

#define CV_ITERATIONS_NUMBER 10
#define CV_GLOBAL_FILE "cvResults.txt"
#define CV_GLOBAL_FORMATTED_FILE "cvResults_formatted.txt"

#include "IterationInfo.h"
#include <array>
#include <vector>
#include <iostream>
#include <iomanip>

class CrossValidator {
public:
    // Container for comparing results to expected values.
    // \param bool : Expected value.
    // \param bool : Actual result.
    typedef std::vector <std::pair <bool, bool> > TestResults;

    // Container for all iterations results.
    // \param IterationInfo : Results of one iteration.
    // \param int : Number of all iterations.
    typedef std::array <IterationInfo<unsigned int>, CV_ITERATIONS_NUMBER> DataContainer;

    CrossValidator () {};
    ~CrossValidator () {};

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

    // Set new capacity value.
    void setCapacity (const unsigned int capacity);

    // TEMP
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

    // TEMP
    // Output stream operator
    //friend std::ostream & operator<< (std::ostream & out, CrossValidator & cv) {
    //    out << "Iterations number: " << CV_ITERATIONS_NUMBER << "\n";

    //    // Return formatted data from all iterations
    //    for (unsigned int i = 0; i < CV_ITERATIONS_NUMBER; ++i) {
    //        IterationInfo iteration = cv.mData[i];

    //        out << std::setw (4) << iteration.mTP << "\n";
    //    }

    //    return out;
    //}

    
    // TEMP
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

