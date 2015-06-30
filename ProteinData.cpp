#include "ProteinData.h"


ProteinData::ProteinData()
{}


ProteinData::~ProteinData()
{}


void ProteinData::setData1(const float & data)
{
    data1 = data;
}

void ProteinData::setData2(const float & data)
{
    data2 = data;
}
void ProteinData::setData3(const float & data)
{
    data3 = data;
}

void ProteinData::setReactionResult(const int & result)
{
    reactionResult = result;
}

void ProteinData::setId(const int & i)
{
    id = i;

}

float ProteinData::getData1()
{
    return data1;
}


float ProteinData::getData2()
{
    return data2;
}


float ProteinData::getData3()
{
    return data3;
}

int ProteinData::getReactionResult()
{
    return reactionResult;
}


int ProteinData::getId()
{
    return id;
}