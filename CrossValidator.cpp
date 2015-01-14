#include "CrossValidator.h"

unsigned int CrossValidator::CV_ITERATIONS_NUMBER = CV_ITERATIONS_NUMBER_DEFAULT;

CrossValidator::CrossValidator (unsigned int capacity)
    : mCapacity (capacity) {
}

void CrossValidator::addIterationInfo (IterationInfo<unsigned int> & iterationInfo) {
    // Insert only if limit is not crossed
    if (mIterationCount < CV_ITERATIONS_NUMBER) {
        mData.push_back (iterationInfo);
        ++mIterationCount;
    }
}

void CrossValidator::addIterationInfo (TestResults & testResults) {
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
        IterationInfo<unsigned int> II;

        // Set ID.
        II.id = mIterationCount;

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
        II.mErrorRate = 1.f - II.mAccuracy;
        II.mSpecificity = (II.mTN) / (float)(II.mTN + II.mFP);
        II.mSensitivity = (II.mTP) / (float)(II.mTP + II.mFN);
        II.mGMean = sqrt (II.mSensitivity * II.mSpecificity);
        II.mAUC = (1.f + II.mSensitivity - (II.mFP / (II.mFP + II.mTP))) / 2.f;
        II.mF1 = 2.f * II.mTP / ((II.mTP + II.mFN) + (II.mTP + II.mFP));
        II.mMCC = ((float)II.mTP * II.mTN - (float)II.mFP * II.mFN) / (float)sqrt ((II.mTP + II.mFN) * (II.mTP + II.mFP) * (II.mTN + II.mFN) * (II.mTN + II.mFP));

        // New iteration is ready to be inserted.
        addIterationInfo (II);
    }
}

CrossValidator::DataContainer * CrossValidator::getDataContainer () {
    return &mData;
}

unsigned int CrossValidator::getTestSetSize () {
    return mCapacity / CV_ITERATIONS_NUMBER;
}

unsigned int CrossValidator::getIterationStartRecordPosition () {
    return (mCapacity / CV_ITERATIONS_NUMBER) * mIterationCount;
}

IterationInfo<float> CrossValidator::getAverageIterationInfo () {
    IterationInfo<float> averageII;
    for (unsigned int iteration = 0; iteration < CV_ITERATIONS_NUMBER; ++iteration) {
        averageII += mData[iteration];
    }
    averageII /= (float)CV_ITERATIONS_NUMBER;

    return averageII;
}

void CrossValidator::setCapacity (const unsigned int capacity) {
    mCapacity = capacity;

#ifdef __VERBOSE
    if (capacity % CV_ITERATIONS_NUMBER != 0) {
        std::cout << " >> WARNING: Number of all records is not divisible by iteration number.\n";
        std::cout << "             To achieve equinumerous test sets, some records will be ignored.\n";
    }
#endif
}

void CrossValidator::setIterationsNumber (const unsigned int iterationsNumber) {
    CV_ITERATIONS_NUMBER = iterationsNumber;
}
