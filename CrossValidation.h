/**
    Manages cross validation procedure, holds all necessary validation 
    indicators and results. Generates stats to be saved.
*/

#ifndef CROSS_VALIDATION
#define CROSS_VALIDATION

#define CV_ITERATIONS_NUMBER 10

#include <array>
#include <iomanip>

class CrossValidation {
public:
    CrossValidation () {};
    ~CrossValidation () {};

    // Hold all importatant calculation for current iteration
    typedef struct IterationInfo {
        // Four main classifications
        unsigned int mTP = 0;   // True positive: Should be positive and is positive.
        unsigned int mTN = 0;   // True negative: Should be negative and is negative.
        unsigned int mFP = 0;   // False positive: Should be negative but is positive.
        unsigned int mFN = 0;   // False negative: Should be positive but is negative.

        // Validation quality indicators
        float mAccuracy;        // = (TP + TN)  /  (TP + TN + FN + FP).
        float mSpecificity;     // = TN / (TN + FP). Also known as TN rate.
        float mSensitivity;     // = TP / (TP + FN). Also known as TP rate.

        // Other important indicators
        float mGMean;       // = sqrt (sensitivity * specificity).
        float mAUC;         // = (1 + sensivity - FP Rate) / 2,  where FP Rate = FP / (FP + TP)
    };

    // Insert new set of data
    void addIterationInfo (IterationInfo & iterationInfo);

private:
    // Holds all CVIteration
    std::array <IterationInfo, CV_ITERATIONS_NUMBER> mData;

    // Current inserted iteration counter
    unsigned int mIterationCount = 0;

    // Output stream operator
    friend std::ostream & operator<< (std::ostream & out, CrossValidation & object);
};

// Output stream definition
std::ostream & operator<< (std::ostream & out, CrossValidation & cv) {
    out << "Iterations number: " << CV_ITERATIONS_NUMBER << "\n";

    // Return formatted data from all iterations
    for (unsigned int i = 0; i < CV_ITERATIONS_NUMBER; ++i) {
        CrossValidation::IterationInfo iteration = cv.mData[i];

        out << std::setw (4) << iteration.mTP << "\n";
    }

    return out;
}

#endif

