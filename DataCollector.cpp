#include "DataCollector.h"


DataCollector::DataCollector (const std::string&  filename) : file (filename), dataCounter  (0) {
}


DataCollector::~DataCollector () {
}

int DataCollector::assignFile () {
	dataFile.open (file);
	if (!(dataFile.is_open ())) {
		std::cout << "ERROR OCURED \n";
		return 1;
	}
	return 0;
}

void DataCollector::closeFile () {
	dataFile.close ();
}

void DataCollector::getDataFromFile ()
{
	char sep;

	std::string s;
	while (dataFile) {
		ProteinData* tempData = new ProteinData;
		if (!std::getline (dataFile, s))
			break;
		char sign;
		float number;
		float index;

		if (!(s[0] == ' ' || s[0]== '-'))
			continue;

		int counter;
		std::istringstream ss (s);
		// number one
		for (counter = 0; counter < 4; counter++) {

			std::string s2;
			ss.get (sign);
			if (!getline (ss, s2, 'e')) break;
			number = atof (s2.c_str ());
			if (sign == '-')
				number *= (-1);

			ss.get (sign);


			if (!getline (ss, s2, ' ')) break;

			index = atof (s2.c_str ());
			while (index > 0) {
				--index;

				if (sign == '+')
					number *= 10;
				else
					number /= 10;
			}

			switch (counter) {
				case 0:
					tempData->data1 = number;
					break;
				case 1:
					tempData->data2 = number;
					break;
				case 2:
					tempData->data3 = number;
					break;
				case 3:
					tempData->reactionResult = number;
					break;
				default:
					break;
			}


	
		}
		

		tempData->number = dataCounter;
		data.push_back (tempData);
		++dataCounter;
	}

}


void DataCollector::showData () {
	for (auto &i : data) {
		std::cout << i->number << " ---- " << i->data1 << " " << i->data2 << " " << i->data3 << " " << i->reactionResult << "\n";
	}

}


void DataCollector::randomData ()  {

	std::random_device rd;
	std::mt19937 g (rd ());
	std::shuffle (data.begin (), data.end (), g);


}


void DataCollector::loadData () {
	assignFile ();
	getDataFromFile ();
	closeFile ();
	showData ();
	randomData ();
//	showData ();
}

