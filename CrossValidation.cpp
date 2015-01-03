#include "CrossValidation.h"

void CrossValidation::addIterationInfo (IterationInfo & iterationInfo) {
    // Insert only if limit is not crossed
    if (mIterationCount < CV_ITERATIONS_NUMBER) {
        mData[mIterationCount] = iterationInfo;
        ++mIterationCount;
    }
}