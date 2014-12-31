//@autgor Andrzej Siadkowski
//class responsible for loading and storage data from input file;
//dasdad
#ifndef DATACOLLECTOR
#define DATACOLLECTOR

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>   
#include <random>
#include "ProteinData.h"

class DataCollector {
public:
	DataCollector();
	DataCollector (const std::string&  filename);
	~DataCollector ();

	//loading data from file 
	void loadData ();
	void getData ();

	void setFilename (const std::string filename);
	void shuffleData ();

private:

	int assignFile ();
	void getDataFromFile ();
	void closeFile ();
	
	void showData ();
	
	std::fstream dataFile;
	std::string file;
	
	std::vector <ProteinData*> data;
	int dataCounter;
};

#endif