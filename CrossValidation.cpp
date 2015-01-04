#include "CrossValidation.h"

CrossValidation::CrossValidation (unsigned int capacity)
    : mCapacity (capacity) {
}

void CrossValidation::addIterationInfo (IterationInfo & iterationInfo) {
    // Insert only if limit is not crossed
    if (mIterationCount < CV_ITERATIONS_NUMBER) {
        mData[mIterationCount] = iterationInfo;
        ++mIterationCount;
    }
}

void CrossValidation::addIterationInfo (TestResults & testResults) { 
    // If results are empty.
    if (testResults.size () == 0) {
        std::cout << " >> ERROR: Provided results are empty!\n";
        std::cout << " >>        Iteration: " << mIterationCount << " (counting from 0).\n";
        std::cout << " >>        Expected results number: " << getTestSetSize () << ".\n";
        return;
    }
    // Otherwise...
    else {
        // Set new IterationInfo
        IterationInfo II;

        // For each <bool, bool> pair check classification.
        for (std::pair<bool, bool> & result : testResults) {

            // True positive
            if (result.first && result.second)
                ++II.mTP;

            // True negative
            else if (!result.first && !result.second)
                ++II.mTN;

            // False positive
            else if (!result.first && result.second)
                ++II.mFP;

            // False negative
            else if (result.first && !result.second)
                ++II.mFN;
        }

        // Calculate indicators
        II.mAccuracy = (II.mTP + II.mTN) / (float)(II.mTP + II.mTN + II.mFN + II.mFP);
        II.mSpecificity = (II.mTN) / (float)(II.mTN + II.mFP);
        II.mSensitivity = (II.mTP) / (float)(II.mTP + II.mFN);
        II.mGMean = sqrt (II.mSensitivity * II.mSpecificity);
        II.mAUC = (1.f + II.mSensitivity - (II.mFP / (II.mFP + II.mTP))) / 2.f;

        // New iteration is ready to be inserted.
        addIterationInfo (II);
    }
}

unsigned int CrossValidation::getTestSetSize () {
    return mCapacity / CV_ITERATIONS_NUMBER;
}

unsigned int CrossValidation::getIterationStartRecordPosition () {
    return (mCapacity / CV_ITERATIONS_NUMBER) * mIterationCount;
}

void CrossValidation::setCapacity (const unsigned int capacity) {
    mCapacity = capacity;

#ifdef __VERBOSE
    if (capacity % CV_ITERATIONS_NUMBER != 0) {
        std::cout << " >> WARNING: Number of all records is not divisible by iteration number.\n";
        std::cout << "             To achieve equinumerous test sets, some records will be ignored.\n";
    }
#endif
}