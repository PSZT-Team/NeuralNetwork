#include "DataCollector.h"

DataCollector::DataCollector () {
}

DataCollector::DataCollector (const std::string&  filename) : file (filename) {
}


DataCollector::~DataCollector () {
}

unsigned int DataCollector::getDataCount () {
    return data.size ();
}

bool DataCollector::assignFile () {
    dataFile.open (file);
    if (!(dataFile.is_open ())) {
        std::cout << " >> Error! Opening file failed (file: " + file + ").\n";
        return false;
    }
    return true;
}

void DataCollector::closeFile () {
    dataFile.close ();
}

void DataCollector::getDataFromFile () {
    //char sep;

    std::string s;
    while (dataFile) {
        ProteinData* tempData = new ProteinData;
        if (!std::getline (dataFile, s))
            break;
        char sign;
        float number;
        float index;

        if (!(s[0] == ' ' || s[0] == '-'))
            continue;

        int counter;
        std::istringstream ss (s);
        // number one
        for (counter = 0; counter < 4; counter++) {

            std::string s2;
            ss.get (sign);
            if (!getline (ss, s2, 'e')) break;
            number = (float)atof (s2.c_str ());
            if (sign == '-')
                number *= (-1);

            ss.get (sign);


            if (!getline (ss, s2, ' ')) break;

            index = (float)atof (s2.c_str ());
            while (index > 0) {
                --index;

                if (sign == '+')
                    number *= 10;
                else
                    number /= 10;
            }

            switch (counter) {
                case 0:
                    tempData->setData1 (number);
                    break;
                case 1:
                    tempData->setData2 (number);
                    break;
                case 2:
                    tempData->setData3 (number);
                    break;
                case 3:
                    tempData->setReactionResult ((int)number);
                    break;
                default:
                    break;
            }
        }

        tempData->setId (data.size ());
        data.push_back (tempData);
        //++dataCounter;
    }

}


std::vector<ProteinData*> DataCollector::getLearningData (const unsigned int & begin, const unsigned int & length) {
    std::vector <ProteinData*> lerningData;

    shuffleData ();

    for (unsigned int i = 0; i < data.size (); ++i)
        if (!(i >= begin && i < (begin + length)))
            lerningData.push_back (new ProteinData (*data[i]));
    return lerningData;
}

std::vector<ProteinData*> DataCollector::getCheckData (const unsigned int & begin, const unsigned int & length) {
    std::vector <ProteinData*> checkData;

    unsigned int last = begin + length;
    if (last > data.size ())
        last = data.size ();

    for (unsigned int i = begin; i < last; ++i)
        checkData.push_back (new ProteinData (*data[i]));

    return checkData;
}

void DataCollector::normalize () {
    for (unsigned int column = 0; column < 3; ++column) {
        // Prepare pointers to methods.

        float (ProteinData::*get)(void) = nullptr;
        void (ProteinData::*set)(const float &) = nullptr;
        switch (column) {
            case 0:
                get = &ProteinData::getData1;
                set = &ProteinData::setData1;
                break;
            case 1:
                get = &ProteinData::getData2;
                set = &ProteinData::setData2;
                break;
            case 2:
                get = &ProteinData::getData3;
                set = &ProteinData::setData3;
                break;
            default:
                break;
        }

        // Calculate mean.
        float mean = 0.f;
        for (unsigned int i = 0; i < data.size (); ++i) {
            mean += (*data[i].*get) ();
        }
        mean /= (float)data.size ();

        // Calculate standard deviation.
        float deviation = 0.f;
        for (unsigned int i = 0; i < data.size (); ++i) {
            deviation += std::pow ((*data[i].*get)() - mean, 2);
        }
        deviation /= (float)data.size ();

        // Normalize.
        for (unsigned int i = 0; i < data.size (); ++i) {
            (*data[i].*set)(((*data[i].*get)() - mean) / deviation);
        }
    }
}

std::vector <ProteinData*> DataCollector::getTestData (const unsigned int & begin, const unsigned int & length) {
    std::vector <ProteinData*> testData = getCheckData (begin, length);

    for (auto &i : testData) {
        i->setReactionResult (0);
    }

    return testData;
}


void DataCollector::showData () {
    for (auto &i : data) {
        std::cout << i->getId () << " ---- " << i->getData1 () << " " << i->getData2 () << " " << i->getData3 () << " " << i->getReactionResult () << "\n";
    }

}


void DataCollector::shuffleData () {
    std::random_device rd;
    std::mt19937 g (rd ());
    std::shuffle (data.begin (), data.end (), g);

}


bool DataCollector::loadData () {
    if (!assignFile ())
        return false;

    getDataFromFile ();
    closeFile ();
    //showData ();
    //showData ();

    return true;
}


void DataCollector::setFilename (const std::string filename) {
    this->file = filename;
}

