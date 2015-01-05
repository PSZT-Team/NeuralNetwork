/*
    Holds all importatant calculation for current iteration.
*/

#ifndef ITERATION_INFO
#define ITERATION_INFO

template <typename Type>
class IterationInfo {
public:
    /// Iteration ID.
    unsigned int id = 0;

    /// Four main classifications.

    Type mTP = 0;   // True positive: Should be positive and is positive.
    Type mTN = 0;   // True negative: Should be negative and is negative.
    Type mFP = 0;   // False positive: Should be negative but is positive.
    Type mFN = 0;   // False negative: Should be positive but is negative.

    /// Validation quality indicators.

    float mAccuracy = 0.f;        // = (TP + TN) / (TP + TN + FN + FP). Equals  1 - Error Rate.
    float mErrorRate = 0.f;       // = (FP + FN) / (TP + TN + FN + FP). Equals  1 - Accuracy.
    float mSpecificity = 0.f;     // = TN / (TN + FP). Also known as TN rate.
    float mSensitivity = 0.f;     // = TP / (TP + FN). Also known as TP rate.
    float mPrecision = 0.f;       // = TP / (TP + FP). Also known ad Positive Predictive Value.

    /// Other important indicators.

    // Geometrical Mean = sqrt (sensitivity * specificity).
    float mGMean = 0.f;

    // Area Under the Curve = (1 + sensivity - FP Rate) / 2, where FP Rate = FP / (FP + TP).
    float mAUC = 0.f;

    // F1 score = 2TP / [(TP + FN) + (TP + FP)]
    float mF1 = 0.f;

    // Matthew's correlation coefficient = (TP * TN - FP * FN) / sqrt[ (TP + FN)(TP + FP)(TN + FN)(TN + FP) ]
    float mMCC = 0.f;

    // Adds all values.    
    template <typename T> 
    IterationInfo & operator+= (const IterationInfo<T> & other) {
        this->mTP += other.mTP;
        this->mTN += other.mTN;
        this->mFP += other.mFP;
        this->mFN += other.mFN;
        this->mAccuracy += other.mAccuracy;
        this->mErrorRate += other.mErrorRate;
        this->mSpecificity += other.mSpecificity;
        this->mSensitivity += other.mSensitivity;
        this->mPrecision += other.mPrecision;
        this->mGMean += other.mGMean;
        this->mAUC += other.mAUC;
        this->mMCC += other.mMCC;
        this->mF1 += other.mF1;

        return *this;
    }

    // Adds all values.
    IterationInfo & operator/= (const float factor) {
        this->mTP /= factor;
        this->mTN /= factor;
        this->mFP /= factor;
        this->mFN /= factor;
        this->mAccuracy /= factor;
        this->mErrorRate /= factor;
        this->mSpecificity /= factor;
        this->mSensitivity /= factor;
        this->mPrecision /= factor;
        this->mGMean /= factor;
        this->mAUC /= factor;
        this->mMCC /= factor;
        this->mF1 /= factor;

        return *this;
    }
};

#endif