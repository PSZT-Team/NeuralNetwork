/*
    Holds all importatant calculation for current iteration.
*/

#ifndef ITERATION_INFO
#define ITERATION_INFO

class IterationInfo {
public:
    /// Four main classifications.

    unsigned int mTP = 0;   // True positive: Should be positive and is positive.
    unsigned int mTN = 0;   // True negative: Should be negative and is negative.
    unsigned int mFP = 0;   // False positive: Should be negative but is positive.
    unsigned int mFN = 0;   // False negative: Should be positive but is negative.

    /// Validation quality indicators.

    float mAccuracy;        // = (TP + TN) / (TP + TN + FN + FP). Equals  1 - Error Rate.
    float mErrorRate;       // = (FP + FN) / (TP + TN + FN + FP). Equals  1 - Accuracy.
    float mSpecificity;     // = TN / (TN + FP). Also known as TN rate.
    float mSensitivity;     // = TP / (TP + FN). Also known as TP rate.
    float mPrecision;       // = TP / (TP + FP). Also known ad Positive Predictive Value.

    /// Other important indicators.

    // Geometrical Mean = sqrt (sensitivity * specificity).
    float mGMean;

    // Area Under the Curve = (1 + sensivity - FP Rate) / 2, where FP Rate = FP / (FP + TP).
    float mAUC;

    // Matthew's correlation coefficient = (TP * TN - FP * FN) / sqrt[ (TP + FN)(TP + FP)(TN + FN)(TN + FP) ]
    float mMCC;

    // F1 score = 2TP / [(TP + FN) + (TP + FP)]
    float mF1;
};

#endif