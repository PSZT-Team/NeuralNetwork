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
	DataCollector (const std::string&  filename);
	~DataCollector ();

	//loading data from file 
	void loadData ();
	void getData ();

private:

	int assignFile ();
	void getDataFromFile ();
	void closeFile ();
	void randomData ();
	void showData ();
	
	std::fstream dataFile;
	std::string file;
	
	std::vector <ProteinData*> data;
	int dataCounter;
};

#endif